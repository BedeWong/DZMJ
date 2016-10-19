#ifndef MJ_DESKTOP_H
#define MJ_DESKTOP_H

#include <QWidget>

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

public slots:
    void responseSlot(MJ_response response);

private:
    MJ_Base *player[4];
    MJ_Player *self;

    MJ_RequestLocal *request;

    MJ_LocalServer *server;

    int self_offset;//本家位置
    int zhuang;//庄家位置
    int ID; //本家ID
};

#endif // MJ_DESKTOP_H
