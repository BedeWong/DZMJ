#include "MJ_Computer.h"

#include <QDebug>

MJ_Computer::MJ_Computer(QObject *parent) : QObject(parent)
{
    this->player = new MJ_Player();
    this->request = new MJ_RequestLocal();
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

        this->player->init(lst, wang);   // 初始化 分析 发送分析结果
        //this->player->analysis();
        MJ_Base::CARD h[16] = {0};
        MJ_Base::CARD g[16] = {0};
        MJ_Base::CARD p[16] = {0};
        MJ_Base::CARD c[16] = {0};
        MJ_RequestData req(this->Id);
        req.setType(MJ_RequestData::R_HGPCList);
        req.setHGPCList(h, g, p, c);
        req.setSenderID(this->Id);

        request->req_send(req);
        qDebug() << "Computer::resl_init " << recvID << "  " << this->Id;
    }

}

void MJ_Computer::resl_wait(MJ_response &resp)
{

}

void MJ_Computer::resl_Chi(MJ_response &resp)
{

}

void MJ_Computer::resl_Peng(MJ_response &resp)
{

}

void MJ_Computer::resl_Gang(MJ_response &resp)
{

}

void MJ_Computer::resl_Hu(MJ_response &resp)
{

}

void MJ_Computer::resl_FaPai(MJ_response &resp)
{
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
        req.setCard(this->card);
        req.setSenderID(this->Id);

        request->req_send(req);
    }
}

void MJ_Computer::resl_ChuPai(MJ_response &resp)
{

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

    switch(type)
    {
    case MJ_response::T_Init:
        this->resl_init(response);
        break;
    case MJ_response::T_Wait:
        break;
    case MJ_response::T_Chi:
        this->resl_Chi(response);
        break;
    case MJ_response::T_Peng:
        this->resl_Peng(response);
        break;
    case MJ_response::T_Gang:
        this->resl_Gang(response);
        break;
    case MJ_response::T_Hu:
        this->resl_Hu(response);
        break;
    case MJ_response::T_FaPai:
        this->resl_FaPai(response);
        break;
    case MJ_response::T_ChuPai:
        this->resl_ChuPai(response);
        break;
    case MJ_response::T_Ok:
        this->resl_OK(response);
        break;
    case MJ_response::T_UnSucc:
        this->resl_Unsucc(response);
        break;
    case MJ_response::T_GMOver:
        this->resl_Over(response);
        break;
    default:
        break;
    }
}
