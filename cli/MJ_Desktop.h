#ifndef MJ_DESKTOP_H
#define MJ_DESKTOP_H

#include <QWidget>

#include "MJ_Cli.h"
#include "../pub/MJ_Base.h"
#include "../pub/MJ_Player.h"
#include "../pub/MJ_otherPlayer.h"
#include "MJ_RequestLocal.h"
#include "MJ_LocalServer.h"

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

private:
    MJ_Base *player[4];

    MJ_Request *request;

    MJ_LocalServer *server;

    int self;//本家位置
    int zhuang;//庄家位置
    int ID; //本家ID
};

#endif // MJ_DESKTOP_H
