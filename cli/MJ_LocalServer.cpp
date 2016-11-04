#include "MJ_LocalServer.h"

#include <QDebug>

static MJ_Base::CARD cardSet[] = {
    'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',//万
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',//条
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',//筒
    'A', 'B', 'C', 'D', 'E', 'F', 'G',          //风

    'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G',

    'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G',

    'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G'
};

//  init static instance
MJ_LocalServer *MJ_LocalServer::Instance = nullptr;

MJ_LocalServer::MJ_LocalServer(QObject *parent) : QObject(parent)
{
    for(auto i=0; i<4; i++)
        this->player[i] = new MJ_Player();

    winner_id = -1;

    stat = SVR_normal;//正常模式

    connect(this, SIGNAL(_postToRecvSlot(MJ_RequestData)), this, SLOT(RecvSlot(MJ_RequestData)), Qt::QueuedConnection),

    this->tmChuPai = new QTimer(this);
    connect(this->tmChuPai, SIGNAL(timeout()), this, SLOT(tmChuPaiSlot()), Qt::QueuedConnection);
    tmOut = new QTimer(this);
    connect(this->tmOut, SIGNAL(timeout()), this, SLOT(tmOutSlot()), Qt::QueuedConnection);
}

MJ_LocalServer *MJ_LocalServer::getInstance()
{
    if(Instance == nullptr)
    {
        Instance = new MJ_LocalServer();
    }

    return Instance;
}

MJ_LocalServer::~MJ_LocalServer()
{
    delete this->player[0];
    delete this->player[1];
    delete this->player[2];
    delete this->player[3];
}

void MJ_LocalServer::send(MJ_response &resp)
{
    emit responseSignal(resp);
}

//开局：选王，选庄，发牌（初始化的13张），
void MJ_LocalServer::start()
{
    MJ_Base::CARD cards[4][16] = {0};
    paiCount = 0;
    maxPaiCount = 136;
    gang_falg = false;
    f_HGPC_valid = false;
    wj_ready = 0;//玩家还没整理好手上的牌

    memset(this->paiList, 0, this->maxPaiCount);

    if(winner_id < 0)  // 第一局,
        this->zhuang_id = 0;
    else
        this->zhuang_id  = winner_id;  // 胜出者为庄
        //this->zhuang_id = rand() % 4; // 选庄

    cur_id = zhuang_id;//当前出牌者
    stat = SVR_normal;//正常模式

    //洗牌
    srand(time(NULL));
    int cardCount[136] = {0};
    for(auto i=0; i<136; i++)
    {
        int r = rand() % 136;
        while(cardCount[r] != 0) r++, r %= 136;

        paiList[i] = cardSet[r];
        cardCount[r] = 1;//标记已选
    }
    //选王
    shaizi1 = rand()%6+1;
    shaizi2 = rand()%6+1;

    maxPaiCount -= (shaizi1 + shaizi2) * 2;
    this->wang = paiList[maxPaiCount - 1];
    switch(this->wang)
    {
    case MJ_Base::MJ_JIUWAN:
        this->wang = MJ_Base::MJ_YIWAN;
        break;
    case MJ_Base::MJ_JIUTONG:
        this->wang = MJ_Base::MJ_YITONG;
        break;
    case MJ_Base::MJ_JIUTIAO:
        this->wang = MJ_Base::MJ_YITIAO;
        break;
    case MJ_Base::MJ_BAN:
        this->wang = MJ_Base::MJ_DONG;
        break;
    default:
        this->wang -= 1;//九万的ascii值比一万小
    }

    //发牌
    for(int i=0; i<13; i++)
    {
        cards[0][i] = this->paiList[paiCount++];
        cards[1][i] = this->paiList[paiCount++];
        cards[2][i] = this->paiList[paiCount++];
        cards[3][i] = this->paiList[paiCount++];
    }

    this->player[0]->init(cards[0], this->wang);
    this->player[1]->init(cards[1], this->wang);
    this->player[2]->init(cards[2], this->wang);
    this->player[3]->init(cards[3], this->wang);

    qDebug() << "~~~~!" << shaizi1 << shaizi2 << maxPaiCount << paiCount;
/******
 *
    qDebug() << "svr:  zhuangid " << this->zhuang_id << "  curid" << this->cur_id;
    qDebug() << "svr:  maxpaiCount:" << this->maxPaiCount << "  paicount" << this->paiCount;
    qDebug() << "svr:  begin:" << (char*)cards[0] << this->wang;
    qDebug() << "svr:  begin:" << (char*)cards[1] << this->wang;
    qDebug() << "svr:  begin:" << (char*)cards[2] << this->wang;
    qDebug() << "svr:  begin:" << (char*)cards[3] << this->wang;
    qDebug() << "svr: all pai:" << this->paiList << endl;
 *
*******/
    // 发送开局消息
    MJ_response resp;
    resp.setCard(this->wang);
    resp.setPaiList(cards[0]);
    resp.setSendTo(0);
    resp.setType(MJ_response::T_Init);
    resp.setPaiCount(this->maxPaiCount - paiCount);
    this->send(resp);

    resp.setCard(this->wang);
    resp.setPaiList(cards[1]);
    resp.setSendTo(1);
    resp.setType(MJ_response::T_Init);
    this->send(resp);

    resp.setCard(this->wang);
    resp.setPaiList(cards[2]);
    resp.setSendTo(2);
    resp.setType(MJ_response::T_Init);
    this->send(resp);

    resp.setCard(this->wang);
    resp.setPaiList(cards[3]);
    resp.setSendTo(3);
    resp.setType(MJ_response::T_Init);
    this->send(resp);
}

