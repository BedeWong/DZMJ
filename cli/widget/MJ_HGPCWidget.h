#ifndef MJ_HGPCWIDGET_H
#define MJ_HGPCWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QResizeEvent>
#include "../../pub/MJ_Base.h"

class SelectChiwidget;
class MJ_HGPCWidget : public QWidget
{
    Q_OBJECT
public:
    enum RESULT{   // 用于记录返回值
        RES_HU = 0x01,
        RES_GANG,
        RES_PENG,
        RES_CHI,
        RES_CANCEL
    };

public:
    explicit MJ_HGPCWidget(QSize _sz = QSize(100, 30), QWidget *parent = 0);
    ~MJ_HGPCWidget();

    void setChiList(const MJ_Base::CARD (*ll)[4], int nCount);

    int getResult()const;
    bool getChiSelected(MJ_Base::CARD *cds)const;

protected:
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

signals:

public slots:
    void hgpc_show(int stat);

    void HuClicked(bool);
    void GangClicked(bool);
    void ChiClicked(bool);
    void ChiFinished();
    void PengClicked(bool);
    void CancelClicked(bool);

private:
    QPushButton *HuButton;
    QPushButton *GangButton;
    QPushButton *PengButton;
    QPushButton *ChiButton;
    QPushButton *CancelButton;

    SelectChiwidget *selectChiWidget;

    MJ_Base::CARD selectChi[4];

    int result;
};

/***** *** *** *** *** ***
 *      widget Item
 *
 */
#include "MJ_widgetBase.h"
class ChiwidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit ChiwidgetItem(const MJ_Base::CARD cds[], SelectChiwidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

    bool event(QEvent *event);

    void mousePressEvent(QMouseEvent *);
signals:
    void clickedSignal(MJ_Base::CARD *cds);
private:
    MJ_Base::CARD cards[4];

    bool draw_rect;//画
};

/******  ***  ***   ***
 *      弹出的 选择吃牌对话框
 *
 */
class SelectChiwidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectChiwidget(int nCount, QPoint pt, QWidget *parent = 0);

    void addItem(ChiwidgetItem *item);
    void getSelected(MJ_Base::CARD *cds)const;

protected:

signals:
    void finishSignal();

public slots:
    void ItemClickedSlot(MJ_Base::CARD *cds);

private:
    ChiwidgetItem *items[8];
    int nCount;

    MJ_Base::CARD selected[4];
};

#endif // MJ_HGPCWIDGET_H
