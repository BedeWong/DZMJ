#ifndef MJ_DESKTOP_H
#define MJ_DESKTOP_H

#include <QWidget>

#include "../AI/MJ_Computer.h"
#include "../pub/MJ_Base.h"
#include "../pub/MJ_Player.h"
#include "../pub/MJ_otherPlayer.h"
#include "../pub/MJ_response.h"
#include "MJ_RequestLocal.h"
#include "MJ_LocalServer.h"
#include "MJ_Cli.h"

namespace Ui {
class MJ_Desktop;
}

class MJ_Desktop : public QWidget
{
    Q_OBJECT

public:
    explicit MJ_Desktop(MJ_Cli::GF_Flag flag, QWidget *parent = 0);
    ~MJ_Desktop();

private:
    Ui::MJ_Desktop *ui;

public:
    enum S_HGPC{
        S_HU,
        S_GANG,
        S_PENG,
        S_CHI
    };

private:
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

    S_HGPC s_stat;
    bool s_hgpc;
};

#endif // MJ_DESKTOP_H