void MJ_LocalServer::faPai_NoCard()
{
    MJ_response resp;
    if(paiCount == maxPaiCount)
    {
        resp.setType(MJ_response::T_GMOver);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        send(resp);
        this->tmChuPai->stop();
        return ;
    }

    qDebug() << ":faPai_NoCard(): ID = " << current_policy_ID << "current_policy:" << current_policy;

    this->cur_id = this->current_policy_ID;
    this->tmOutCard   = this->player[cur_id]->getLastCard();

    qDebug() << QString::fromLocal8Bit("超时就出：") << this->tmOutCard;

    resp.setSendTo(cur_id);
    resp.setWho(cur_id);
    resp.setType(MJ_response::T_FaPai);
    resp.setCard(MJ_Base::MJ_noCard);

    send(resp);

    this->tmChuPai->start(8000);//出牌8秒

    qDebug() << "svr  ::faPai(): id = " << cur_id;
}

void MJ_LocalServer::faPai()
{
    MJ_response resp;
    if(paiCount == maxPaiCount)
    {
        resp.setType(MJ_response::T_GMOver);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        send(resp);
        this->tmChuPai->stop();
        return ;
    }

    this->tmOutCard = this->paiList[paiCount++];
    this->card = tmOutCard;

    for(int i=0; i<4; i++)
        this->mem_policy[i] = P_None;

    if(player[cur_id]->testHu(card))
        mem_policy[cur_id] |= P_Hu;
    if(player[cur_id]->testGang(card))
        mem_policy[cur_id] |= P_Gang;
    if(player[cur_id]->testAnGang() != MJ_Base::MJ_noCard)
        mem_policy[cur_id] |= P_Gang;
    if(player[cur_id]->testBuGang(card))
        mem_policy[cur_id] |= P_Gang;
    if(player[cur_id]->testZiMo(card))
        mem_policy[cur_id] |= P_Hu;

    if(mem_policy[cur_id] != P_None)
    {
        qDebug() << "\tmem_policy[cur_id]:" << mem_policy[cur_id];
        this->f_HGPC_valid = true;
        current_policy = P_None;
    }

    resp.setSendTo(cur_id);
    resp.setWho(cur_id);
    resp.setType(MJ_response::T_FaPai);
    resp.setCard(this->card);
    resp.setPaiCount(this->maxPaiCount - this->paiCount);
    send(resp);

    this->player[cur_id]->addCard(this->card);

    this->tmChuPai->start(8000);//出牌8秒

    MJ_Base::CARD lst[16] = {0};
    qDebug() << "svr  ::faPai(): id = " << cur_id << QString::fromLocal8Bit("发牌后张数：") << this->player[cur_id]->getPaiList(lst);
    qDebug() << "svr  ::faPai(): pailist:" << (char *)lst << endl;
}

