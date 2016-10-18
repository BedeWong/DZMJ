#include "MJ_Player.h"
#include "MJ_AnalyResult.h"
#include <cstring>
#include <stack>

#ifdef DEBUG

  #include <iostream>
  using namespace std;

#endif

bool MJ_Player::hasSanLian(MJ_Base::CARD *li, MJ_Base::CARD c, CARD **res)
{
    if(isFENG(c))
        return false;

    CARD *it = li;
    int flg = 0;
    while(*it && flg<3)
    {
        if(*it == c)
        {
            res[flg++] = it;
            c--;
        }
        it++;
    }
    if(flg == 3)
    {
        return true;
    }
    return false;
}

bool MJ_Player::hasErLian(MJ_Base::CARD *li, MJ_Base::CARD c, CARD **res)
{
    if(isFENG(c))
        return false;

    CARD *it = li;
    int flg = 0;
    while(*it && flg<2)
    {
        if(*it == c)
        {
            res[flg++] = it;
            c--;
        }
        it++;
    }
    if(flg == 2)
    {
        return true;
    }
    return false;
}

bool MJ_Player::hasKanZhang(MJ_Base::CARD *li, MJ_Base::CARD c, CARD **res)
{
    if(isFENG(c))
        return false;

    CARD *it = li;
    int flg = 0;
    while(*it && flg<2)
    {
        if(*it == c)
        {
            res[flg++] = it;
            c-=2;
        }
        it++;
    }
    if(flg == 2)
    {
        return true;
    }
    return false;
}

MJ_Player::MJ_Player()
{

}

void MJ_Player::init(MJ_Player::pCCARD _paiList, MJ_Base::CARD _wang)
{
    _g = 0;
    _p = 0;
    _c = 0;
    _h = 0;

    memset(this->paiList, 0, sizeof(this->paiList));
    memset(this->HuList, 0, sizeof(this->HuList));
    memset(this->cGangList, 0, sizeof(this->cGangList));
    memset(this->cPengList, 0, sizeof(this->cPengList));
    memset(this->cChiList, 0, sizeof(this->cChiList));
    memset(this->gang, 0, sizeof(this->gang));
    memset(this->peng, 0, sizeof(this->peng));
    memset(this->chi, 0, sizeof(this->chi));

    this->wang = _wang;
    this->NewCard = MJ_noCard;

    this->paiCount = strlen(_paiList);
    if(this->paiCount > 13)
        throw(0);
    memcpy(this->paiList, _paiList, this->paiCount);
    MJ_sort(this->paiList, this->paiCount);

#ifdef DEBUG
    cout << "*初始化游戏- ： "  << this->paiList << "牌张数:" << this->paiCount << endl;
#endif
}

void MJ_Player::addCard(MJ_Base::CARD nc)
{
    auto i = this->paiCount;
    for(; i >0; i--)
    {
        if(this->paiList[i-1] < nc)
        {
           this->paiList[i] = this->paiList[i-1];
           continue;
        }
        break;
    }

    this->paiList[i] = nc;
    this->paiCount++;
#ifdef DEBUG
    cout << "*增加牌:" << nc << ", " << this->paiList << "  " << this->paiCount << endl;
#endif
}

int MJ_Player::ChuPai(MJ_Base::CARD c)
{
    this->paiRecList[this->paiRecCount++] = c;
    return DelCard(c);
}

int MJ_Player::DelCard(MJ_Base::CARD card)
{
    auto i = 0;
    bool flag = false;

    for (; i<this->paiCount; i++)
    {
        if(flag)
        {
            this->paiList[i-1] = this->paiList[i];
            continue;
        }
        if(this->paiList[i] == card)
        {
            flag = true;
        }
    }

    if (!flag)
    {
#ifdef DEBUG
        cout << "*删除不成功." << __FUNCTION__ << endl;
        cout << "*删除一张牌-：" << card << " " <<  this->paiList << " " << this->paiCount << endl;
#endif
        return -1;
    }

    this->paiList[i-1] = MJ_noCard;
    this->paiCount -= 1;

#ifdef DEBUG
        cout << "*删除一张牌-：" << card << " " <<  this->paiList << " " << this->paiCount << endl;
#endif
    return 0;
}

int MJ_Player::DelCard(int offset)
{
    if (offset < 0 || offset >= this->paiCount)
    {
#ifdef DEBUG
        cout << "*删除不成功." << __FUNCTION__ << endl;
        cout << "*删除一张牌-：" << offset << " " <<  this->paiList << " " << this->paiCount << endl;
#endif
        return -1;
    }

    for(auto i=offset; i<this->paiCount-1; i++)
    {
        this->paiList[i] = this->paiList[i+1];
    }

    this->paiCount -= 1;
    this->paiList[this->paiCount] = MJ_noCard;

#ifdef DEBUG
        cout << "*删除一张牌-：" << offset << " " <<  this->paiList << " " << this->paiCount << endl;
#endif
    return 0;
}

