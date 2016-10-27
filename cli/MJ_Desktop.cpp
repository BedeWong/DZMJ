#include "MJ_Desktop.h"
#include "ui_MJ_Desktop.h"

#include <QDesktopWidget>
#include <QDebug>

MJ_Desktop::MJ_Desktop(MJ_Cli::GF_Flag flag, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MJ_Desktop)
{
    ui->setupUi(this);
    this->resize(800, 600);
    QDesktopWidget * desktopWidget = QApplication::desktop();
    this->setGeometry((desktopWidget->width()-this->width())/2,
                      (desktopWidget->height()-this->height())/2, this->width(), this->height());

    this->player[0] = new MJ_Player();
    this->player[1] = new MJ_otherPlayer();
    this->player[2] = new MJ_otherPlayer();
    this->player[3] = new MJ_otherPlayer();
    this->self = dynamic_cast<MJ_Player*> (this->player[0]);
    this->self->init("", MJ_Base::MJ_noCard);
    //this->player[0]->init();
    this->player[1]->init();
    this->player[2]->init();
    this->player[3]->init();

//    this->tmChuPai = new QTimer();
//    this->tmWait = new QTimer();
//    connect(this->tmChuPai, SIGNAL(timeout()), this, SLOT(tmChuPaiSLot()));
//    connect(this->tmWait, SIGNAL(timeout()), this, SLOT(tmWaitSlot()));


    if(flag == MJ_Cli::GF_Local)
    {
        this->request = new MJ_RequestLocal();
        this->server = MJ_LocalServer::getInstance();
        this->zhuang = 0;
        this->self_offset = 0;//庄家，本家都在0下标位置
        this->ID = 0;

        this->computer1 = new MJ_Computer(this);
        this->computer1->init();
        this->computer1->setID(1);
        this->computer2 = new MJ_Computer(this);
        this->computer2->init();
        this->computer2->setID(2);
        this->computer3 = new MJ_Computer(this);
        this->computer3->init();
        this->computer3->setID(3);

        connect(this->server, SIGNAL(responseSignal(MJ_response)),
                this->computer1, SLOT(responseSlot(MJ_response)));
        connect(this->server, SIGNAL(responseSignal(MJ_response)),
                        this->computer2, SLOT(responseSlot(MJ_response)));
        connect(this->server, SIGNAL(responseSignal(MJ_response)),
                        this->computer3, SLOT(responseSlot(MJ_response)));

        // bind svr --->   this
//        QObject::connect(this->request, SIGNAL(RequestSignal(MJ_RequestData)),
//                this->server, SLOT(RecvSlot(MJ_RequestData)));
        QObject::connect(this->server, SIGNAL(responseSignal(MJ_response)),
                         this, SLOT(responseSlot(MJ_response)));
    }
    else if(flag == MJ_Cli::GF_OnLine)
    {
        // ......
    }

    this->init_widgets();

    qDebug() << "MJ_Desktop::MJ_Desktop  end!" << endl;
}

MJ_Desktop::~MJ_Desktop()
{
    delete ui;

    delete this->player[0];
    delete this->player[1];
    delete this->player[2];
    delete this->player[3];

//    delete this->tmChuPai;
//    delete this->tmWait;

    delete this->request;
    delete this->server;
}

void MJ_Desktop::preStart()
{

}

