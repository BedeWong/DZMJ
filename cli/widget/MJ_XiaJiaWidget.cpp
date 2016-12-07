#include "MJ_XiaJiaWidget.h"
#include "MJ_widgetBase.h"
#include "../../pub/MJ_otherPlayer.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QDebug>

MJ_XiaJiaWidget::MJ_XiaJiaWidget(QWidget *parent) : QWidget(parent)
{
    this->draw_newCard = false;
    pix_display = new QPixmap(DS_Width, DS_Height);
    pix_display->fill(QColor(0,0,0,100));  // ÖÆ×÷Í¸Ã÷

    WidgetBaseInstance = MJ_widgetBase::getInstance();

    this->setSize(QSize(DS_Width, DS_Height));
}

void MJ_XiaJiaWidget::setModel(MJ_otherPlayer *model)
{
    if(model == nullptr)
        throw(-1);

    this->dataModel = model;
}

void MJ_XiaJiaWidget::setSize(QSize sz)
{
    x_scale = sz.width() / qreal(DS_Width);
    y_scale = sz.height() / qreal(DS_Height);

    this->resize(sz);
}

void MJ_XiaJiaWidget::draw_NewCard(bool flag)
{
    this->draw_newCard = flag;
    update();
}

void MJ_XiaJiaWidget::paintEvent(QPaintEvent *)
{
    nextStartPoint = QPoint((this->size().width()-84)/2, DS_Height-5);
    this->pix_display->fill(QColor(0,0,0,0));

    QPainter painter(this->pix_display);
    draw_GPC(painter);
    draw_PaiList(painter);

    painter.end();

    QPixmap display = this->pix_display->scaled(DS_Width*x_scale, DS_Height*y_scale, Qt::KeepAspectRatio);

    painter.begin(this);
    painter.drawPixmap(0, 0, display);
    return ;
}

void MJ_XiaJiaWidget::draw_GPC(QPainter &painter)
{
    int gpc_seq[8] = {0};
    int gpc_count = 0;
    if((gpc_count = this->dataModel->getGPCseq(gpc_seq)) <= 0)
    {
        nextStartPoint -= QPoint(0, 60);
        return;
    }

    const QPixmap *pix_card;
    const QPixmap *c_bg = this->WidgetBaseInstance->operator [](MJ_widgetBase::C_bg);

    QMatrix mat;
    mat.rotate(270);
    QPixmap gpc_bg = c_bg->transformed(mat, Qt::SmoothTransformation);

    MJ_Base::CARD gang[8] = {0};
    MJ_Base::CARD peng[8] = {0};
    MJ_Base::CARD chi[16] = {0};
    int _g = 0;
    int _p = 0;
    int _c = 0;

    this->dataModel->getChi(chi);
    this->dataModel->getPeng(peng);
    this->dataModel->getGang(gang);

//    mat.reset();
//    mat.rotate(270);
    nextStartPoint -= QPoint(0, 60);
    for(int i=0; i<gpc_count; i++)
    {
        if(gpc_seq[i] == MJ_Base::SEQ_CHI)
        {
            pix_card = WidgetBaseInstance->operator [](chi[_c++]);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(0, 55);
            pix_card = WidgetBaseInstance->operator [](chi[_c++]);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(0, 55);
            pix_card = WidgetBaseInstance->operator [](chi[_c++]);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(0, 60);
        }
        else if(gpc_seq[i] == MJ_Base::SEQ_PENG)
        {
            pix_card = WidgetBaseInstance->operator [](peng[_p++]);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(0, 55);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(0, 55);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(0, 60);
        }
        else if(gpc_seq[i] == MJ_Base::SEQ_GANG)
        {
            pix_card = WidgetBaseInstance->operator [](gang[_g++]);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(0, 55);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(0, 55);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            QPoint gangCardPoint = nextStartPoint + QPoint(-10, 55);
            painter.drawPixmap(gangCardPoint, gpc_bg);
            painter.drawPixmap(gangCardPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(0, 60);
        }
    }
    //qDebug() << "nextStartPoint: " << nextStartPoint;
    //qDebug() << "MJ_XiaJiaWidget::draw_GPC";
}

void MJ_XiaJiaWidget::draw_PaiList(QPainter &painter)
{
    int paiCount = this->dataModel->getPaiList(nullptr);
    const QPixmap *shangjia = this->WidgetBaseInstance->operator [](MJ_widgetBase::C_right);

    //qDebug() << "nextStartPoint: " << nextStartPoint;
    nextStartPoint -= QPoint(0, 40) * paiCount;
    //qDebug() << shangjia->size() << "nextStartPoint: " << nextStartPoint;

    if(this->draw_newCard == true)
    {
        nextStartPoint -= QPoint(0, 100);
        painter.drawPixmap(nextStartPoint, *shangjia);
        nextStartPoint += QPoint(0, 100);
        //qDebug() << "MJ_XiaJiaWidget draw_newCard:" << draw_newCard << nextStartPoint;
    }

    for(int i=0; i<paiCount; i++)
    {
        painter.drawPixmap(nextStartPoint, *shangjia);
        nextStartPoint += QPoint(0, 40);
    }

    //qDebug() << "MJ_XiaJiaWidget pCount:" << paiCount;
    //qDebug() << "MJ_XiaJiaWidget::draw_PaiList";
}
