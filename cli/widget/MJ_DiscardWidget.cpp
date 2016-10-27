#include "MJ_DiscardWidget.h"

#include <QDebug>

MJ_DiscardWidget::MJ_DiscardWidget(QWidget *parent) : QWidget(parent)
{
    this->count = 0;
    memset(this->cardSet, 0, sizeof(this->cardSet));

    MJcard = MJ_widgetBase::getInstance();
    this->display = new QPixmap(Size_x, Size_y);
    this->display->fill(QColor(0, 0, 0, 0));
    this->resize(Size_x, Size_y);
}

void MJ_DiscardWidget::addCard(MJ_Base::CARD cd)
{
    this->cardSet[this->count++] = cd;

    QPainter painter(this->display);
    const QPixmap *pix_bg = this->MJcard->operator [](MJ_widgetBase::C_bg);
    const QPixmap *pix_card;
    for(int i=0; i<this->count; i++)
    {
        pix_card = this->MJcard->operator [](this->cardSet[i]);
        painter.drawPixmap(i%10*53, i/10*64, *pix_bg);
        painter.drawPixmap(i%10*53, i/10*64, *pix_card);
    }
    update();
}

void MJ_DiscardWidget::undo()
{
    this->count--;

    QPainter painter(this->display);
    const QPixmap *pix_bg = this->MJcard->operator [](MJ_widgetBase::C_bg);
    const QPixmap *pix_card;

    for(int i=0; i<this->count; i++)
    {
        pix_card = this->MJcard->operator [](this->cardSet[i]);
        painter.drawPixmap(i%10*53, i/10*64, *pix_bg);
        painter.drawPixmap(i%10*53, i/10*64, *pix_card);
    }
    update();
}

void MJ_DiscardWidget::setId(int _id)
{
    this->ID = _id;
}

void MJ_DiscardWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QMatrix mat;
    mat.rotate(this->ID * 90);

    QPixmap draw = this->display->transformed(mat, Qt::SmoothTransformation);
    draw = draw.scaled(Size_x*x_scaled, Size_y*y_scaled);

    painter.drawPixmap(0, 0, draw);

}

void MJ_DiscardWidget::resizeEvent(QResizeEvent *event)
{
    sz = event->size();
    x_scaled = sz.width() / qreal(Size_x);
    y_scaled = sz.height() / qreal(Size_y);
}
