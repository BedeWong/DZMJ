#ifndef MJ_SHANGJIAWIDGET_H
#define MJ_SHANGJIAWIDGET_H

#include "../../pub/MJ_otherPlayer.h"
#include "MJ_widgetBase.h"

#include <QWidget>

class MJ_ShangJiaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MJ_ShangJiaWidget(QWidget *parent = 0);

    void setModel(MJ_otherPlayer *model);

    void setSize(QSize sz);

    void draw_NewCard(bool flag);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void draw_GPC(QPainter &painter);

    void draw_PaiList(QPainter &painter);
signals:

public slots:

private:
    enum DisplaySize{
        DS_Width = 100,    // x
        DS_Height = 800    // y
    };

    qreal x_scale, y_scale;
    QPoint nextStartPoint;
    QPixmap *pix_display;
    MJ_otherPlayer *dataModel;
    bool draw_newCard;

    MJ_widgetBase *WidgetBaseInstance;
};

#endif // MJ_SHANGJIAWIDGET_H