/***  *****  *****  *****  [吃牌]  *****  *****  *****
 * 吃牌规则：风不能吃，  王不能吃，   假王（中风）可以吃（前提是本局王不是风的情况下）,假王替代真王原身,   可以吃王旁边的牌
 * 假设手上的牌总是排序好的  ? > 条 > 筒 > 风
 * 单张牌最多有2种吃法，5种形式
 *      去重 -> 只计算可吃的牌不计算如何吃
 *      吃王 -> 用红中代替
 *      有红中：
*/
int MJ_Player::cChi()
{
    CARD cards[16] = {0};
    int count = 0;

    //  去重
    for(auto i=0; i<this->paiCount; i++)
    {
        if(this->paiList[i] == this->paiList[i+1])
            continue;

        // 若牌中有红中， 添真王
        if(this->paiList[i] == MJ_ZHONG)
        {
            for(auto i=0; i<count; i++)
            {
                if(cards[i] > this->wang)         // 找位置
                    continue;
                else if(cards[i] == this->wang)     //存在了
                    break;
                else
                {
                    for(auto j=count; j>i; cards[j] = cards[j-1], j--);
                    cards[i] = this->wang;
                    count++;
                    break;
                }
            }
        }

        if(isFENG(this->paiList[i]))   // 过滤掉 字牌
            continue;

        cards[count] = this->paiList[i];
        count++;
    }

    if(count <= 1)// 少于两张不同的牌 可以不用判断了
    {
#ifdef DEBUG
        cout << "*计算可吃-：没牌可吃。" << this->paiList << endl;
#endif
        //  不要忘记更新到结果集中
        memset(this->cChiList, 0, sizeof(this->cChiList));
        return 0;
    }

#ifdef DEBUG
    cout << "*计算可吃-：去重后：" << cards << endl;
#endif

    int cLen = 0;
    for(auto i=0; i<count - 1; i++)
    {
        CARD tmp;
        cout << cards[i];
        if(cards[i] == cards[i+1] + 1)  // 二连
        {
            tmp = cards[i] + 1;
            if(isMJCARD(tmp))
            {
                if(tmp == this->wang && !strchr(this->cChiList, MJ_ZHONG))   //真王不能吃， 用红中代替
                    this->cChiList[cLen++] = MJ_ZHONG;
                else if(!strchr(this->cChiList, tmp))
                    this->cChiList[cLen++] = tmp;
                else
                {;}
            }

            tmp = cards[i] - 2;
            if(isMJCARD(tmp))
            {
                if(tmp == this->wang && !strchr(this->cChiList, MJ_ZHONG))
                    this->cChiList[cLen++] = MJ_ZHONG;
                else if(!strchr(this->cChiList, tmp))
                    this->cChiList[cLen++] = tmp;
                else
                {
                    ;
                }
            }
        }
        else if (cards[i] == cards[i+1] + 2) // 坎张
        {
            tmp = cards[i] - 1;
            if(isMJCARD(tmp))
            {
                if(tmp == this->wang && !strchr(this->cChiList, MJ_ZHONG))
                    this->cChiList[cLen++] = MJ_ZHONG;
                else if(!strchr(this->cChiList, tmp))
                    this->cChiList[cLen++] = tmp;
            }
        }
        else
        {

        }

        if(i>0 && cards[i-1]-1 == cards[i] && cards[i] == cards[i+1]+1)   // 三连一样可以吃
        {
            tmp = cards[i];
            if(tmp == this->wang && !strchr(this->cChiList, MJ_ZHONG))
                this->cChiList[cLen++] = MJ_ZHONG;
            else if(!strchr(this->cChiList, tmp))
                this->cChiList[cLen++] = tmp;
        }
    }

    this->cChiCount = cLen;
    return cLen;
}

int MJ_Player::cPeng()
{
    int count = 0;
    for(auto i=0; i<this->paiCount - 1; i++)
    {
        if(this->paiList[i] == this->paiList[i+1])  // 相同的两张牌
        {
            this->cPengList[count++] = this->paiList[i];

            if(this->paiList[i] == this->paiList[i + 2])    //有可能三张相同的  甚至四张相同的
            {
                i += 2;
            }
            else if(this->paiList[i] == this->paiList[i + 3])
            {
                i += 3;
            }
            else
                i++;
        }
    }

    this->cPengCount = count;
    return count;
}

