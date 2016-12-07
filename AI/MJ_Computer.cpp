#include "MJ_Computer.h"
#include "../pub/MJ_RequestData.h"

#include <QDebug>

MJ_Computer::MJ_Computer(QObject *parent) : QObject(parent)
{
    this->player = new MJ_Player();
    this->request = new MJ_RequestLocal();

    tm = new QTimer(this);
    connect(tm, SIGNAL(timeout()), this, SLOT(tmSlot()), Qt::QueuedConnection);
}

MJ_Computer::~MJ_Computer()
{

}

void MJ_Computer::init()
{
    this->player->init("", MJ_Base::MJ_noCard);
}

void MJ_Computer::setID(int _id)
{
    this->Id = _id;
}

void MJ_Computer::resl_init(MJ_response &resp)
{
    int recvID = resp.getSendTo();
    if(recvID == this->Id)
    {
        MJ_Base::CARD lst[16] = {0};
        resp.getPaiList(lst);

        MJ_Base::CARD wang;
        wang = resp.getCard();

        this->player->init(lst, resp.getCard()); // 初始化数据
        MJ_Base::CARD h[16] = {0};
        MJ_Base::CARD g[16] = {0};
        MJ_Base::CARD p[16] = {0};
        MJ_Base::CARD c[16] = {0};

        this->player->AnalysisHGPC();
        this->player->getCanHuList(h, 16);
        this->player->getCanGangList(g, 16);
        this->player->getCanPengList(p, 16);
        this->player->getCanChiList(c, 16);

        MJ_RequestData req(this->Id);
        req.setType(MJ_RequestData::R_HGPCList);
        req.setHGPCList(h, g, p, c);
        this->request->req_send(req);

        //qDebug() << "Computer::resl_init " << recvID << "  " << this->Id;
    }

}

void MJ_Computer::resl_wait(MJ_response &resp)
{
    int who = resp.getWho();
    MJ_Base::CARD card = resp.getCard();

    if(who == this->Id)
        return ;

    MJ_RequestData req(this->Id);
    if(this->player->testHu(card))
    {
        req.setType(MJ_RequestData::R_Hu);
    }
    else if(this->player->testGang(card))
    {
        req.setType(MJ_RequestData::R_Gang);
    }
    else if(this->player->testPeng(card))
    {
        req.setType(MJ_RequestData::R_Peng);
    }
    else if((who+1)%4 == this->Id && this->player->testChi(card))
    {
        req.setType(MJ_RequestData::R_Chi);
        MJ_Base::CARD chilist[8][4] = {0};
        int ret = this->player->getCChiList(card, chilist);
        req.setChi(chilist[0]);

        qDebug() << "Computer::" << ret << " " << chilist[0];
    }
    else
    {
        req.setType(MJ_RequestData::R_CanCel);
    }

    req.setCard(resp.getCard());
    this->request->req_send(req);
}

void MJ_Computer::resl_Chi(MJ_response &resp)
{
    int who = resp.getWho();

    MJ_Base::CARD _chi[4] = {0};
    resp.getChi(_chi);

    if(who == this->Id)
    {
        this->player->Chi(resp.getCard(), _chi);

        MJ_Base::CARD lst[16] = {0};
        int count  = this->player->getPaiList(lst);
        qDebug() << "_Computer::" << count << QString::fromLocal8Bit("剩余牌数:") << lst;
    }

}

void MJ_Computer::resl_Peng(MJ_response &resp)
{
    int who = resp.getWho();

    if(who == this->Id)
    {
        this->player->Peng(resp.getCard());

        MJ_Base::CARD lst[16] = {0};
        int count  = this->player->getPaiList(lst);
        qDebug() << "_Computer::" << count << QString::fromLocal8Bit("剩余牌数:") << lst;
    }

}

void MJ_Computer::resl_Gang(MJ_response &resp)
{
    int who = resp.getWho();

    if(who == this->Id)
    {
        this->player->Gang(resp.getCard());

        MJ_Base::CARD lst[16] = {0};
        int count  = this->player->getPaiList(lst);
        qDebug() << "_Computer::" << count << QString::fromLocal8Bit("剩余牌数:") << lst;
    }
}

void MJ_Computer::resl_Hu(MJ_response &resp)
{

}