void MJ_LocalServer::resl_chuPai(MJ_RequestData &req)
{
    int senderID = req.getSenderID();
    if(senderID >= 4 || senderID < 0)
        return ;

    MJ_Base::CARD cd = req.getCard();
    this->player[senderID]->DelCard(cd);//默认出牌
    //回应ok
    MJ_response resp;
    resp.setType(MJ_response::T_Ok);
    resp.setSendTo(senderID);
    send(resp);    

    //通知所有玩家，某玩家出了一张牌
    resp.setType(MJ_response::T_ChuPai);
    resp.setCard(cd);
    resp.setWho(senderID);
    resp.setSendTo(MJ_response::SDT_Broadcast);
    send(resp);
    qDebug() << "svr ::resl_chuPai SDT_Broadcast ok";

    cur_id = senderID;
    mem_policy[senderID] = P_None;

    int j = senderID + 1;
    j %= 4;//定位到下家
    mem_policy[j] = P_None;
    if(player[j]->testHu(cd))
        mem_policy[j] |= P_Hu;
    if(player[j]->testGang(cd))
        mem_policy[j] |= P_Gang;
    if(player[j]->testPeng(cd))
        mem_policy[j] |= P_Peng;
    if(player[j]->testChi(cd))
        mem_policy[j] |= P_Chi;
    if(mem_policy[j] != P_None) // 有选项
    {
        this->stat = SVR_vote;//等待抉择
    }
    qDebug() << "\tmem_policy[j]:" << mem_policy[j] << "j = " << j;

    j += 1; j %= 4;
    mem_policy[j] = P_None;
    if(player[j]->testHu(cd))
        mem_policy[j] |= P_Hu;
    if(player[j]->testGang(cd))
        mem_policy[j] |= P_Gang;
    if(player[j]->testPeng(cd))
        mem_policy[j] |= P_Peng;
    if(mem_policy[j] != P_None) // 有选项
    {
        this->stat = SVR_vote;//等待抉择
    }
    qDebug() << "\tmem_policy[j]:" << mem_policy[j] << "j = " << j;

    j += 1; j %= 4;
    mem_policy[j] = P_None;
    if(player[j]->testHu(cd))
        mem_policy[j] |= P_Hu;
    if(player[j]->testGang(cd))
        mem_policy[j] |= P_Gang;
    if(player[j]->testPeng(cd))
        mem_policy[j] |= P_Peng;
    if(mem_policy[j] != P_None) // 有选项
    {
        this->stat = SVR_vote;//等待抉择
    }
    qDebug() << "\tmem_policy[j]:" << mem_policy[j] << "j = " << j;

    if(this->stat == SVR_vote) // 本桌有玩家可以 胡杠碰吃
    {
//        qDebug() << "\tmem_policy[0]:" << mem_policy[0];
//        qDebug() << "\tmem_policy[1]:" << mem_policy[1];
//        qDebug() << "\tmem_policy[2]:" << mem_policy[2];
//        qDebug() << "\tmem_policy[3]:" << mem_policy[3];
        qDebug() << "svr  resl_FaPai():need wait! current_policy=" << this->current_policy;
        resp.setType(MJ_response::T_Wait);
        resp.setWho(senderID);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        send(resp);

        this->tmChuPai->stop();
        this->tmOut->start(6000);//6s选择时间
        f_HGPC_valid = true;
        current_policy = P_None;
    }
    else if(this->stat == SVR_normal) // 继续发牌
    {
        qDebug() << "svr  resl_ChuPai() continue FaPai";
        cur_id ++;
        cur_id %= 4;
        gang_falg = false;//此标志用于判断是否抢杠胡
        this->_FaPaiRequest();
    }
    else   // 出错了！
    {
        // do print err log
    }
}

// 更新 玩家 胡杠碰吃 列表
void MJ_LocalServer::resl_HGPCList(MJ_RequestData &request)
{
    int senderID = request.getSenderID();
    if(senderID >= 4 || senderID < 0)
        return ;

    if((wj_ready & 0x0f) != 0x0f)
    {
        wj_ready |= 1 << senderID;
        if((wj_ready & 0x0f) == 0x0f)//这时候到齐了，开始发牌（这算是开局发牌，
                            //   后面还会收到很多的HGPCList消息, 开局发牌只有一次 ）
        {
            qDebug() << " ### resl_HGPCList  to FaPai() ";
            this->_FaPaiRequest();
        }
    }

    MJ_Base::CARD hu[8] = {0};
    MJ_Base::CARD g[8] = {0};
    MJ_Base::CARD p[8] = {0};
    MJ_Base::CARD c[8] = {0};

    request.getHGPCList(hu, g, p, c);

    player[senderID]->setcChiList(c);
    player[senderID]->setcPengList(p);
    player[senderID]->setcGangList(g);
    player[senderID]->setcHuList(hu);

    qDebug() << "svr   recv HGPCList:" << hu << " " << g << " " << p << " " << c;
    qDebug() << "svr  wj_ready:" << wj_ready<< endl;

}