int MJ_Player::cGang()
{
    int count = 0;

    for(auto i=0; i<this->paiCount-2; i++)
    {
        if(this->paiList[i] == this->paiList[i+1] &&
                this->paiList[i] == this->paiList[i+2])
        {
            this->cGangList[count++] = this->paiList[i];
            if(this->paiList[i] == this->paiList[i+3])
                i += 3;
            else
                i += 2;
        }
    }

    this->cGangCount = count;
    return count;
}

int MJ_Player::analysis()
{
    if(!(this->paiCount != 1 || this->paiCount != 4 ||
            this->paiCount != 10 || this->paiCount != 13))
        return -1;

    AnalyResults.clear();
/**** 提前判断  特殊胡法 --  十三浪  巧七对   七风会
 *   牌数  必须等于 13张
*/
    if(this->paiCount == 13)
    {
        CARD list[16] = {0};
        MJ_AnalyResult analy;
        int ret = 0;
        analy.dan_count = this->paiCount;
        memcpy(analy.dan, this->paiList, 16);

        MJ_sort(analy.dan, analy.dan_count);
        ret = analy.H_QiFengHui(list);
        for(auto i=0; i<ret; i++)
        {
            AnalyResults[list[i]] = analy;
        }
        if(ret)
            return 0;

        analy.dan_count = this->paiCount;
        memcpy(analy.dan, this->paiList, 16);

        MJ_sort(analy.dan, analy.dan_count);
        ret = analy.H_ShiSanLang(list);
        for(auto i=0; i<ret; i++)
        {
            AnalyResults[list[i]] = analy;
        }
        if(ret)
            return 0;

        analy.dan_count = this->paiCount;
        memcpy(analy.dan, this->paiList, 16);

        MJ_sort(analy.dan, analy.dan_count);
        //巧七对 需要把 王提取出来
        for(auto i=0; i<this->paiCount; i++)
        {
            if(analy.dan[i] == this->wang)
            {
                analy.dan[i] = MJ_Player::NewCard;
                analy.w_count++;
            }
        }
        MJ_sort(analy.dan, analy.dan_count);
        analy.dan_count -= analy.w_count;
        ret = analy.H_QiaoQiDui(list);

        for(auto i=0; i<ret; i++)
        {
            AnalyResults[list[i]] = analy;
        }
        if(ret)
            return 0;
    }

    typedef struct _node{
        typedef enum{
            W_ST, W_SL, W_DZ, W_EL, W_KZ, W_DAN
        }WHO;

        typename MJ_Base::CARD pai[16];    //
        int count;  // 张数
        int pos;    // 续点位置
        WHO who;    // 压栈者
    }Node;

    MJ_AnalyResult analy;

    CARD pai[16];//循环中用到的牌
    int pCount;//循环中用到的牌长度

    Node node;
    Node::WHO whoFlg;

    std::stack<Node> STACK;

    //前期准备
    //AnalyResults.clear();
    memset(&node, 0, sizeof(node));
    memcpy(node.pai, this->paiList, this->paiCount);
    node.count = this->paiCount;
    node.pos = 0;
    node.who = Node::W_ST;
    analy._w = this->wang;
    analy.st_count=1;

    for(auto i=0; i<node.count; i++)
    {
        if(node.pai[i] == this->wang)   // 提取王的个数
        {
            analy.w_count++;
            node.pai[i] = MJ_noCard;
        }
        else if(node.pai[i] == MJ_ZHONG)    // 假王代替真王原形
        {
            node.pai[i] = this->wang;
        }
    }
    MJ_sort(node.pai, node.count);
    node.count -= analy.w_count;
    STACK.push(node);

    while(!STACK.empty())
    {
        Node n = STACK.top(); STACK.pop();
        memcpy(pai, n.pai, 16);
        pCount = n.count;
        whoFlg = n.who;
        auto i=n.pos;

        switch(whoFlg)      //
        {
        case Node::W_ST:
            analy.st_count--;
            analy.st[analy.st_count] = 0;
            break;
        case Node::W_SL:
            analy.sl_count--;
            analy.sl[analy.sl_count] = 0;
            break;
        case Node::W_DZ:
            analy.dz_count--;
            analy.dz[analy.dz_count] = 0;
            break;
        case Node::W_EL:
            analy.el_count--;
            analy.el[analy.el_count] = 0;
            break;
        case Node::W_KZ:
            analy.kz_count--;
            analy.kz[analy.kz_count] = 0;
            break;
        case Node::W_DAN:
            break;
        }

        /****************************************************
         *      选出三张相同的牌 组合
         *      每选一组，将原纪录压栈，记下原先迭代到的位置，下次沿着原纪录
         *   向后继续
         */
        for(; whoFlg==Node::W_ST && i<pCount-2; i++)
        {
            if(hasSanTong(pai + i))
            {
                Node tmp;
                memcpy(tmp.pai, pai, 16);
                tmp.count = pCount;
                tmp.pos = i+3;          // 改变 pos  即可
                tmp.who = Node::W_ST;
                STACK.push(tmp);

                analy.st[analy.st_count++] = pai[i];

                for(auto j=i; j<pCount; pai[j] = pai[j+3],j++);
                pCount -= 3;

                i = -1;
                continue;
            }
        }
        if(whoFlg == Node::W_ST && i >= pCount-2)//这里并不是表示pai中没有相同的三张牌的组合
                                        //  有时候很有可能三张同牌组合拆分成三连的组合 获得的胡法倍数更高
        {
            i = 0;
            whoFlg = Node::W_SL;    //
        }

        /****************************************************
         *   找出三连组合
         */
        for(; whoFlg==Node::W_SL && i<pCount-2; i++)
        {
            CARD *res[3] = {0};
            if(hasSanLian(pai, pai[i], res))
            {
                Node tmp;
                memcpy(tmp.pai, pai, 16);
                tmp.count = pCount;
                tmp.pos = i+3;          // 改变 pos  即可
                tmp.who = Node::W_SL;
                STACK.push(tmp);

                analy.sl[analy.sl_count++] = pai[i];

                *res[0] = MJ_noCard;
                *res[1] = MJ_noCard;
                *res[2] = MJ_noCard;
                this->MJ_sort(pai, pCount);
                pCount -= 3;

                i = -1;
                continue;
            }
        }
        if(whoFlg == Node::W_SL && i >= pCount-2)
        {
            i = 0;
            whoFlg = Node::W_DZ;
        }

        /***
         *      做个基本过滤：王 <= 1个 && 剩下的牌还大于5张
         *                 王 == 2个 && 剩下的牌还多于10张
         *                 王 == 3个 && 剩下的牌还多于10张
         *      这样的牌是不会胡的了！
         */
        if((analy.w_count<=1 && pCount>=5) ||
                (analy.w_count==2 && pCount>10) ||
                (analy.w_count==3 && pCount>10))
            continue;

        /*****
         *  找对子
         */
        for(; whoFlg==Node::W_DZ && i<pCount-1; i++)
        {
            if(hasDuiZi(pai+i))
            {
                Node tmp;
                memcpy(tmp.pai, pai, 16);
                tmp.count = pCount;
                tmp.pos = i+2;          // 改变 pos  即可
                tmp.who = Node::W_DZ;
                STACK.push(tmp);

                analy.dz[analy.dz_count++] = pai[i];

                for(auto j=i; j<pCount; pai[j] = pai[j+2],j++);
                pCount -= 2;

                i = -1;
                continue;
            }
        }
        if(whoFlg == Node::W_DZ && i >= pCount-1)
        {
            i = 0;
            whoFlg = Node::W_KZ;
        }

        /*****
         *   找坎张
         */
        for(; whoFlg==Node::W_KZ && i<pCount-1; i++)
        {
            CARD *res[2] = {NULL};
            if(hasKanZhang(pai, pai[i], res))
            {
                Node tmp;
                memcpy(tmp.pai, pai, 16);
                tmp.count = pCount;
                tmp.pos = i+2;          // 改变 pos  即可
                tmp.who = Node::W_KZ;
                STACK.push(tmp);

                analy.kz[analy.kz_count++] = pai[i] - 1;

                *res[0] = MJ_noCard;
                *res[1] = MJ_noCard;
                this->MJ_sort(pai, pCount);
                pCount -= 2;

                i = -1;
                continue;
            }
        }
        if(whoFlg == Node::W_KZ && i >= pCount-1)
        {
            i = 0;
            whoFlg = Node::W_EL;
        }

        /*****
         *
         */
        for(; whoFlg==Node::W_EL && i<pCount-1; i++)
        {
            CARD *res[2] = {NULL};
            if(hasErLian(pai, pai[i], res))
            {
                Node tmp;
                memcpy(tmp.pai, pai, 16);
                tmp.count = pCount;
                tmp.pos = i+2;          // 改变 pos  即可
                tmp.who = Node::W_EL;
                STACK.push(tmp);

                analy.el[analy.el_count++] = pai[i];

                *res[0] = MJ_noCard;
                *res[1] = MJ_noCard;
                this->MJ_sort(pai, pCount);
                pCount -= 2;

                i = -1;
                continue;
            }
        }

        /****
         *  剩下的就是单牌了
         */
        analy.dan_count = pCount;
        analy.dan_count && memcpy(analy.dan, pai, pCount);
        analy.dan_count || memset(analy.dan, 0, 8);

        CARD allTing[16] = {0};
        int tingCount = 0;
        if(tingCount = analy.canHu(allTing))
        {
            for(auto i=0; i<tingCount; i++)
            {
//                auto it = AnalyResults.find(allTing[i]);
                //if(it != AnalyResults.end())
                {
                    AnalyResults[allTing[i]] = analy;
#ifdef DEBUG
//      analy.printResult();                }
#endif
                }
            }
        }

    }

    return 0;
}

