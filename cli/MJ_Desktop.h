#ifndef MJ_DESKTOP_H
#define MJ_DESKTOP_H

#include <QPushButton>
#include <QWidget>
#include <QTimer>

#include "../AI/MJ_Computer.h"
#include "../pub/MJ_Base.h"
#include "../pub/MJ_Player.h"
#include "../pub/MJ_otherPlayer.h"
#include "../pub/MJ_response.h"
#include "MJ_RequestLocal.h"
#include "MJ_LocalServer.h"
#include "MJ_Cli.h"

#include "widget/MJ_HGPCWidget.h"
#include "widget/MJ_CLockWidget.h"
#include "widget/MJ_DiscardWidget.h"
#include "widget/MJ_DuiMenWidget.h"
#include "widget/MJ_selfWidget.h"
#include "widget/MJ_ShangJiaWidget.h"
#include "widget/MJ_XiaJiaWidget.h"

namespace Ui {
class MJ_Desktop;
}

class MJ_Desktop : public QWidget
{
    Q_OBJECT

public:
    explicit MJ_Desktop(MJ_Cli::GF_Flag flag, QWidget *parent = 0);
    ~MJ_Desktop();

    void preStart();

private:
    Ui::MJ_Desktop *ui;

public:
    enum S_HGPC{
        S_None  = 0x0000,
        S_HU    = 0x0001,
        S_GANG  = 0x0010,
        S_PENG  = 0x0100,
        S_CHI   = 0x1000
    };

private:
    void init_widgets();

    void resl_init(MJ_response&);
    void resl_wait(MJ_response&);
    void resl_Chi(MJ_response&);
    void resl_Peng(MJ_response&);
    void resl_Gang(MJ_response&);
    void resl_Hu(MJ_response&);
    void resl_FaPai(MJ_response&);
    void resl_ChuPai(MJ_response&);
    void resl_OK(MJ_response&);
    void resl_Unsucc(MJ_response&);
    void resl_Over(MJ_response&);
//    void resl_(MJ_response&);

public slots:
    void responseSlot(MJ_response response);

    void selfWidgetSlot(MJ_Base::CARD cd);
    void HGPCWidgetSlot();

//    void tmChuPaiSLot();
//    void tmWaitSlot();

    void startButtonClicked(bool);

private:
    MJ_Base *player[4];
    MJ_Player *self;
    MJ_Computer *computer1;
    MJ_Computer *computer2;
    MJ_Computer *computer3;

    MJ_RequestLocal *request;
    MJ_LocalServer *server;

    int self_offset;//本家位置
    int zhuang;//庄家位置
    int ID; //本家ID
    int cur_zhuapai;//当前要出牌玩家

    S_HGPC s_stat;  //保存一种状态，
    bool s_hgpc;//已操作
    int s_id;//为谁保存状态
    MJ_Base::CARD s_card;
    MJ_Base::CARD self_newCard;

    MJ_HGPCWidget *HGPC_widget;
    MJ_CLockWidget *clock_wdiget;
    MJ_DiscardWidget *disCard_Widget[4];
    MJ_selfWidget *self_widget;
    MJ_DuiMenWidget *DuiMen_widget;
    MJ_ShangJiaWidget *ShangJia_widget;
    MJ_XiaJiaWidget *XiaJia_widget;

    QPushButton *startButton;
};

#endif // MJ_DESKTOP_H
