#include "MJ_DuiMenWidget.h"
#include "MJ_widgetBase.h"
#include "../../pub/MJ_otherPlayer.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QDebug>

MJ_DuiMenWidget::MJ_DuiMenWidget(QWidget *parent) : QWidget(parent)
{
    this->draw_newCard = false;
    this->pix_display = new QPixmap(DS_Width, DS_Height);
    this->pix_display->fill(QColor(0,0,0,100));

    WidgetBaseInstance = MJ_widgetBase::getInstance();

    this->setSize(QSize(DS_Width, DS_Height));
}

void MJ_DuiMenWidget::setModel(MJ_otherPlayer *model)
{
    if(model != nullptr)
        this->dataModel = model;
    return ;
}

void MJ_DuiMenWidget::setSize(QSize sz)
{
    x_scale = sz.width()/qreal(DS_Width);
    y_scale = sz.height()/qreal(DS_Height);

    this->resize(sz);
}

void MJ_DuiMenWidget::draw_NewCard(bool flag)
{
    draw_newCard = flag;
    update();
}

void MJ_DuiMenWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this->pix_display);
    this->pix_display->fill(QColor(0,0,0,0));

    nextStartPoint = QPoint(DS_Width-5, (DS_Height-84)/2);
    draw_GPC(painter);
    draw_PaiList(painter);

    painter.end();

    QPixmap display = this->pix_display->scaled(DS_Width*x_scale, DS_Height*y_scale, Qt::KeepAspectRatio);

    painter.begin(this);
    painter.drawPixmap(0, 0, display);
    return ;
}

void MJ_DuiMenWidget::draw_GPC(QPainter &painter)
{
    int gpc_seq[8] = {0};
    int gpc_count = 0;
    if((gpc_count = this->dataModel->getGPCseq(gpc_seq)) <= 0)
    {
        nextStartPoint -= QPoint(60, 0);
        return;
    }

    const QPixmap *pix_card;
    const QPixmap *c_bg = this->WidgetBaseInstance->operator [](MJ_widgetBase::C_bg);

    QMatrix mat;
    //mat.rotate(180);
    QPixmap gpc_bg = c_bg->transformed(mat, Qt::SmoothTransformation);

    MJ_Base::CARD gang[8] = {0};
    MJ_Base::CARD peng[8] = {0};
    MJ_Base::CARD chi[16] = {0};
    int gCount, _g = 0;
    int pCount, _p = 0;
    int cCount, _c = 0;

    cCount = this->dataModel->getChi(chi);
    pCount = this->dataModel->getPeng(peng);
    gCount = this->dataModel->getGang(gang);

    for(int i=0; i<gpc_count; i++)
    {
        if(gpc_seq[i] == MJ_Base::SEQ_CHI)
        {
            pix_card = WidgetBaseInstance->operator [](chi[_c++]);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(55, 0);
            pix_card = WidgetBaseInstance->operator [](chi[_c++]);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(55, 0);
            pix_card = WidgetBaseInstance->operator [](chi[_c++]);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(60, 0);
        }
        else if(gpc_seq[i] == MJ_Base::SEQ_PENG)
        {
            pix_card = WidgetBaseInstance->operator [](peng[_p++]);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(55, 0);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(55, 0);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(60, 0);
        }
        else if(gpc_seq[i] == MJ_Base::SEQ_GANG)
        {
            pix_card = WidgetBaseInstance->operator [](gang[_g++]);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(55, 0);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(55, 0);
            painter.drawPixmap(nextStartPoint, gpc_bg);
            painter.drawPixmap(nextStartPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            QPoint gangCardPoint = nextStartPoint + QPoint(55, -10);
            painter.drawPixmap(gangCardPoint, gpc_bg);
            painter.drawPixmap(gangCardPoint, pix_card->transformed(mat, Qt::SmoothTransformation));

            nextStartPoint -= QPoint(60, 0);
        }
    }

    //qDebug() << "MJ_DuiMenWidget::draw_HGP" << endl;
}

void MJ_DuiMenWidget::draw_PaiList(QPainter &painter)
{
    int paiCount = this->dataModel->getPaiList(nullptr);
    const QPixmap *shangjia = this->WidgetBaseInstance->operator [](MJ_widgetBase::C_top);

    //nextStartPoint -= QPoint(48, 0) * paiCount;

    for(int i=0; i<paiCount; i++)
    {
        painter.drawPixmap(nextStartPoint, *shangjia);
        nextStartPoint -= QPoint(48, 0);
    }

    if(this->draw_newCard == true)
    {
        nextStartPoint -= QPoint(48, 0);
        painter.drawPixmap(nextStartPoint, *shangjia);
        //qDebug() << "MJ_DuiMenWidget::draw_newCard " << draw_newCard << nextStartPoint;
    }

    //qDebug() << "MJ_DuiMenWidget::paiCount " << paiCount;
    //qDebug() << "MJ_DuiMenWidget::draw_PaiList ";
}
