#include "MJ_Desktop.h"
#include "ui_MJ_Desktop.h"
#include "../pub/MJ_AnalyResult.h"

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
                this->computer1, SLOT(responseSlot(MJ_response)), Qt::QueuedConnection);
        connect(this->server, SIGNAL(responseSignal(MJ_response)),
                        this->computer2, SLOT(responseSlot(MJ_response)), Qt::QueuedConnection);
        connect(this->server, SIGNAL(responseSignal(MJ_response)),
                        this->computer3, SLOT(responseSlot(MJ_response)), Qt::QueuedConnection);

        // bind svr --->   this
//        QObject::connect(this->request, SIGNAL(RequestSignal(MJ_RequestData)),
//                this->server, SLOT(RecvSlot(MJ_RequestData)));
        QObject::connect(this->server, SIGNAL(responseSignal(MJ_response)),
                         this, SLOT(responseSlot(MJ_response)), Qt::QueuedConnection);
    }
    else if(flag == MJ_Cli::GF_OnLine)
    {
        // ......
    }

    this->init_widgets();
}

MJ_Desktop::~MJ_Desktop()
{
    delete ui;

    delete this->player[0];
    delete this->player[1];
    delete this->player[2];
    delete this->player[3];

    delete this->request;
    delete this->gameOverWidget;
}

void MJ_Desktop::preStart()
{
    this->startButton->show();
}

void MJ_Desktop::init_widgets()
{
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
    this->gameOverWidget    = new MJ_gameOverWidget(this);

    wangAndRemainCardCount = new MJ_wangAndRemainCardCount(this);

    connect(self_widget, SIGNAL(retClicked(MJ_Base::CARD)),
            this, SLOT(selfWidgetSlot(MJ_Base::CARD)), Qt::QueuedConnection);
    connect(HGPC_widget, SIGNAL(hgpc_finished()),
            this, SLOT(HGPCWidgetSlot()), Qt::QueuedConnection);

    this->self_widget->setModel(this->self);
    this->XiaJia_widget->setModel(dynamic_cast<MJ_otherPlayer*>(this->player[1]));
    this->DuiMen_widget->setModel(dynamic_cast<MJ_otherPlayer*>(this->player[2]));
    this->ShangJia_widget->setModel(dynamic_cast<MJ_otherPlayer*>(this->player[3]));

    int x = this->size().width();
    int y = this->size().height();

    this->self_widget->setSize(QSize(x*0.6, y/6));
    this->self_widget->move((x-this->self_widget->width()) / 2,
                            y - this->self_widget->height());
    qDebug() << this->self_widget->geometry();

    this->DuiMen_widget->setSize(QSize(x*0.6, y/6));
    this->DuiMen_widget->move((x-this->self_widget->width())/2, 0);
    qDebug() << this->DuiMen_widget->geometry();

    this->XiaJia_widget->setSize(QSize(x/6, y*0.6));
    this->XiaJia_widget->move(x - this->XiaJia_widget->width() + this->clock_wdiget->width(),
                              (y - this->XiaJia_widget->height()) / 2);
    qDebug() << this->XiaJia_widget->geometry();

    this->ShangJia_widget->setSize(QSize(x/6, y*0.6));
    this->ShangJia_widget->move(0, (this->height()-this->ShangJia_widget->height()) / 2);
    qDebug() << this->ShangJia_widget->geometry();

    this->disCard_Widget[0]->setId(this->ID);
    this->disCard_Widget[1]->setId(1);
    this->disCard_Widget[2]->setId(2);
    this->disCard_Widget[3]->setId(3);

    x = this->width()*5/6;
    y = this->height()*5/6;

    this->disCard_Widget[0]->resize((this->width())/3.3, (this->height() - 80)/3 - this->self_widget->height()/3);
    this->disCard_Widget[0]->move(this->width()/2.8,
                                         /*this->height()-this->self_widget->height()-(this->height() - 80)/3);*/
                                  this->height()/2+40);
    this->disCard_Widget[1]->resize((this->width() - this->XiaJia_widget->width()-this->ShangJia_widget->width())/3,
                                    this->height()/3);
    this->disCard_Widget[1]->move(this->width()/2+120, this->height()/3);

    this->disCard_Widget[2]->resize(this->width()/3.3, (this->height()-80)/3 - this->DuiMen_widget->height()/3);
    this->disCard_Widget[2]->move(this->width()/3, this->height()/2-40-this->disCard_Widget[2]->height());
    this->disCard_Widget[3]->resize((this->width() - this->ShangJia_widget->width()-this->ShangJia_widget->width())/3,
                                    this->height()/3);
    this->disCard_Widget[3]->move(this->ShangJia_widget->width()-40, this->height()/3);

    //  开始按钮
    this->startButton = new QPushButton(QString::fromLocal8Bit("开始"),this);
    this->startButton->setObjectName("StartButton");
    this->startButton->move((this->width()-this->startButton->width())/2, this->height()*3/4);
    connect(this->startButton, SIGNAL(clicked(bool)), this, SLOT(startButtonClicked(bool)), Qt::QueuedConnection);

    this->HGPC_widget->move(this->width()*2/3, this->height()-this->self_widget->height()-60);
    this->HGPC_widget->hide();

    wangAndRemainCardCount->setGeometry(30, 30, this->width()/12, this->height()/7);

    this->gameOverWidget->move((this->width()-this->gameOverWidget->width())/2,
                               120);
    this->gameOverWidget->hide();
}

