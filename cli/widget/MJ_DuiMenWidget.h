#ifndef MJ_DUIMENWIDGET_H
#define MJ_DUIMENWIDGET_H

#include <QWidget>

class MJ_otherPlayer;
class MJ_widgetBase;
class MJ_DuiMenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MJ_DuiMenWidget(QWidget *parent = 0);

    void setModel(MJ_otherPlayer *model);

    void setSize(QSize sz);

    void draw_NewCard(bool);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void draw_GPC(QPainter &painter);

    void draw_PaiList(QPainter &painter);

signals:

public slots:

private:
    enum DisplaySize{
        DS_Width = 900,    // x
        DS_Height = 100    // y
    };

    qreal x_scale, y_scale;
    QPoint nextStartPoint;
    QPixmap *pix_display;
    MJ_otherPlayer *dataModel;
    bool draw_newCard;

    MJ_widgetBase *WidgetBaseInstance;
};

#endif // MJ_DUIMENWIDGET_H
