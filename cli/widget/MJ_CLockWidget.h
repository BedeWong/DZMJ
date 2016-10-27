#ifndef MJ_CLOCKWIDGET_H
#define MJ_CLOCKWIDGET_H

#include <QWidget>
#include <QSize>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QTimer>

class MJ_CLockWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MJ_CLockWidget(int _zhuang, QSize _sz=QSize(80, 60), QWidget *parent = 0);

    void setSize(QSize _sz);
    void setZhuang(int z);
    void clockStart(int curid, int s=10, bool stat=true);

private:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void timerSlot();

private:
    bool flicker;
    int timeout;
    QTimer *tm;
    QSize sz;
    QPoint center;
    QPoint drawPath[4][3];
    QPoint drawFangWei[4];
    int zhuang;
    int cur_id;
    QPixmap *bg;
    QByteArray fangwei[4];
};

#endif // MJ_CLOCKWIDGET_H
