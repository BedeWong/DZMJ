#include "MJ_selfWidget.h"
#include "MJ_widgetBase.h"
#include "../../pub/MJ_Base.h"

#include <QDebug>
#include <QPainter>
#include <QEvent>

MJ_selfWidget::MJ_selfWidget(QWidget *parent) : QWidget(parent)
{
    this->pix_display = new QPixmap(DS_Width, DS_Height);
    this->pix_display->fill(QColor(0,0,0,100));

    for(int i=0; i<13; i++)
    {
        MJ_SelfWidgetItem *item = new MJ_SelfWidgetItem(MJ_Base::MJ_noCard, this);
        connect(item, SIGNAL(ItemClicked(MJ_Base::CARD)), this, SLOT(itemClicked(MJ_Base::CARD)), Qt::QueuedConnection);
        connect(item, &MJ_SelfWidgetItem::redraw, this, &draw_PaiList, Qt::QueuedConnection);
        this->items.push_back(item);
    }
    this->newCard = new MJ_SelfWidgetItem(MJ_Base::MJ_noCard, this);
    connect(newCard, SIGNAL(ItemClicked(MJ_Base::CARD)), this, SLOT(itemClicked(MJ_Base::CARD)), Qt::QueuedConnection);

    WidgetBaseInstance = MJ_widgetBase::getInstance();

    this->dataModel = nullptr;
    this->gpc_need_redraw = true;
    this->draw_newCard = false;

    this->setSize(QSize(DS_Width, DS_Height));

    //qDebug() << "MJ_selfWidget construction ok";
}

void MJ_selfWidget::setModel(MJ_Player *model)
{
    this->dataModel = model;
}

void MJ_selfWidget::setSize(QSize sz)
{
    x_scale = sz.width() / qreal(DS_Width);
    y_scale = sz.height() / qreal(DS_Height);

    this->resize(sz);
}

void MJ_selfWidget::paintEvent(QPaintEvent *)
{
    nextStartPoint = QPoint(5, (DS_Height-84)/2);

    QPainter painter(this->pix_display);
    this->pix_display->fill(QColor(0,0,0,0));

    if(gpc_need_redraw)
    {
        //gpc_need_redraw = false;
        draw_GPC(painter);
    }

    painter.end();
    QPixmap display = this->pix_display->scaled(DS_Width*x_scale, DS_Height*y_scale, Qt::KeepAspectRatio);

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //painter.shear(-0.1, 0);
    painter.drawPixmap(0, 0, display);
    return ;
}

void MJ_selfWidget::draw_GPC(QPainter &painter)
{
    int gpc_seq[8] = {0};
    int gpc_count = 0;
    if(this->dataModel == nullptr)
    {
        return ;
    }
    if((gpc_count = this->dataModel->getGPCseq(gpc_seq)) <= 0)
    {
        return;
    }

    const QPixmap *pix_card;
    const QPixmap *gpc_bg = this->WidgetBaseInstance->operator [](MJ_widgetBase::C_bg);

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
    for(int i=0; i<gpc_count; i++)
    {
        if(gpc_seq[i] == MJ_Base::SEQ_CHI)
        {
            pix_card = WidgetBaseInstance->operator [](chi[_c++]);
            painter.drawPixmap(nextStartPoint, *gpc_bg);
            painter.drawPixmap(nextStartPoint, *pix_card);

            nextStartPoint += QPoint(55, 0);
            pix_card = WidgetBaseInstance->operator [](chi[_c++]);
            painter.drawPixmap(nextStartPoint, *gpc_bg);
            painter.drawPixmap(nextStartPoint, *pix_card);

            nextStartPoint += QPoint(55, 0);
            pix_card = WidgetBaseInstance->operator [](chi[_c++]);
            painter.drawPixmap(nextStartPoint, *gpc_bg);
            painter.drawPixmap(nextStartPoint, *pix_card);

            nextStartPoint += QPoint(60, 0);
        }
        else if(gpc_seq[i] == MJ_Base::SEQ_PENG)
        {
            pix_card = WidgetBaseInstance->operator [](peng[_p++]);
            painter.drawPixmap(nextStartPoint, *gpc_bg);
            painter.drawPixmap(nextStartPoint, *pix_card);

            nextStartPoint += QPoint(55, 0);
            painter.drawPixmap(nextStartPoint, *gpc_bg);
            painter.drawPixmap(nextStartPoint, *pix_card);

            nextStartPoint += QPoint(55, 0);
            painter.drawPixmap(nextStartPoint, *gpc_bg);
            painter.drawPixmap(nextStartPoint, *pix_card);

            nextStartPoint += QPoint(60, 0);
        }
        else if(gpc_seq[i] == MJ_Base::SEQ_GANG)
        {
            pix_card = WidgetBaseInstance->operator [](gang[_g++]);
            painter.drawPixmap(nextStartPoint, *gpc_bg);
            painter.drawPixmap(nextStartPoint, *pix_card);

            nextStartPoint += QPoint(55, 0);
            painter.drawPixmap(nextStartPoint, *gpc_bg);
            painter.drawPixmap(nextStartPoint, *pix_card);

            nextStartPoint += QPoint(55, 0);
            painter.drawPixmap(nextStartPoint, *gpc_bg);
            painter.drawPixmap(nextStartPoint, *pix_card);

            QPoint gangCardPoint = nextStartPoint - QPoint(55, 10);
            painter.drawPixmap(gangCardPoint, *gpc_bg);
            painter.drawPixmap(gangCardPoint, *pix_card);

            nextStartPoint += QPoint(60, 0);
        }
    }

    //qDebug() << "MJ_selfWidget::draw_GPC" << endl;
}

