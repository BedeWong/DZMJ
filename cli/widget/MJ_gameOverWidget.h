#ifndef MJ_GAMEOVERWIDGET_H
#define MJ_GAMEOVERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QListView>
#include <QStandardItemModel>

#include "../../pub/MJ_Base.h"
#include "MJ_widgetBase.h"

class WinnerPaiListWidget;
class MJ_gameOverWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MJ_gameOverWidget(QWidget *parent = 0);

    void setPai(MJ_Base::CARD *g,
                MJ_Base::CARD *p,
                MJ_Base::CARD *c,
                MJ_Base::CARD *pailist,
                MJ_Base::CARD card);

    void setFan(std::list<const char *> ll, int fan);

    void setWinner(QString winner);

signals:

public slots:

private:
    QListView *view;
    WinnerPaiListWidget *cdsWidget;
    QPushButton *okButton;
    QStandardItemModel *model;
    QLabel *winnerLabel;
};

/********************************************************
 *
 */
class WinnerPaiListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WinnerPaiListWidget(QWidget *parent = 0);
    explicit WinnerPaiListWidget(MJ_Base::CARD *g,
                                 MJ_Base::CARD *p,
                                 MJ_Base::CARD *c,
                                 MJ_Base::CARD *pailist,
                                 MJ_Base::CARD card,
                                 QWidget *parent = 0);
    void setPai(MJ_Base::CARD *g,
                MJ_Base::CARD *p,
                MJ_Base::CARD *c,
                MJ_Base::CARD *pailist,
                MJ_Base::CARD card);

protected:
    void paintEvent(QPaintEvent *event);

private:
    MJ_Base::CARD g[8];
    MJ_Base::CARD p[8];
    MJ_Base::CARD c[16];
    MJ_Base::CARD pailist[16];
    MJ_Base::CARD card;

    MJ_widgetBase *WidgetBaseInstance;
    QPixmap *pix;
};

#endif // MJ_GAMEOVERWIDGET_H

