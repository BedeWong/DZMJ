#ifndef MJ_DISCARDWIDGET_H
#define MJ_DISCARDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#include "MJ_widgetBase.h"
#include "../../pub/MJ_Base.h"

class MJ_DiscardWidget : public QWidget
{
    Q_OBJECT
public:
    enum SIZE{
        Size_x = 580,
        Size_y = 320
    };

public:
    explicit MJ_DiscardWidget(QWidget *parent = 0);

    void addCard(MJ_Base::CARD cd);
    void undo();

    void setId(int _id);

private:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

signals:

public slots:

private:
    int count;
    MJ_Base::CARD cardSet[64];

    MJ_widgetBase *MJcard;
    QPixmap *display;

    qreal x_scaled;// 缩放因子
    qreal y_scaled;// 缩放因子
    QSize sz;
    int ID;
};

#endif // MJ_DISCARDWIDGET_H