void MJ_Desktop::init_widgets()
{
    this->startButton = new QPushButton(QString::fromLocal8Bit("开始"),this);
    this->startButton->move((this->width()-this->startButton->width())/2, this->height()*3/4);
    connect(this->startButton, SIGNAL(clicked(bool)), this, SLOT(startButtonClicked(bool)));

    this->HGPC_widget = new MJ_HGPCWidget(QSize(100, 30), this);
    this->clock_wdiget= new MJ_CLockWidget(0, QSize(80, 60), this);
    this->disCard_Widget[0] = new MJ_DiscardWidget(this);
    this->disCard_Widget[1] = new MJ_DiscardWidget(this);
    this->disCard_Widget[2] = new MJ_DiscardWidget(this);
    this->disCard_Widget[3] = new MJ_DiscardWidget(this);

    this->self_widget       = new MJ_selfWidget(this);
    this->DuiMen_widget     = new MJ_DuiMenWidget(this);
    this->ShangJia_widget   = new MJ_ShangJiaWidget(this);
    this->XiaJia_widget     = new MJ_XiaJiaWidget(this);


    this->self_widget->setModel(this->self);
    this->XiaJia_widget->setModel(dynamic_cast<MJ_otherPlayer*>(this->player[1]));
    this->DuiMen_widget->setModel(dynamic_cast<MJ_otherPlayer*>(this->player[2]));
    this->ShangJia_widget->setModel(dynamic_cast<MJ_otherPlayer*>(this->player[3]));

    int x = this->size().width();
    int y = this->size().height();

    this->self_widget->setSize(QSize(x*0.7, y/6));
    this->self_widget->move((x-this->self_widget->width()) / 2,
                            y - this->self_widget->height());
    qDebug() << this->self_widget->geometry();

    this->DuiMen_widget->setSize(QSize(x*0.7, y/6));
    this->DuiMen_widget->move((x-this->self_widget->width())/2, 0);
    qDebug() << this->DuiMen_widget->geometry();

    this->XiaJia_widget->setSize(QSize(x/6, y*0.7));
    this->XiaJia_widget->move(x - this->XiaJia_widget->width() + this->clock_wdiget->width(),
                              (y - this->XiaJia_widget->height()) / 2);
    qDebug() << this->XiaJia_widget->geometry();

    this->ShangJia_widget->setSize(QSize(x/6, y*0.7));
    this->ShangJia_widget->move(0, (this->height()-this->ShangJia_widget->height()) / 2);
    qDebug() << this->ShangJia_widget->geometry();
}

void MJ_Desktop::resl_init(MJ_response &resp)
{
    //初始化数据
    // 发送分析结果
    int recverID = resp.getSendTo();
    MJ_Base::CARD lst[16] = {0};
    resp.getPaiList(lst);

    qDebug() << "MJ_Desktop  Init:" << lst << " " << resp.getCard();
    qDebug() << "MJ_Desktop  Init: recvid = " << recverID;
    if(recverID == this->ID)
    {
        this->self->init(lst, resp.getCard()); // 初始化数据
        MJ_Base::CARD h[16] = {0};
        MJ_Base::CARD g[16] = {0};
        MJ_Base::CARD p[16] = {0};
        MJ_Base::CARD c[16] = {0};
        this->self->getCanHuList(h, 16);
        this->self->getCanGangList(g, 16);
        this->self->getCanPengList(p, 16);
        this->self->getCanChiList(c, 16);

        MJ_RequestData req(this->ID);
        req.setType(MJ_RequestData::R_HGPCList);
        req.setHGPCList(h, g, p, c);
        this->request->req_send(req);

        this->self_widget->draw_NewCard(false);
        this->self_widget->draw_PaiList();
    }
    else
    {
        this->player[recverID]->init(lst, resp.getCard());
    }

    /****
       *
       *  TEST
    */

    update();
   /**/
}

void MJ_Desktop::resl_wait(MJ_response &resp)
{
    //this->tmWait->start(9900);//10s 定时
    //判断自己可以胡杠碰吃？
    int who = resp.getWho();
    int recverId = resp.getSendTo();
    MJ_Base::CARD card= resp.getCard();

    this->s_stat = S_None;
    this->s_hgpc = false;

    //  自己的就算了
    if(who == this->ID)
    {
        return;
    }
    //  不是广播的消息，不是对自己发的消息 忽略
    if(recverId != MJ_response::SDT_Broadcast && recverId != this->ID)
    {
        return;
    }

    int self_hgpcStat = 0;
    if((who+1)%4 == this->ID && this->self->testChi(card))
        self_hgpcStat |= S_CHI;
    if(this->self->testPeng(card))
        self_hgpcStat |= S_PENG;
    if(this->self->testGang(card))
        self_hgpcStat |= S_GANG;
    if(this->self->testHu(card))
        self_hgpcStat |= S_HU;
    /***************
       *  调用胡杠碰吃选择窗口
    */
}

//  下面的 吃碰杠胡都是收到的动作，
//  通过判断是谁发出，做出相应的界面调整
//  也有可能是自己的，因为自己发出动作需要得到MJ_LocalServer的验证
//  收到此消息不代表就成功了，需要等待接受 出牌 消息，出牌消息发给谁，谁才是最终执行成功的
void MJ_Desktop::resl_Chi(MJ_response &resp)
{
    int who = resp.getWho();
    int recverId = resp.getSendTo();

    this->s_stat = S_CHI;
    this->s_hgpc = true;
    this->s_id   = who;

    MJ_Base::CARD _chi[4] = {0};
    resp.getChi(_chi);
    if(who == this->ID)
    {
        this->self->Chi(resp.getCard(), _chi);
    }
    else
        this->player[who]->Chi(resp.getCard(), _chi);
    /*********
     *    更新界面
     */
}