// 胡牌消息：返回胡牌玩家
//            胡的牌，手上的牌，杠碰吃(未填坑)
void MJ_LocalServer::resl_Hu(MJ_RequestData &req)
{
    int senderID = req.getSenderID();

    if(!f_HGPC_valid)
    {
        qDebug() << "\t!f_HGPC_valid";
        resp_unsuccessful(senderID, senderID);
        return;
    }

    qDebug() << "\tsenderId:" << senderID;
    qDebug() << "\tmem_policy[0]:" << mem_policy[0];
    qDebug() << "\tmem_policy[1]:" << mem_policy[1];
    qDebug() << "\tmem_policy[2]:" << mem_policy[2];
    qDebug() << "\tmem_policy[3]:" << mem_policy[3];

    if(this->mem_policy[senderID] & P_Hu)
    {
        this->stat = SVR_normal;
        if(this->tmOut->isActive())
            this->tmOut->stop();

        MJ_response resp;
        resp.setType(MJ_response::T_Ok);
        resp.setWho(senderID);
        resp.setSendTo(senderID);
        send(resp);

        // 胡牌消息体
        resp.setType(MJ_response::T_Hu);
        resp.setCard(req.getCard());
        resp.setWho(senderID);

        MJ_Base::CARD g[8] = {0}, p[8] = {0}, c[16] = {0}, pai[16] = {0};
        this->player[senderID]->getGang(g);
        this->player[senderID]->getPeng(p);
        this->player[senderID]->getChi(c);
        this->player[senderID]->getPaiList(pai);

        resp.setgpc(g, p, c);
        resp.setPaiList(pai);
        resp.setSendTo(MJ_response::SDT_Broadcast);

        if(gang_falg && cur_id == senderID)  // 杠上花
        {
            resp.setHuType(MJ_response::HU_GangShangHua);
        }
        else if(gang_falg && cur_id != senderID)  //抢杠胡
        {
            resp.setHuType(MJ_response::HU_QiangGang);
        }
        else if(cur_id == senderID)//自摸
        {
            resp.setHuType(MJ_response::HU_ZiMo);
        }
        else
            resp.setHuType(MJ_response::HU_JiePaio);

        send(resp);

        //本局结束
        resp.setType(MJ_response::T_GMOver);
        resp.setWho(senderID);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        send(resp);
        this->tmChuPai->stop();
    }
    else
    {
        qDebug() << "\t!mem_policy ERR";
        resp_unsuccessful(senderID, senderID);
    }
}

void MJ_LocalServer::resl_Gang(MJ_RequestData &req)
{
     int senderID = req.getSenderID();
     if(this->cur_id == senderID) // 当前出牌玩家杠牌，不是暗杠，那就是转杠
     {
         this->resl_BuGang(req);
         return;
     }

     if(!f_HGPC_valid)
     {
         resp_unsuccessful(senderID, senderID);
         return;
     }

     if(current_policy > P_Gang)
     {
         resp_unsuccessful(senderID, senderID);
         return ;
     }

     if(mem_policy[senderID] & P_Gang)
     {
         MJ_Base::CARD cd = req.getCard();
         current_policy = P_Gang;
         current_policy_ID = senderID;
         mem_policy[current_policy_ID] = P_None;//取消掉状态

         MJ_response resp;
         resp.setType(MJ_response::T_Ok);
         resp.setWho(senderID);
         resp.setSendTo(senderID);
         send(resp);

         resp.setType(MJ_response::T_Gang);
         resp.setCard(cd);
         resp.setSendTo(MJ_response::SDT_Broadcast);
         resp.setWho(senderID);
         send(resp);

         // 进入if分支的即为最终执行成功的，杠->补牌
         if(mem_policy[0] <= P_Gang && mem_policy[1] <= P_Gang &&
                 mem_policy[2] <= P_Gang && mem_policy[3] <= P_Gang)
         {
             this->stat = SVR_normal;
             if(this->tmOut->isActive())
                 this->tmOut->stop();

             // 补牌
             cur_id = current_policy_ID;
             this->player[cur_id]->Gang(cd);//
             gang_falg = true;//此标志用于判断是否抢杠胡
             this->_FaPaiRequest();
         }
     }
     else
         resp_unsuccessful(senderID, senderID);

     return ;
}

