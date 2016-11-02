#ifndef MJ_SELFWIDGET_H
#define MJ_SELFWIDGET_H

#include "../../pub/MJ_Player.h"
#include <QWidget>
#include <QList>

class MJ_Player;
class MJ_widgetBase;
class MJ_SelfWidgetItem;
class MJ_selfWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MJ_selfWidget(QWidget *parent = 0);

    void setModel(MJ_Player *model);

    void setSize(QSize sz);

    void draw_PaiList();

    void draw_NewCard(bool);

    void setWang(MJ_Base::CARD wang);

protected:
    void paintEvent(QPaintEvent *event);

private:

    void draw_GPC(QPainter &painter);

signals:
    void retClicked(MJ_Base::CARD cd);

public slots:
    void itemClicked(MJ_Base::CARD cd);

private:
    enum DisplaySize{
        DS_Width = 840,    // x
        DS_Height = 100    // y
    };

    qreal x_scale, y_scale;
    bool gpc_need_redraw;
    bool draw_newCard;
    QPoint nextStartPoint;
    QPixmap *pix_display;
    MJ_Player *dataModel;

    MJ_Base::CARD wang;

    QList<MJ_SelfWidgetItem*> items;
    MJ_SelfWidgetItem *newCard;

    MJ_widgetBase *WidgetBaseInstance;
};


class MJ_SelfWidgetItem : public QWidget
{
    Q_OBJECT
public:
    explicit MJ_SelfWidgetItem(MJ_Base::CARD cd = MJ_Base::MJ_noCard, QWidget *parent = 0);
    MJ_SelfWidgetItem(const MJ_SelfWidgetItem &);

    void setCard(MJ_Base::CARD cd);
    void setSize(int x, int y);

private:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

protected:
    bool event(QEvent *event);

signals:
    void ItemClicked(MJ_Base::CARD cd);

public slots:

private:
    enum CardSize{
        cs_width = 58,
        cs_height = 84
    };

    qreal x_scale, y_scale;
    MJ_Base::CARD card;
    bool isClicked;
    bool isTall;
    MJ_widgetBase *WidgetBaseInstance;
};

#endif // MJ_SELFWIDGET_H