void MJ_Desktop::resl_Peng(MJ_response &resp)
{
    int who = resp.getWho();
    int recverId = resp.getSendTo();

    //  如果有玩家吃过这张牌，需要回退操作
    if(this->s_hgpc && this->s_stat == S_CHI)
    {
        this->player[this->s_id]->UndoChi();
    }

    this->s_stat = S_PENG;
    this->s_hgpc = true;
    this->s_id   = who;
    if(who == this->ID)
    {
        this->self->Peng(resp.getCard());
    }
    else
        this->player[who]->Peng(resp.getCard());
    /*********
     *    更新界面
     */
}

void MJ_Desktop::resl_Gang(MJ_response &resp)
{
    int who = resp.getWho();
    int recverId = resp.getSendTo();

    //  如果有玩家吃过这张牌，需要回退操作
    if(this->s_hgpc && this->s_stat == S_CHI)
    {
        this->player[this->s_id]->UndoChi();
    }
    //  如果有玩家碰了这张牌，err
    if(this->s_hgpc && this->s_stat == S_PENG)
    {

    }

    this->s_stat = S_GANG;
    this->s_hgpc = true;
    this->s_id   = who;
    if(who == this->ID)
    {
        this->self->Gang(resp.getCard());
    }
    else
        this->player[who]->Gang(resp.getCard());
    /*********
     *    更新界面
     */
}

void MJ_Desktop::resl_Hu(MJ_response &resp)
{
    int who = resp.getWho();
    int recverId = resp.getSendTo();

    //  如果有玩家吃[碰杠]过这张牌，需要回退操作
    if(this->s_hgpc && this->s_stat == S_CHI)
    {
        this->player[this->s_id]->UndoChi();
    }
    else if(this->s_hgpc && this->s_stat == S_PENG)
    {
        this->player[this->s_id]->UndoPeng();
    }
    else if(this->s_hgpc && this->s_stat == S_GANG)
    {
        this->player[this->s_id]->UndoGang();
    }

    this->s_stat = S_HU;
    this->s_hgpc = true;
    this->s_id   = who;

    MJ_Base::CARD pailist[16] = {0};
    resp.getPaiList(pailist);
    if(who == this->ID)
    {
        this->self->Hu(resp.getCard(), pailist);
    }
    else
        this->player[who]->Hu(resp.getCard(), pailist);
    /*********
     *    更新界面
     */
}

void MJ_Desktop::resl_FaPai(MJ_response &resp)
{
    int who = resp.getWho();
    int recverID = resp.getSendTo();

    cur_zhuapai = who;//标记当前要出牌的玩家
    if(who == this->ID)
    {
        this->self_widget->draw_NewCard(true);
        this->XiaJia_widget->draw_NewCard(false);
        this->ShangJia_widget->draw_NewCard(false);
        this->DuiMen_widget->draw_NewCard(false);

        this->self_newCard = resp.getCard();
        this->self->setNewCard(resp.getCard());
        this->self_widget->draw_PaiList();

        int hg_stat = 0;
        if(this->self->testGang(self_newCard))
            hg_stat |= S_GANG;
        if(this->self->testHu(self_newCard))
            hg_stat |= S_HU;
        if(hg_stat != S_None)
        {
            /***
             *  显示可胡牌，杠牌窗口
             */
        }
    }
    else
    {
        if(who == 1)
        {
            this->XiaJia_widget->draw_NewCard(true);
            this->self_widget->draw_NewCard(false);
            this->ShangJia_widget->draw_NewCard(false);
            this->DuiMen_widget->draw_NewCard(false);
        }
        else if(who == 2)
        {
            this->DuiMen_widget->draw_NewCard(true);
            this->self_widget->draw_NewCard(false);
            this->XiaJia_widget->draw_NewCard(false);
            this->ShangJia_widget->draw_NewCard(false);
        }
        else if(who == 3)
        {
            this->ShangJia_widget->draw_NewCard(true);
            this->self_widget->draw_NewCard(false);
            this->DuiMen_widget->draw_NewCard(false);
            this->XiaJia_widget->draw_NewCard(false);
        }
        else
        {
            qDebug() << "MJ_Desktop::resl_FaPai err: who = " << who;
        }
    }

    this->clock_wdiget->clockStart(who, 8);
    update();

    qDebug() << "MJ_Desktop::resl_FaPai" << endl;
}

