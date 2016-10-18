#include "MJ_Cli.h"
#include "ui_MJ_Cli.h"

MJ_Cli::MJ_Cli(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MJ_Cli)
{
    ui->setupUi(this);

    this->desk = nullptr;
}

MJ_Cli::~MJ_Cli()
{
    delete ui;
}

void MJ_Cli::on_DanJi_clicked()
{
    if(this->desk != nullptr)
        delete this->desk;

    this->desk = new MJ_Desktop(GF_Local);
    this->hide();
    this->desk->show();
}

void MJ_Cli::on_OnLine_clicked()
{
    if(this->desk != nullptr)
        delete this->desk;

    this->desk = new MJ_Desktop(GF_OnLine);
    this->hide();
    this->desk->show();
}