void MJ_LocalServer::resl_Peng(MJ_RequestData &req)
{
    int senderID = req.getSenderID();

    if(!f_HGPC_valid)
    {
        resp_unsuccessful(senderID, senderID);
        return;
    }

    if(current_policy > P_Peng)
    {
        resp_unsuccessful(senderID, senderID);
        return ;
    }

    if(mem_policy[senderID] & P_Peng)
    {
        MJ_Base::CARD cd = req.getCard();
        current_policy = P_Peng;
        current_policy_ID = senderID;
        mem_policy[current_policy_ID] = P_None;//取消掉状态

        MJ_response resp;
        resp.setType(MJ_response::T_Ok);
        resp.setWho(senderID);
        resp.setSendTo(senderID);
        send(resp);

        resp.setType(MJ_response::T_Peng);
        resp.setCard(cd);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        resp.setWho(senderID);
        send(resp);

        if(mem_policy[0] <= P_Peng && mem_policy[1] <= P_Peng &&
                mem_policy[2] <= P_Peng && mem_policy[3] <= P_Peng)
        {
            this->stat = SVR_normal;
            if(this->tmOut->isActive())
                this->tmOut->stop();
            this->player[current_policy_ID]->Peng(cd);
            gang_falg = false;//此标志用于判断是否抢杠胡

            this->faPai_NoCard();
        }
    }
    else
    {
        resp_unsuccessful(senderID, senderID);
    }
    return;
}

void MJ_LocalServer::resl_Chi(MJ_RequestData &req)
{
    int senderID = req.getSenderID();
    MJ_Base::CARD cd = req.getCard();


    if(!f_HGPC_valid)
    {
        resp_unsuccessful(senderID, senderID);
        return;
    }

    if(current_policy > P_Chi)
    {
        resp_unsuccessful(senderID, senderID);
        return;
    }

    // 只能吃上家的牌
    if(current_policy == P_None && (cur_id+1)%4 == senderID)
    {
        req.getChi(this->chi);

        current_policy_ID = senderID;
        current_policy = P_Chi;
        mem_policy[current_policy_ID] = P_None;//取消掉状态

        //广播这个玩家的吃操作
        MJ_response resp;
        resp.setType(MJ_response::T_Ok);
        resp.setWho(senderID);
        resp.setSendTo(senderID);
        send(resp);

        resp.setType(MJ_response::T_Chi);
        resp.setCard(req.getCard());
        resp.setWho(senderID);
        resp.setChi(this->chi);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        send(resp);

        // 如果没有比吃更高优先级的操作存在，则叫玩家出牌
        if(mem_policy[0] <= P_Chi && mem_policy[1] <= P_Chi &&
                mem_policy[2] <= P_Chi && mem_policy[3] <= P_Chi)
        {
            this->stat = SVR_normal;
            if(this->tmOut->isActive())
                this->tmOut->stop();

            this->player[current_policy_ID]->Chi(cd, this->chi);
            gang_falg = false;//此标志用于判断是否抢杠胡

            this->faPai_NoCard();
        }
    }
    else // 回应不成功
    {
        resp_unsuccessful(senderID, senderID);
    }
    return ;
}