void MJ_Desktop::resl_init(MJ_response &resp)
{
    //初始化数据
    // 发送分析结果
    int recverID = resp.getSendTo();
    MJ_Base::CARD lst[16] = {0};
    resp.getPaiList(lst);

    qDebug() << "_Desktop  Init:" << lst << " " << resp.getCard()
             << "recvid = " << recverID;

    if(recverID == this->ID)
    {
        this->wangAndRemainCardCount->mj_setPaiCount(resp.getPaiCount());
        this->wangAndRemainCardCount->mj_setCard(resp.getCard());
        this->self_widget->setWang(resp.getCard());

        this->self->init(lst, resp.getCard()); // 初始化数据
        MJ_Base::CARD h[16] = {0};
        MJ_Base::CARD g[16] = {0};
        MJ_Base::CARD p[16] = {0};
        MJ_Base::CARD c[16] = {0};

        this->self->AnalysisHGPC();
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

    this->disCard_Widget[0]->clean();
    this->disCard_Widget[1]->clean();
    this->disCard_Widget[2]->clean();
    this->disCard_Widget[3]->clean();

    /****
       *  TEST    */

    update();
   /**/
}

// 出现多玩家可碰杠吃等情况，wait消息只会收到一次，、
// 收到wait消息后要记录后面的吃碰杠情况，保存权限最高的一个，最后谁收到出牌消息，谁才是最后胜出的
void MJ_Desktop::resl_wait(MJ_response &resp)
{
    //判断自己可以胡杠碰吃？
    int who = resp.getWho();//who：谁出的牌
    //int recverId = resp.getSendTo();//recverID通常是4，MJ_response::SDT_Broadcast
    MJ_Base::CARD card = resp.getCard();//刚才[who]出的牌


    this->s_card = card;
    this->s_stat = S_None;
    this->s_hgpc = false;
    this->s_id = who;   // 保存谁出的牌，吃[杠碰胡]时可以让他撤销

    //  定时 6s 不闪
    this->clock_wdiget->clockStart(0, 6, false);

    //  自己的就算了
    if(who == this->ID)
    {
        qDebug() << "Desktop::resl_wait:  SELF" << "return";
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
    *****************/
    //if(self_hgpcStat & S_CHI == (int)S_CHI) //err:
    if((self_hgpcStat & S_CHI) == S_CHI)
    {
        MJ_Base::CARD chilist[8][4] = {0};
        int ret = this->self->getCChiList(card, chilist);
        this->HGPC_widget->setChiList(chilist, ret);
        qDebug() << __FUNCTION__ << __LINE__ << "****chilist***" << chilist[0] << " "
                 << chilist[1] << " " << chilist[2];
    }

    //自己能胡杠碰吃 就弹窗
    if(self_hgpcStat)
        this->HGPC_widget->hgpc_show(self_hgpcStat);

    qDebug() << "Desktop::resl_wait: " << this->self->getCanChiList(nullptr, 0);
    qDebug() << "Desktop::resl_wait: " << (S_HGPC)self_hgpcStat << endl;
}

//  下面的 吃碰杠胡都是收到的动作，
//  通过判断是谁发出，做出相应的界面调整
//  也有可能是自己的，因为自己发出动作需要得到MJ_LocalServer的验证
//  收到此消息不代表就成功了，需要等待接受 出牌 消息，出牌消息发给谁，谁才是最终执行成功的
void MJ_Desktop::resl_Chi(MJ_response &resp)
{
    int who = resp.getWho();

    MJ_Base::CARD _chi[4] = {0};
    resp.getChi(_chi);

    if(who == this->ID)
    {
        this->self->Chi(resp.getCard(), _chi);
        this->HGPC_widget->hide();
    }
    else
        this->player[who]->Chi(resp.getCard(), _chi);

    // 撤销上一次的动作（本次撤销 玩家的出牌）
    this->disCard_Widget[this->s_id]->undo();

    this->s_stat = S_CHI;
    this->s_hgpc = true;
    this->s_id   = who;

    /*********
     *    更新界面
     */
}

void MJ_Desktop::resl_Peng(MJ_response &resp)
{
    int who = resp.getWho();

    //  如果有玩家吃过这张牌，需要回退操作
    if(this->s_hgpc && this->s_stat == S_CHI)
    {
        this->player[this->s_id]->UndoChi();
    }
    else
    {
        this->disCard_Widget[this->s_id]->undo();
    }

    if(who == this->ID)
    {
        this->self->Peng(resp.getCard());
        this->HGPC_widget->hide();
    }
    else
        this->player[who]->Peng(resp.getCard());


    this->s_stat = S_PENG;
    this->s_hgpc = true;
    this->s_id   = who;

    /*********
     *    更新界面
     */
}

void MJ_Desktop::resl_Gang(MJ_response &resp)
{
    int who = resp.getWho();

    //  如果有玩家吃过这张牌，需要回退操作
    if(this->s_hgpc && this->s_stat == S_CHI)
    {
        this->player[this->s_id]->UndoChi();
    }
    //  如果有玩家碰了这张牌，err
    else if(this->s_hgpc && this->s_stat == S_PENG)
    {

    }
    else
    {
        this->disCard_Widget[this->s_id]->undo();
    }

    this->s_stat = S_GANG;
    this->s_hgpc = true;
    this->s_id   = who;
    if(who == this->ID)
    {
        this->self->Gang(resp.getCard());
        this->HGPC_widget->hide();
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
    else
    {
        this->disCard_Widget[this->s_id]->undo();
    }

    this->s_stat = S_HU;
    this->s_hgpc = true;
    this->s_id   = who;

    MJ_Base::CARD pailist[16] = {0};
    MJ_Base::CARD g[8]={0}, p[8]={0}, c[16]={0};
    resp.getPaiList(pailist);
    resp.getgpc(g, p, c);

    qDebug() << QString::fromLocal8Bit("玩家%1胜出:").arg(who) << "pailist:" << pailist;
    qDebug() << "\tgang:" << g << "  p:" << p << " c:" << c;

    MJ_Player win = MJ_Player();
    win.setPengList(p);
    win.setGangList(g);
    win.setChiList(c);
    win.setPaiList(pailist);
    win.setWang(this->self->getWang());
    win.AnalysisHGPC();
    if(win.testHu(resp.getCard()))
    {
        MJ_AnalyResult analy(win);
        analy.calc_BeiShu(win, resp.getCard(), MJ_AnalyResult::F_JiePao);
        int fan = analy.getFan();
        auto list = analy.HU_names();

        this->gameOverWidget->setPai(g, p, c, pailist, resp.getCard());
        this->gameOverWidget->setFan(list, fan);
        this->gameOverWidget->show();
    }

    if(who == this->ID)
    {
        this->self->Hu(resp.getCard(), pailist);
        this->HGPC_widget->hide();
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
    this->self_newCard = resp.getCard();

    cur_zhuapai = who;//标记当前要出牌的玩家
    if(who == this->ID)
    {
        if(self_newCard != MJ_Base::MJ_noCard)
            this->self_widget->draw_NewCard(true);
        this->XiaJia_widget->draw_NewCard(false);
        this->ShangJia_widget->draw_NewCard(false);
        this->DuiMen_widget->draw_NewCard(false);

        this->HGPC_widget->hide();


        this->self->setNewCard(resp.getCard());
        this->self_widget->draw_PaiList();

        int hg_stat = 0;
        if(this->self->testGang(self_newCard))//暗杠
        {
            anGangCard = this->self_newCard;
            hg_stat |= S_GANG;
        }
        else if((anGangCard = this->self->testAnGang()) != MJ_Base::MJ_noCard)
        {
            hg_stat |= S_GANG;
        }
        if(this->self->testHu(self_newCard))
            hg_stat |= S_HU;
        if(this->self->testBuGang(self_newCard))//  补杠：补杠可被抢杠胡
            hg_stat |= S_BuGang;
        if(this->self->testZiMo(self_newCard))
            hg_stat |= S_ZiMo;

        if(hg_stat != S_None)
        {
            /***
             *  显示可胡牌，杠牌窗口
             */
            this->s_card  = self_newCard;
            this->HGPC_widget->hgpc_show(hg_stat);
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
            qDebug() << "_Desktop::resl_FaPai err: who = " << who;
        }
    }

    this->clock_wdiget->clockStart(who, 8);

    if(self_newCard != MJ_Base::MJ_noCard)
        this->wangAndRemainCardCount->mj_setPaiCount(resp.getPaiCount());

    update();

    qDebug() << "_Desktop::resl_FaPai" << endl;
}

void MJ_Desktop::resl_ChuPai(MJ_response &resp)
{
    qDebug() << "_Desktop::resl_ChuPai:" << resp.getCard() << endl;

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
            if(self_newCard != MJ_Base::MJ_noCard)
                this->self->addCard(this->self_newCard);
            this->self->ChuPai(card);

            // 分重新分析自己的胡杠碰吃 发送给svr
            MJ_Base::CARD h[16] = {0};
            MJ_Base::CARD g[16] = {0};
            MJ_Base::CARD p[16] = {0};
            MJ_Base::CARD c[16] = {0};

            qDebug() << __FUNCTION__ << __LINE__ << "  ";
            this->self->AnalysisHGPC();
            qDebug() << this->self->getCanHuList(h, 16);
            qDebug() << this->self->getCanGangList(g, 16);
            qDebug() << this->self->getCanPengList(p, 16);
            qDebug() << this->self->getCanChiList(c, 16);

            qDebug() << __FUNCTION__ << __LINE__ << "  " << h << g << p << c;

            MJ_RequestData req(this->ID);
            req.setType(MJ_RequestData::R_HGPCList);
            req.setHGPCList(h, g, p, c);
            this->request->req_send(req);
        }
        this->self_widget->draw_PaiList();
    }
    // 添加到出过的牌 窗口
    this->disCard_Widget[who]->addCard(card);
    this->HGPC_widget->hide();
    update();
    qDebug() << "_Desktop::resl_ChuPai:" << " endl" << endl;
}

void MJ_Desktop::resl_OK(MJ_response &)
{

}

void MJ_Desktop::resl_Unsucc(MJ_response &resp)
{
    int recverID = resp.getSendTo();

    if(recverID == this->ID)
    {
        /****
         * 通常是 胡杠碰吃 窗口需要调整
         */

    }
}

void MJ_Desktop::resl_Over(MJ_response &)
{

    this->clock_wdiget->clockStart(0, 0, false);
    this->preStart();
}

void MJ_Desktop::responseSlot(MJ_response response)
{
    int type = response.getType();

    qDebug() << "_Desktop::responseSlot: Send to" << response.getSendTo();
    qDebug() << "_Desktop::responseSlot: who" << response.getWho();

    switch(type)
    {
    case MJ_response::T_Init:
        qDebug() << "_Desktop::responseSlot: type:" << "T_Init";
        this->resl_init(response);
        break;
    case MJ_response::T_Wait:
        qDebug() << "_Desktop::responseSlot: type:" << "T_Wait";
        this->resl_wait(response);
        break;
    case MJ_response::T_Chi:
        qDebug() << "_Desktop::responseSlot: type:" << "T_Chi";
        this->resl_Chi(response);
        break;
    case MJ_response::T_Peng:
        qDebug() << "_Desktop::responseSlot: type:" << "T_Peng";
        this->resl_Peng(response);
        break;
    case MJ_response::T_Gang:
        qDebug() << "_Desktop::responseSlot: type:" << "T_Gang";
        this->resl_Gang(response);
        break;
    case MJ_response::T_Hu:
        qDebug() << "_Desktop::responseSlot: type:" << "T_Hu";
        this->resl_Hu(response);
        break;
    case MJ_response::T_FaPai:
        qDebug() << "_Desktop::responseSlot: type:" << "T_FaPai";
        this->resl_FaPai(response);
        break;
    case MJ_response::T_ChuPai:
        qDebug() << "_Desktop::responseSlot: type:" << "T_ChuPai";
        this->resl_ChuPai(response);
        break;
    case MJ_response::T_Ok:
        qDebug() << "_Desktop::responseSlot: type:" << "T_Ok";
        this->resl_OK(response);
        break;
    case MJ_response::T_UnSucc:
        qDebug() << "_Desktop::responseSlot: type:" << "T_UnSucc";
        this->resl_Unsucc(response);
        break;
    case MJ_response::T_GMOver:
        qDebug() << "_Desktop::responseSlot: type:" << "T_GMOver";
        this->resl_Over(response);
        break;
    default:
        break;
    }
}

void MJ_Desktop::selfWidgetSlot(MJ_Base::CARD cd)
{
    if(cur_zhuapai == this->ID) //
    {
        qDebug() << QString::fromLocal8Bit("出牌：") << cd;

        this->HGPC_widget->hide();
        this->cur_zhuapai = -1;//修复重复出牌bug

        MJ_RequestData req(this->ID);
        req.setType(MJ_RequestData::R_ChuPai);
        req.setCard(cd);
        req.setSenderID(this->ID);

        request->req_send(req);
    }
}

void MJ_Desktop::HGPCWidgetSlot()
{
    int result = HGPC_widget->getResult();
    qDebug() << "_Desktop::HGPCWidgetSlot:" << result;

    MJ_RequestData req(this->ID);
    switch(result)
    {
    case MJ_HGPCWidget::RES_HU:
        req.setType(MJ_RequestData::R_Hu);
        qDebug() << QString::fromLocal8Bit(" 我方胡牌：") << this->s_card;
        break;
    case MJ_HGPCWidget::RES_GANG:
        if(this->anGangCard != MJ_Base::MJ_noCard) // 可以暗杠就先暗杠
        {
            req.setType(MJ_RequestData::R_AnGang);
            this->s_card = this->anGangCard;
            qDebug() << QString::fromLocal8Bit(" 我方暗杠：") << this->s_card;
        }
        else
        {
            req.setType(MJ_RequestData::R_Gang);
            qDebug() << QString::fromLocal8Bit(" 我方明杠：") << this->s_card;
        }
        break;
    case MJ_HGPCWidget::RES_PENG:
        req.setType(MJ_RequestData::R_Peng);
        qDebug() << QString::fromLocal8Bit(" 我方碰牌：") << this->s_card;
        break;
    case MJ_HGPCWidget::RES_CHI:
        {
            req.setType(MJ_RequestData::R_Chi);
            MJ_Base::CARD cds[4] = {0};
            this->HGPC_widget->getChiSelected(cds);
            req.setChi(cds);
            qDebug() << QString::fromLocal8Bit(" 我方吃牌：") << cds;
        }
        break;
    //case MJ_HGPCWidget::RES_CANCEL:
    default:
        req.setType(MJ_RequestData::R_CanCel);
        qDebug() << QString::fromLocal8Bit(" 我方过：") << this->s_card;
        break;
    }
    req.setCard(this->s_card);
    req.setSenderID(this->ID);

    this->request->req_send(req);
}

void MJ_Desktop::startButtonClicked(bool)
{
    MJ_RequestData req(this->ID);

    req.setType(MJ_RequestData::R_Init);
    req.setSenderID(this->ID);

    this->request->req_send(req);
    this->startButton->hide();
    this->gameOverWidget->hide();
}




