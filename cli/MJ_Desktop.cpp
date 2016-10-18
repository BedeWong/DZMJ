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

    if(flag == MJ_Cli::GF_Local)
    {
        this->request = new MJ_RequestLocal();
        this->server = new MJ_LocalServer();
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
