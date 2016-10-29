#include "MJ_CLockWidget.h"
#include <QBitmap>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QFontMetrics>

#include <QDebug>

MJ_CLockWidget::MJ_CLockWidget(int _zhuang, QSize _sz, QWidget *parent) : QWidget(parent)
{
    this->flicker = false;
    this->timeout = 0;
    this->zhuang = _zhuang;

    this->setSize(_sz);
    this->tm = new QTimer(this);
    connect(this->tm, SIGNAL(timeout()), this, SLOT(timerSlot()), Qt::QueuedConnection);

    this->fangwei[0] = QByteArray("东");
    this->fangwei[1] = QByteArray("南");
    this->fangwei[2] = QByteArray("西");
    this->fangwei[3] = QByteArray("北");

    this->setAutoFillBackground(true);
    this->bg = new QPixmap(":/bg_clock.png");
    this->setMask(this->bg->mask());

    qDebug() << "MJ_CLockWidget::MJ_CLockWidget  end!" << endl;
}

void MJ_CLockWidget::setSize(QSize _sz)
{
    this->sz = _sz;
    this->center = QPoint(sz.width()/2, sz.height()/2);

    QPoint point((this->parentWidget()->width()-_sz.width())/2,
                 (this->parentWidget()->height()-_sz.height())/2);
    this->setGeometry(point.x(), point.y(), _sz.width(), _sz.height());

    this->drawPath[0][0] = QPoint(0, sz.height());
    this->drawPath[0][1] = this->center;
    this->drawPath[0][2] = QPoint(sz.width(), sz.height());

    this->drawPath[1][0] = this->center;
    this->drawPath[1][1] = QPoint(sz.width(), 0);
    this->drawPath[1][2] = QPoint(sz.width(), sz.height());

    this->drawPath[2][0] = QPoint(0, 0);
    this->drawPath[2][1] = QPoint(sz.width(), 0);
    this->drawPath[2][2] = this->center;

    this->drawPath[3][0] = QPoint(0, 0);
    this->drawPath[3][1] = center;
    this->drawPath[3][2] = QPoint(0, sz.height());

    QFont font("Times", 12);
    QFontMetrics met(font);
    int w = met.width(QString::fromLocal8Bit("东"));
    int h = met.height();
    drawFangWei[0] = QPoint(center.x()-w/2, sz.height()-h/2);
    drawFangWei[1] = QPoint(center.y()-h/2, -sz.width()+w);
    drawFangWei[2] = QPoint(-center.x()-w/3, -h/3);
    drawFangWei[3] = QPoint(-center.x()+w/3, w);
}

void MJ_CLockWidget::setZhuang(int z)
{
    this->zhuang = z;
}

void MJ_CLockWidget::clockStart(int curid, int s, bool stat)
{
    this->flicker = stat;
    this->timeout = s;
    this->cur_id = curid;
    tm->start(1000);
}

void MJ_CLockWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.drawPixmap(0,0,*this->bg);

    QPen pen(QColor(150,150,150));
    painter.setPen(pen);
    painter.drawLine(0, 0, sz.width(), sz.height());
    painter.drawLine(sz.width(), 0, 0, sz.height());

    //  画四个三角形 平分这么窗口
    for(int i=0; i<4; i++)
    {
        if(this->flicker && this->cur_id == i && this->timeout%2)
        {
            painter.setBrush(QBrush(QColor(240, 10, 10)));
        }
        else
        {
            painter.setBrush(Qt::NoBrush);
        }
        painter.drawPolygon(this->drawPath[i], 3);
    }

    painter.setBrush(QBrush(QColor(220, 220, 220)));
    painter.drawEllipse(this->center, sz.width()/5, sz.height()/5);

    QFont font("Times", 12);
    QString str = QString::number(this->timeout);
    QFontMetrics met(font);
    int w = met.width(str);
    int h = met.height();
    painter.setFont(font);
    painter.setPen(QColor(255, 0, 0));
    painter.drawText(center.x()-w/2, center.y()+h/2, str);

    // 画 东南西北
    font.setPixelSize(12);
    painter.setFont(font);
    painter.setPen(QColor(255, 250, 250));

    int x = 4 - zhuang;
    x %= 4;
    painter.drawText(drawFangWei[0], QString::fromLocal8Bit(fangwei[x]));

    x++;
    x %= 4;
    painter.rotate(90);
    painter.drawText(drawFangWei[1], QString::fromLocal8Bit(fangwei[x]));

    x++;
    x %= 4;
    painter.rotate(90);
    painter.drawText(drawFangWei[2], QString::fromLocal8Bit(fangwei[x]));

    x++;
    x %= 4;
    painter.rotate(90);
    painter.drawText(drawFangWei[3], QString::fromLocal8Bit(fangwei[x]));
}

void MJ_CLockWidget::timerSlot()
{
    this->timeout--;
    if(this->timeout <= 0)
    {
        this->tm->stop();
    }
    update();
}