void MJ_LocalServer::resl_Cancel(MJ_RequestData &req)
{
    int senderID = req.getSenderID();
    MJ_Base::CARD cd = req.getCard();

    if(senderID == this->cur_id)    // 如果是玩家不想暗杠，自摸而发来的则忽略
    {
        return;
    }

    this->mem_policy[senderID] = P_None;//取消这个操作

    qDebug() << "\tcurrent_policy:" << current_policy;
    qDebug() << "\tmem_policy[0]:" << mem_policy[0];
    qDebug() << "\tmem_policy[1]:" << mem_policy[0];
    qDebug() << "\tmem_policy[2]:" << mem_policy[0];
    qDebug() << "\tmem_policy[3]:" << mem_policy[0];
    // 当这个玩家操作取消后，之前有玩家选择的操作是当前最大值时
    if(mem_policy[0] <= current_policy && mem_policy[1] <= current_policy &&
            mem_policy[2] <= current_policy && mem_policy[3] <= current_policy)
    {
        //  没有操作，能执行这个分支的话，可以发牌给下家了
        if(current_policy == P_None)
        {
            this->stat = SVR_normal;
            if(this->tmOut->isActive())
                this->tmOut->stop();

            cur_id++;
            cur_id %= 4;
            gang_falg = false;//此标志用于判断是否抢杠胡
            this->_FaPaiRequest();

            return;
        }

        this->stat = SVR_normal;
        if(this->tmOut->isActive())
            this->tmOut->stop();

        if(current_policy == P_Gang)  // 杠是个特殊操作
        {
            // 补牌
            cur_id = current_policy_ID;
            this->player[current_policy_ID]->Gang(cd);
            gang_falg = true;//此标志用于判断是否抢杠胡
            this->_FaPaiRequest();
        }
        else if(current_policy == P_Peng)
        {
            cur_id = current_policy_ID;
            this->player[current_policy_ID]->Peng(cd);
            gang_falg = false;//此标志用于判断是否抢杠胡
            this->faPai_NoCard();
        }
        else if(current_policy == P_Chi)
        {
            cur_id = current_policy_ID;
            this->player[current_policy_ID]->Chi(cd, this->chi);//this->chi  是之前玩家发送吃请求
                                                //  时保存下来的
            gang_falg = false;//此标志用于判断是否抢杠胡
            this->faPai_NoCard();
        }
    }
    //  否则继续等待
}

void MJ_LocalServer::resl_AnGang(MJ_RequestData &req)
{
    int senderID = req.getSenderID();

    MJ_response resp;
    resp.setType(MJ_response::T_Ok);
    resp.setWho(senderID);
    resp.setSendTo(senderID);
    send(resp);

    resp.setType(MJ_response::T_AnGang);
    resp.setSendTo(MJ_response::SDT_Broadcast);
    resp.setWho(senderID);
    send(resp);

    this->player[senderID]->Gang(req.getCard());
    this->player[senderID]->DelCard(req.getCard());

    cur_id = senderID;
    gang_falg = true;//此标志用于判断是否抢杠胡
    this->_FaPaiRequest();
}

void MJ_LocalServer::resl_BuGang(MJ_RequestData &req)
{
    int senderID = req.getSenderID();
    MJ_Base::CARD cd = req.getCard();

    MJ_response resp;
    resp.setType(MJ_response::T_Ok);
    resp.setWho(senderID);
    resp.setSendTo(senderID);
    send(resp);

    resp.setType(MJ_response::T_BuGang);
    resp.setSendTo(MJ_response::SDT_Broadcast);
    resp.setWho(senderID);
    send(resp);

    cur_id = senderID;
    int j = senderID;
    this->mem_policy[j] = P_None;//
    j++;
    j%=4;
    for(auto i=0; i<3; i++)
    {
        if(this->player[j]->testHu(cd))
        {
            this->mem_policy[j] = P_Hu;
            this->stat = SVR_vote;
        }
        else {
            this->mem_policy[j] = P_None;//
        }
        j++;
        j%=4;//定位到下个玩家
    }

    if(this->stat == SVR_vote)
    {
        current_policy = P_None;
        resp.setType(MJ_response::T_Wait);
        resp.setWho(senderID);
        resp.setSendTo(MJ_response::SDT_Broadcast);
        send(resp);

        this->tmOut->start(6000);//6s选择时间
        current_policy = P_Gang;
        current_policy_ID = senderID;
        f_HGPC_valid = true;
        gang_falg = true;//此标志用于判断是否抢杠胡
    }
    else
    {
        this->player[senderID]->Gang(cd);
        cur_id = senderID;
        this->_FaPaiRequest();

//        resp.setType(MJ_response::T_ChuPai);
//        resp.setSendTo(MJ_response::SDT_Broadcast);
//        resp.setWho(senderID);
//        send(resp);
    }
}

