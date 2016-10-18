#ifndef MJ_CLI_H
#define MJ_CLI_H

#include <QWidget>

#include "MJ_Desktop.h"

namespace Ui {
class MJ_Cli;
}

class MJ_Cli : public QWidget
{
    Q_OBJECT

public:
    explicit MJ_Cli(QWidget *parent = 0);
    ~MJ_Cli();

private slots:
    void on_DanJi_clicked();

    void on_OnLine_clicked();

public:
    enum GF_Flag{
        GF_Local, GF_OnLine
    };

private:
    Ui::MJ_Cli *ui;

    MJ_Desktop *desk;
};

#endif // MJ_CLI_H