void MJ_Computer::resl_FaPai(MJ_response &resp)
{
    if(resp.getSendTo() == this->Id)
    {
        this->card = resp.getCard();
        if(this->card == MJ_Base::MJ_noCard)
            this->card = this->player->getLastCard();

        tm->start(2000);
    }
    return ;


    qDebug() << "Computer::resl_FaPai card=" << resp.getCard() << "id = " << this->Id << " who:"
             << resp.getWho() << "recvID = " << resp.getSendTo();
    int recvID = resp.getSendTo();
    if(recvID == this->Id)
    {
        this->card = resp.getCard();
    //    this->player->addCard(this->card); // 添加

        // 出牌
        MJ_RequestData req(this->Id);
        req.setType(MJ_RequestData::R_ChuPai);

        if(card == MJ_Base::MJ_noCard)
        {
            qDebug() << "\t this card:" << this->player->getLastCard();
            req.setCard(this->player->getLastCard());
        }
        else
        {
            qDebug() << "\t this card:" << card;
            req.setCard(this->card);
        }

        req.setSenderID(this->Id);

        request->req_send(req);
    }
}

void MJ_Computer::resl_ChuPai(MJ_response &resp)
{
    int who = resp.getWho();
    qDebug() << "_Computer:chupai:" << who;
    if(who == this->Id)
    {
        this->player->ChuPai(resp.getCard());

        MJ_Base::CARD lst[16] = {0};
        int count = this->player->getPaiList(lst);
        qDebug() << "_Computer:chupai:" << count << ":" << lst;

        MJ_Base::CARD h[16] = {0};
        MJ_Base::CARD g[16] = {0};
        MJ_Base::CARD p[16] = {0};
        MJ_Base::CARD c[16] = {0};

        this->player->AnalysisHGPC();
        this->player->getCanHuList(h, 16);
        this->player->getCanGangList(g, 16);
        this->player->getCanPengList(p, 16);
        this->player->getCanChiList(c, 16);

        MJ_RequestData req(this->Id);
        req.setType(MJ_RequestData::R_HGPCList);
        req.setHGPCList(h, g, p, c);
        this->request->req_send(req);
    }
}

void MJ_Computer::resl_OK(MJ_response &resp)
{

}

void MJ_Computer::resl_Unsucc(MJ_response &resp)
{

}

void MJ_Computer::resl_Over(MJ_response &resp)
{

}

void MJ_Computer::responseSlot(MJ_response response)
{
    int type = response.getType();
    //qDebug() << "Computer::responseSlot: Send to" << response.getSendTo();
    //qDebug() << "Computer::responseSlot: who" << response.getWho();


    switch(type)
    {
    case MJ_response::T_Init:
        //qDebug() << "Computer::responseSlot: type:" << "T_Init";
        this->resl_init(response);
        break;
    case MJ_response::T_Wait:
        //qDebug() << "Computer::responseSlot: type:" << "T_Wait";
        this->resl_wait(response);
        break;
    case MJ_response::T_Chi:
        ////qDebug() << "Computer::responseSlot: type:" << "T_Chi";
        this->resl_Chi(response);
        break;
    case MJ_response::T_Peng:
        //qDebug() << "Computer::responseSlot: type:" << "T_Peng";
        this->resl_Peng(response);
        break;
    case MJ_response::T_Gang:
        //qDebug() << "Computer::responseSlot: type:" << "T_Gang";
        this->resl_Gang(response);
        break;
    case MJ_response::T_Hu:
        //qDebug() << "Computer::responseSlot: type:" << "T_Hu";
        this->resl_Hu(response);
        break;
    case MJ_response::T_FaPai:
        //qDebug() << "Computer::responseSlot: type:" << "T_FaPai";
        this->resl_FaPai(response);
        break;
    case MJ_response::T_ChuPai:
        //qDebug() << "Computer::responseSlot: type:" << "T_ChuPai";
        this->resl_ChuPai(response);
        break;
    case MJ_response::T_Ok:
        //qDebug() << "Computer::responseSlot: type:" << "T_Ok";
        this->resl_OK(response);
        break;
    case MJ_response::T_UnSucc:
        //qDebug() << "Computer::responseSlot: type:" << "T_UnSucc";
        this->resl_Unsucc(response);
        break;
    case MJ_response::T_GMOver:
        //qDebug() << "Computer::responseSlot: type:" << "T_GMOver";
        this->resl_Over(response);
        break;
    default:
        break;
    }
}

void MJ_Computer::tmSlot()
{
    this->tm->stop();
    MJ_RequestData data(this->Id);
    data.setType(MJ_RequestData::R_ChuPai);
    data.setSenderID(this->Id);
    data.setCard(this->card);
    this->request->req_send(data);

    qDebug() << "_Computer" << "2s chupai";
}