int MJ_Player::Chi(MJ_Base::CARD card, pCCARD ll)
{
    for(int i=0; i<3; i++)
    {
        if(card != ll[i])
            this->DelCard(ll[i]);

        this->chi[_c++] = ll[i];
    }

    this->MJ_sort(this->paiList, this->paiCount);

    return 0;
}

int MJ_Player::Peng(MJ_Base::CARD card)
{
    this->DelCard(card);
    this->DelCard(card);

    this->peng[_p++] = card;

    this->MJ_sort(this->paiList, this->paiCount);

    return 0;
}

int MJ_Player::Gang(MJ_Base::CARD card)
{
    this->DelCard(card);
    this->DelCard(card);
    this->DelCard(card);

    this->gang[_g++] = card;

    this->MJ_sort(this->paiList, this->paiCount);

    return 0;
}

int MJ_Player::Hu(CARD card, pCCARD ll)
{
    return 0;
}

int MJ_Player::getCChiList(MJ_Base::CARD card, MJ_Base::CARD (*res)[3])
{
    return 0;
}

bool MJ_Player::testHu(MJ_Base::CARD c)
{
    for(auto it : AnalyResults)
    {
        if(it.first == c)
        {
            return true;
        }
    }

    return false;
}

bool MJ_Player::testPeng(MJ_Base::CARD c)
{
    for(auto i=0; i<this->cPengCount; i++)
    {
        if(c == this->cPengList[i])
            return true;
    }

    return false;
}

