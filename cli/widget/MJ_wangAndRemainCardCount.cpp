#include "MJ_wangAndRemainCardCount.h"
#include "MJ_widgetBase.h"

#include <QPainter>
#include <QDebug>

MJ_wangAndRemainCardCount::MJ_wangAndRemainCardCount(QWidget *parent) : QWidget(parent)
{
    widgetBaseInstance = MJ_widgetBase::getInstance();
    this->card = MJ_Base::MJ_ZHONG;

    this->dis_paiCountBg = new QLabel(this);
    this->dis_paiCountBg->setPixmap(*widgetBaseInstance->operator [](MJ_widgetBase::C_top));
    this->dis_paiCountBg->setGeometry(this->width()/3, this->height() - 10,
                                    16, 20);

    this->dis_paiCount = new QLabel("0", this);
    this->dis_paiCount->setGeometry(this->width()/3+18, this->height() - 10,
                                    30, 20);
//    this->dis_paiCount->resize(this->width()/3, this->height()/2);
//    this->dis_paiCount->move();
}

void MJ_wangAndRemainCardCount::mj_setCard(MJ_Base::CARD cd)
{
    this->card = cd;
}

void MJ_wangAndRemainCardCount::mj_setPaiCount(int count)
{
    this->paiCount = count;
    this->dis_paiCount->setText(QString::number(paiCount));
}

int MJ_wangAndRemainCardCount::operator --(int)
{
    this->paiCount--;
    if(this->paiCount < 0)
        this->paiCount = 0;

    qDebug() << __FUNCTION__ << __LINE__ << paiCount;
    this->dis_paiCount->setText(QString::number(paiCount));

    return this->paiCount;
}

void MJ_wangAndRemainCardCount::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    const QPixmap *pix = widgetBaseInstance->operator [](MJ_widgetBase::C_bottom);
    QPixmap drawPix = pix->scaled(this->width()/2, this->height(), Qt::KeepAspectRatio);
    painter.drawPixmap(0, 0, drawPix);

    pix = widgetBaseInstance->operator [](card);
    drawPix = pix->scaled(this->width()/2, this->height(), Qt::KeepAspectRatio);
    painter.drawPixmap(0, 5, drawPix);
}
