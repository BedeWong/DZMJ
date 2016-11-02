#ifndef MJ_WANGANDREMAINCARDCOUNT_H
#define MJ_WANGANDREMAINCARDCOUNT_H

// 显示 王牌
// 显示剩余牌数
#include <QWidget>
#include <QLabel>
#include "../../pub/MJ_Base.h"

class MJ_widgetBase;
class MJ_wangAndRemainCardCount : public QWidget
{
    Q_OBJECT
public:
    explicit MJ_wangAndRemainCardCount(QWidget *parent = 0);

    void mj_setCard(MJ_Base::CARD cd);
    void mj_setPaiCount(int count);

    int operator --(int);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

private:
    MJ_widgetBase *widgetBaseInstance;
    MJ_Base::CARD card;//王
    int paiCount; // 剩下的牌数
    QLabel *dis_paiCountBg;
    QLabel *dis_paiCount;
};

#endif // MJ_WANGANDREMAINCARDCOUNT_H