void MJ_selfWidget::itemClicked(MJ_Base::CARD cd)
{
    //qDebug() << "````selfWidget::itemClicked";
    emit retClicked(cd);
}

void MJ_selfWidget::draw_PaiList()
{
    //qDebug() << "\"draw_PaiList nextStartPoint:" <<nextStartPoint;
    MJ_Base::CARD pailist[16] = {0};
    int pCount = this->dataModel->getPaiList(pailist);

    for(int i=0; i<pCount; i++)
    {
        if(pailist[i] == this->wang)
        {
            for(int j=i; j>0; j--)
                pailist[j] = pailist[j-1];
            pailist[0] = this->wang;
            continue;
        }
        /*  给红中定位置   */
        else if(pailist[i] == MJ_Base::MJ_ZHONG)
        {
            int j = i;
            for(; j>0; j--)
            {
                if(pailist[j] > this->wang)
                {
                    pailist[j+1] = MJ_Base::MJ_ZHONG;
                    break;
                }
                else
                    pailist[j] = pailist[j-1];
            }
            if(j < 0)
            pailist[0] = MJ_Base::MJ_ZHONG;
        }
    }

    int gpc_count = this->dataModel->getGPCseq(nullptr);
    nextStartPoint = QPoint(gpc_count*160*x_scale*y_scale + 10, DS_Height-84);

    for(int i=0; i<13; i++)
    {
        if(i < pCount)
        {
            this->items[i]->move(this->nextStartPoint);
            this->items[i]->setCard(pailist[i]);
            this->items[i]->setSize(55*x_scale, 84*y_scale);
            this->items[i]->show();

            nextStartPoint += QPoint(50*x_scale, 0);
        }
        else
        {
            this->items[i]->setCard(MJ_Base::MJ_noCard);
        }
    }

    if(draw_newCard == true)
    {
        MJ_Base::CARD cd;
        this->dataModel->getNewCard(cd);
        this->newCard->setCard(cd);
        this->newCard->move(nextStartPoint + QPoint(30*x_scale*y_scale, 0));
        this->newCard->setSize(55*x_scale, 84*y_scale);
        this->newCard->show();
    }
    else{
        this->newCard->setCard(MJ_Base::MJ_noCard);
        this->newCard->hide();
    }

    //qDebug() << "MJ_selfWidget pCount:" << pCount;
    //qDebug() << "MJ_selfWidget::draw_PaiList" << endl;
}

void MJ_selfWidget::draw_NewCard(bool flag)
{
    this->draw_newCard = flag;
    if(!flag)
        this->newCard->setCard(MJ_Base::MJ_noCard);
}

void MJ_selfWidget::setWang(MJ_Base::CARD wang)
{
    this->wang = wang;
}


/*********************************************
 *      SelfWidget  的项 *
 */
MJ_SelfWidgetItem::MJ_SelfWidgetItem(MJ_Base::CARD cd, QWidget *parent) : QWidget(parent)
{
    this->card = cd;
    WidgetBaseInstance = MJ_widgetBase::getInstance();

    if(this->card == MJ_Base::MJ_noCard)
        this->hide();

    this->setObjectName("MyCardItem");
}

MJ_SelfWidgetItem::MJ_SelfWidgetItem(const MJ_SelfWidgetItem &t)
{
    this->card = t.card;
    this->WidgetBaseInstance = t.WidgetBaseInstance;
    this->resize(t.size());

    if(this->card == MJ_Base::MJ_noCard)
        this->hide();

    this->setParent(t.parentWidget());
    this->setObjectName("MyCardItem");
}

void MJ_SelfWidgetItem::setCard(MJ_Base::CARD cd)
{
    this->card = cd;
    if(this->card == MJ_Base::MJ_noCard)
    {
        this->hide();
    }
    else
    {
        this->show();
    }
}

void MJ_SelfWidgetItem::setSize(int x, int y)
{
    this->x_scale = x/55.0;
    this->y_scale = y/84.0;

    this->resize(x, y);
}

void MJ_SelfWidgetItem::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    const QPixmap *pix_bg = this->WidgetBaseInstance->operator [](MJ_widgetBase::C_bottom);
    const QPixmap *pix_card = this->WidgetBaseInstance->operator [](this->card);

    QPixmap bg = pix_bg->scaled(55*x_scale, 84*y_scale, Qt::KeepAspectRatio);
    QPixmap cd = pix_card->scaled(48*x_scale, 64*y_scale, Qt::KeepAspectRatio);

    painter.drawPixmap(0,0, bg);
    painter.drawPixmap(0, 18 * x_scale * y_scale, cd);

    //qDebug() << "--MJ_SelfWidgetItem::paintEvent--" << endl;
    //qDebug() << bg.size() << cd.size() << this->size();
}

void MJ_SelfWidgetItem::mousePressEvent(QMouseEvent *)
{
    isClicked = true;
}

void MJ_SelfWidgetItem::mouseReleaseEvent(QMouseEvent *)
{
    if(isClicked)
    {
        emit ItemClicked(this->card);
        isClicked = false;
    }
}


bool MJ_SelfWidgetItem::event(QEvent *e)
{
    if(e->type() == QEvent::Enter)
    {
        QPoint topLeftPoint = this->mapToParent(QPoint(0,0));

        this->move(topLeftPoint - QPoint(0, 10));
    }
    else if(e->type() == QEvent::Leave)
    {
        QPoint topLeftPoint = this->mapToParent(QPoint(0,0));

        if(topLeftPoint.y() == 6)
            this->move(topLeftPoint + QPoint(0, 10));
    }

    return QWidget::event(e);
}

