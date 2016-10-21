#include "MJ_Desktop.h"
#include "ui_MJ_Desktop.h"

MJ_Desktop::MJ_Desktop(MJ_Cli::GF_Flag flag, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MJ_Desktop)
{
    ui->setupUi(this);

    this->player[0] = new MJ_Player();
    this->player[1] = new MJ_otherPlayer();
    this->player[2] = new MJ_otherPlayer();
    this->player[3] = new MJ_otherPlayer();

    this->self = dynamic_cast<MJ_Player*> (this->player[0]);

    if(flag == MJ_Cli::GF_Local)
    {
        this->request = new MJ_RequestLocal();
        this->server = new MJ_LocalServer();
        this->zhuang = 0;
        this->self_offset = 0;//庄家，本家都在0下标位置
        this->ID = 0;

        this->computer1 = new MJ_Computer();
        this->computer1->setID(1);
        this->computer2 = new MJ_Computer();
        this->computer2->setID(2);
        this->computer3 = new MJ_Computer();
        this->computer3->setID(3);

        QObject::connect(this->request, SIGNAL(RequestSignal(MJ_RequestData)),
                this->server, SLOT(RecvSlot(MJ_RequestData)));
    }
    else if(flag == MJ_Cli::GF_OnLine)
    {

    }
}

MJ_Desktop::~MJ_Desktop()
{
    delete ui;

    delete this->player[0];
    delete this->player[1];
    delete this->player[2];
    delete this->player[3];

    delete this->request;
    delete this->server;
}

void MJ_Desktop::resl_init(MJ_response &resp)
{
    //初始化数据
    // 发送分析结果
    int senderid = resp.getSendTo();
    MJ_Base::CARD lst[16] = {0};
    resp.getPaiList(lst);

    this->self->init(lst, resp.getCard()); // 初始化数据
    MJ_Base::CARD h[16] = {0};
    MJ_Base::CARD g[16] = {0};
    MJ_Base::CARD p[16] = {0};
    MJ_Base::CARD c[16] = {0};
    this->self->getHuList(h, 16);
    this->self->getGangList(g, 16);
    this->self->getPengList(p, 16);
    this->self->getChiList(c, 16);

    MJ_RequestData req(this->ID);
    req.setType(MJ_RequestData::R_HGPCList);
    req.setHGPCList(h, g, p, c);
    this->request->req_send(req);
}

void MJ_Desktop::resl_wait(MJ_response &resp)
{
    //判断自己可以胡杠碰吃？
}

void MJ_Desktop::resl_Chi(MJ_response &resp)
{

}

void MJ_Desktop::resl_Peng(MJ_response &resp)
{

}

void MJ_Desktop::resl_Gang(MJ_response &resp)
{

}

void MJ_Desktop::resl_Hu(MJ_response &resp)
{

}

void MJ_Desktop::resl_FaPai(MJ_response &resp)
{

}

void MJ_Desktop::resl_ChuPai(MJ_response &resp)
{

}

void MJ_Desktop::resl_OK(MJ_response &resp)
{

}

void MJ_Desktop::resl_Unsucc(MJ_response &resp)
{

}

void MJ_Desktop::resl_Over(MJ_response &resp)
{

}

void MJ_Desktop::responseSlot(MJ_response response)
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