void MJ_LocalServer::_FaPaiRequest()
{
    MJ_RequestData req(cur_id);
    req.setType(MJ_RequestData::R_FaPai);

    emit _postToRecvSlot(req);
}

void MJ_LocalServer::RecvSlot(MJ_RequestData request)
{
    int type = request.getType();
    int senderID = request.getSenderID();

    qDebug() << "svr RecvSlot senderID" << senderID;
    switch(type)
    {
    case MJ_RequestData::R_Init:
        qDebug() << "svr RecvSlot type" << "R_Init";
        this->start();
        qDebug() << "svr RecvSlot R_Init" << "end";
        break;

    case MJ_RequestData::R_HGPCList:
        qDebug() << "svr RecvSlot type" << "R_HGPCList";
        this->resl_HGPCList(request);
        qDebug() << "svr RecvSlot R_HGPCList" << "end";
        break;

    case MJ_RequestData::R_ChuPai:
        qDebug() << "svr RecvSlot type" << "R_ChuPai";
        this->resl_chuPai(request);
        qDebug() << "svr RecvSlot R_ChuPai" << "end";
        break;

    case MJ_RequestData::R_Hu:
        qDebug() << "svr RecvSlot type" << "R_Hu";
        this->resl_Hu(request);
        qDebug() << "svr RecvSlot R_Hu" << "end";
        break;

    case MJ_RequestData::R_Gang:
        qDebug() << "svr RecvSlot type" << "R_Gang";
        this->resl_Gang(request);
        qDebug() << "svr RecvSlot R_Gang" << "end";
        break;

    case MJ_RequestData::R_Peng:
        qDebug() << "svr RecvSlot type" << "R_Peng";
        this->resl_Peng(request);
        qDebug() << "svr RecvSlot R_Peng" << "end";
        break;

    case MJ_RequestData::R_Chi:
        qDebug() << "svr RecvSlot type" << "R_Chi";
        this->resl_Chi(request);
        qDebug() << "svr RecvSlot R_Chi" << "end";
        break;

    case MJ_RequestData::R_CanCel:
        qDebug() << "svr RecvSlot type" << "R_CanCel";
        this->resl_Cancel(request);
        qDebug() << "svr RecvSlot R_CanCel" << "end";
        break;

    case MJ_RequestData::R_AnGang:
        qDebug() << "svr RecvSlot type" << "R_AnGang";
        this->resl_AnGang(request);
        qDebug() << "svr RecvSlot R_AnGang" << "end";
        break;

    case MJ_RequestData::R_BuGang:
        qDebug() << "svr RecvSlot type" << "R_BuGang";
        this->resl_BuGang(request);
        qDebug() << "svr RecvSlot R_BuGang" << "end";
        break;

    case MJ_RequestData::R_FaPai:
        qDebug() << "svr RecvSlot type" << "R_FaPai";
        this->faPai();
        qDebug() << "svr RecvSlot R_FaPai" << "end";
        break;

    default: // 出错了！
        qDebug() << "svr RecvSlot type" << "default" << endl;
        break;
    }
}

// 胡杠碰吃 选择超时
void MJ_LocalServer::tmOutSlot()
{
    f_HGPC_valid = false;
    this->stat = SVR_normal;//恢复正常模式
    this->tmOut->stop();

    // 有玩家做出了选择的要处理掉
    if(this->current_policy != P_None)
    {
        this->current_policy = P_None;

        this->faPai_NoCard();
//        MJ_response resp;
//        resp.setType(MJ_response::T_ChuPai);
//        resp.setSendTo(MJ_response::SDT_Broadcast);
//        resp.setWho(this->current_policy_ID);
//        send(resp);
    }
    else {  // 都没做出选择, 发牌给下家
        this->cur_id++;
        this->cur_id %= 4;
        this->_FaPaiRequest();
    }

    qDebug() << "svr  tmOutSlot(): current_policy" << current_policy;
}

void MJ_LocalServer::tmChuPaiSlot()
{
    qDebug() << "svr MJ_LocalServer::tmChuPaiSlot:";
    qDebug() << "   cur_id:" << cur_id;
    qDebug() << "   card:" << this->tmOutCard;

    MJ_RequestData req(cur_id);
    req.setType(MJ_RequestData::R_ChuPai);
    req.setSenderID(cur_id);
    req.setCard(this->tmOutCard);

    this->resl_chuPai(req);
}






