#include "MJC_CardSet.h"
#include "MJ_AnalyResult.h"
#include <cstring>
#include <algorithm>
#include <stack>

#ifdef DEBUG

  #include <iostream>
  using namespace std;

#endif

bool MJC_CardSet::hasSanLian(MJC_CardSet::CARD *li, MJC_CardSet::CARD c, CARD **res)
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

bool MJC_CardSet::hasErLian(MJC_CardSet::CARD *li, MJC_CardSet::CARD c, CARD **res)
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

bool MJC_CardSet::hasKanZhang(MJC_CardSet::CARD *li, MJC_CardSet::CARD c, CARD **res)
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

MJC_CardSet::MJC_CardSet()
{

}

void MJC_CardSet::init(MJC_CardSet::pCCARD _paiList, MJC_CardSet::CARD _wang)
{
    /*
    CARD paiList[16];

    CARD HuList[16];
    CARD cGangList[8];
    CARD cPengList[8];
    CARD cChiList[16];

    CARD gang[8];
    CARD peng[8];
    CARD chi[8];
*/
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
//    std::sort(this->paiList, this->paiList + this->paiCount, this->MJ_cmp);

#ifdef DEBUG
    cout << "*初始化游戏- ： "  << this->paiList << "牌张数:" << this->paiCount << endl;
#endif
}

void MJC_CardSet::addCard(MJC_CardSet::CARD newCard)
{
    auto i = this->paiCount;
    for(; i >0; i--)
    {
        if(this->paiList[i-1] < newCard)
        {
           this->paiList[i] = this->paiList[i-1];
           continue;
        }
        break;
    }

    this->paiList[i] = newCard;
    this->paiCount++;
#ifdef DEBUG
    cout << "*增加牌:" << newCard << ", " << this->paiList << "  " << this->paiCount << endl;
#endif
}

int MJC_CardSet::delCard(MJC_CardSet::CARD card)
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

int MJC_CardSet::delCard(int offset)
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
int MJC_CardSet::cChi()
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

    return cLen;
}

int MJC_CardSet::cPeng()
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

    return count;
}

int MJC_CardSet::cGang()
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

    return count;
}

int MJC_CardSet::analysis()
{
    typedef struct _node{
        typedef enum{
            W_ST, W_SL, W_DZ, W_EL, W_KZ, W_DAN
        }WHO;

        typename MJC_CardSet::CARD pai[16];    //
        int count;  // 张数
        int pos;    // 续点位置
        WHO who;    // 压栈者
    }Node;

    MJ_AnalyResult analy;

    CARD pai[16];//循环中用到的牌
    int pCount;//循环中用到的牌长度

    Node node;
    std::stack<Node> STACK;

    Node::WHO whoFlg;

    AnalyResults.clear();
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
        memcpy(analy.dan, pai, pCount);

        CARD allTing[16] = {0};
        int tingCount = 0;
        if(tingCount = analy.canHu(allTing))
        {
            for(auto i=0; i<tingCount; i++)
            {
                auto it = AnalyResults.find(allTing[i]);
                //if(it != AnalyResults.end())
                {
                    AnalyResults[allTing[i]] = analy;
#ifdef DEBUG
      analy.printResult();
      for(auto&&it : AnalyResults){
              std::cout << it.first << endl;
                }
#endif
                }
            }
        }
    }

    return 0;
}

int MJC_CardSet::Chi(MJC_CardSet::CARD card, CARD c[])
{
    for(int i=0; i<3; i++)
    {
        if(card != c[i])
            this->delCard(c[i]);

        this->chi[_c++] = c[i];
    }

    return 0;
}

int MJC_CardSet::Peng(MJC_CardSet::CARD card)
{
    this->delCard(card);
    this->delCard(card);

    this->peng[_p++] = card;
    return 0;
}

int MJC_CardSet::Gang(MJC_CardSet::CARD card)
{
    this->delCard(card);
    this->delCard(card);
    this->delCard(card);

    this->gang[_g++] = card;
    return 0;
}

int MJC_CardSet::Hu(CARD card)
{
    return 0;
}

void MJC_CardSet::MJ_sort(typename MJC_CardSet::CARD *li, int len)
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
void MJC_CardSet::printTest()
{
    cout << "**********************************************" << endl;
    cout << "* 手上的牌: " << this->paiList << "共 " << this->paiCount << " 张" << endl;
    cout << "* 可以吃：" << this->cChiList << endl;
    cout << "* 可以碰：" << this->cPengList << endl;
    cout << "* 可以杠：" << this->cGangList << endl;
    cout << "* 可以胡：" << this->HuList << endl;
    cout << "**********************************************" << endl;
}
#endif