bool MJ_Player::testGang(MJ_Base::CARD c)
{
    for(auto i=0; i<this->cGangCount; i++)
    {
        if(c == this->cGangList[i])
            return true;
    }

    return false;
}

bool MJ_Player::testChi(MJ_Base::CARD c)
{
    for(auto i=0; i<this->cChiCount; i++)
    {
        if(c == this->cChiList[i])
            return true;
    }

    return false;
}

int MJ_Player::copy_chiList(MJ_AnalyResult *to)
{
    to->c_count = this->_c;
    memcpy(to->chi, this->chi, this->_c * 3);
    return this->_c;
}

int MJ_Player::copy_pengList(MJ_AnalyResult *to)
{
    to->p_count = this->_p;
    memcpy(to->peng, this->peng, this->_p);
    return this->_p;
}

int MJ_Player::copy_gangList(MJ_AnalyResult *to)
{
    to->g_count = this->_g;
    memcpy(to->gang, this->gang, this->_g);
    return this->_g;
}

void MJ_Player::MJ_sort(typename MJ_Base::CARD *li, int len)
{
    for(int i=0; i<len-1; i++)
    {
        for (int j=0; j<len-1-i; j++)
        {
            if(li[j] < li[j+1])
            {
                CARD t = li[j];
                li[j] = li[j+1];
                li[j+1] = t;
            }
        }
    }
}

#ifdef DEBUG
void MJ_Player::printTest()
{
    cout << "**********************************************" << endl;
    cout << "* 手上的牌: " << this->paiList << "共 " << this->paiCount << " 张" << endl;
    cout << "* 可以吃：" << this->cChiList << endl;
    cout << "* 可以碰：" << this->cPengList << endl;
    cout << "* 可以杠：" << this->cGangList << endl;
    cout << "* 可以胡：" << this->HuList << endl;
    cout << "* 可以胡：";
    for(auto it : AnalyResults) {
        it.second.calc_BeiShu(*this, it.first, MJ_AnalyResult::F_JiePao);
        cout << it.first << ": " << it.second.getFan() << "  ";
        list<const char *> lst = it.second.HU_names();

        for(list<const char *>::iterator x = lst.begin(); x !=lst.end(); ++x)
        {
            cout << *x << "、";
        }
        cout << endl;
    }

    cout << "\n**********************************************" << endl;
}

void MJ_Player::printHu()
{

}
#endif