void MJ_Desktop::resl_ChuPai(MJ_response &resp)
{
    qDebug() << "MJ_Desktop::resl_ChuPai:" << resp.getCard() << endl;

    int who = resp.getWho();
    MJ_Base::CARD card = resp.getCard();

    if(who < 0 || who >=4)
        throw("err:");

    if(who == this->ID)
    {
        if(card == this->self_newCard)
        {
            this->self_widget->draw_NewCard(false);
        }
        else
        {
            this->self->addCard(this->self_newCard);
            this->self->ChuPai(card);
        }

    }
    // 添加到出过的牌 窗口
    //this->disCard_Widget[who]->addCard(card);

    //this->player[who]->ChuPai(resp.getCard());
    qDebug() << "MJ_Desktop::resl_ChuPai:" << " endl" << endl;
}

void MJ_Desktop::resl_OK(MJ_response &resp)
{

}

void MJ_Desktop::resl_Unsucc(MJ_response &resp)
{
    int who = resp.getWho();
    int recverID = resp.getSendTo();

    if(recverID == this->ID)
    {
        /****
         * 通常是 胡杠碰吃 窗口需要调整
         */

    }
}

void MJ_Desktop::resl_Over(MJ_response &resp)
{
    int who = resp.getWho();

    if(who == this->ID)
    {

    }

    this->preStart();
}

void MJ_Desktop::responseSlot(MJ_response response)
{
    int type = response.getType();

    qDebug() << "MJ_Desktop::responseSlot: Send to" << response.getSendTo();
    qDebug() << "MJ_Desktop::responseSlot: who" << response.getWho();

    switch(type)
    {
    case MJ_response::T_Init:
        qDebug() << "MJ_Desktop::responseSlot: type:" << "T_Init";
        this->resl_init(response);
        break;
    case MJ_response::T_Wait:
        qDebug() << "MJ_Desktop::responseSlot: type:" << "T_Wait";
        break;
    case MJ_response::T_Chi:
        qDebug() << "MJ_Desktop::responseSlot: type:" << "T_Chi";
        this->resl_Chi(response);
        break;
    case MJ_response::T_Peng:
        qDebug() << "MJ_Desktop::responseSlot: type:" << "T_Peng";
        this->resl_Peng(response);
        break;
    case MJ_response::T_Gang:
        qDebug() << "MJ_Desktop::responseSlot: type:" << "T_Gang";
        this->resl_Gang(response);
        break;
    case MJ_response::T_Hu:
        qDebug() << "MJ_Desktop::responseSlot: type:" << "T_Hu";
        this->resl_Hu(response);
        break;
    case MJ_response::T_FaPai:
        qDebug() << "MJ_Desktop::responseSlot: type:" << "T_FaPai";
        this->resl_FaPai(response);
        break;
    case MJ_response::T_ChuPai:
        qDebug() << "MJ_Desktop::responseSlot: type:" << "T_ChuPai";
        this->resl_ChuPai(response);
        break;
    case MJ_response::T_Ok:
        qDebug() << "MJ_Desktop::responseSlot: type:" << "T_Ok";
        this->resl_OK(response);
        break;
    case MJ_response::T_UnSucc:
        qDebug() << "MJ_Desktop::responseSlot: type:" << "T_UnSucc";
        this->resl_Unsucc(response);
        break;
    case MJ_response::T_GMOver:
        qDebug() << "MJ_Desktop::responseSlot: type:" << "T_GMOver";
        this->resl_Over(response);
        break;
    default:
        break;
    }
}

void MJ_Desktop::startButtonClicked(bool)
{
    MJ_RequestData req(this->ID);

    req.setType(MJ_RequestData::R_Init);
    req.setSenderID(this->ID);

    this->request->req_send(req);
    this->startButton->hide();
}

//void MJ_Desktop::tmChuPaiSLot()
//{

//}

//void MJ_Desktop::tmWaitSlot()
//{

//}













