#ifndef MJ_XIAJIAWIDGET_H
#define MJ_XIAJIAWIDGET_H

#include <QWidget>

class MJ_otherPlayer;
class MJ_widgetBase;

class MJ_XiaJiaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MJ_XiaJiaWidget(QWidget *parent = 0);
    void setModel(MJ_otherPlayer *model);

    void setSize(QSize sz);

    void draw_NewCard(bool flag);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

private:
    void draw_GPC(QPainter &painter);

    void draw_PaiList(QPainter &painter);

private:
    enum DisplaySize{
        DS_Width = 100,    // x
        DS_Height = 900    // y
    };

    qreal x_scale, y_scale;
    QPoint nextStartPoint;
    QPixmap *pix_display;
    MJ_otherPlayer *dataModel;
    bool draw_newCard;

    MJ_widgetBase *WidgetBaseInstance;
};

#endif // MJ_XIAJIAWIDGET_H
