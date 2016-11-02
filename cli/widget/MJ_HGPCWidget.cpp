#include "MJ_HGPCWidget.h"
#include "../MJ_Desktop.h"

#include <QPainter>
#include <QBitmap>
#include <QDebug>

MJ_HGPCWidget::MJ_HGPCWidget(QSize _sz, QWidget *parent) : QWidget(parent)
{
    this->selectChiWidget = nullptr;

    this->HuButton = new QPushButton(QString::fromLocal8Bit("ºú"),this);
    this->GangButton = new QPushButton(QString::fromLocal8Bit("¸Ü"), this);
    this->PengButton = new QPushButton(QString::fromLocal8Bit("Åö"), this);
    this->ChiButton = new QPushButton(QString::fromLocal8Bit("³Ô"), this);
    this->CancelButton = new QPushButton(QString::fromLocal8Bit("¹ý"), this);

    this->resize(_sz);

    connect(this->HuButton, SIGNAL(clicked(bool)), this, SLOT(HuClicked(bool)));
    connect(this->GangButton, SIGNAL(clicked(bool)), this, SLOT(GangClicked(bool)));
    connect(this->PengButton, SIGNAL(clicked(bool)), this, SLOT(PengClicked(bool)));
    connect(this->ChiButton, SIGNAL(clicked(bool)), this, SLOT(ChiClicked(bool)));
    connect(this->CancelButton, SIGNAL(clicked(bool)), this, SLOT(CancelClicked(bool)));

    this->ChiButton->setObjectName("chiButton");
    this->ChiButton->installEventFilter(this);

    this->hide();
}

MJ_HGPCWidget::~MJ_HGPCWidget()
{
    if(this->selectChiWidget)
        delete this->selectChiWidget;
}

void MJ_HGPCWidget::setChiList(const MJ_Base::CARD (*ll)[4], int nCount)
{
    if(this->selectChiWidget != nullptr)
    {
        delete this->selectChiWidget;
        this->selectChiWidget = nullptr;
    }

    this->result = 0;
    this->selectChiWidget = new SelectChiwidget(nCount, this->pos(),
                                                this->parentWidget());
    connect(this->selectChiWidget, SIGNAL(finishSignal()), this, SLOT(selectChiFinished()));

    ChiwidgetItem *newItem;
    for(auto i=0; i<nCount; i++)
    {
        newItem = new ChiwidgetItem(ll[i], this->selectChiWidget);
        this->selectChiWidget->addItem(newItem);
        qDebug() << "\tHGPCWidget::setChiList :" << ll[i];
    }
}

int MJ_HGPCWidget::getResult() const
{
    return result;
}

bool MJ_HGPCWidget::getChiSelected(MJ_Base::CARD *cds) const
{
    if(cds == nullptr)
        return false;

    memcpy(cds, this->selectChi, 4);

    return true;
}

void MJ_HGPCWidget::resizeEvent(QResizeEvent *event)
{
    QSize sz = event->size();
    this->HuButton->setGeometry(0, 0, sz.width()/5, sz.height());
    this->GangButton->setGeometry(sz.width()/5, 0,
                                  sz.width()/5, sz.height());
    this->PengButton->setGeometry(sz.width()*2/5, 0,
                                  sz.width()/5, sz.height());
    this->ChiButton->setGeometry(sz.width()*3/5, 0,
                                  sz.width()/5, sz.height());
    this->CancelButton->setGeometry(sz.width()*4/5, 0,
                                    sz.width()/5, sz.height());
}

bool MJ_HGPCWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched->objectName() == QString("chiButton") && event->type() == QEvent::Enter)
    {
        //this->selectChiWidget->show();
    }
    else if(watched->objectName() == QString("chiButton") && event->type() == QEvent::Leave)
    {
        //this->selectChiWidget->hide();
    }

    return false;
}

void MJ_HGPCWidget::hgpc_show(int stat)
{
    if((stat & MJ_Desktop::S_HU) || (stat & MJ_Desktop::S_ZiMo))
        this->HuButton->setDisabled(false);
    else
        this->HuButton->setDisabled(true);

    if((stat & MJ_Desktop::S_GANG) || (stat & MJ_Desktop::S_BuGang))
        this->GangButton->setDisabled(false);
    else
        this->GangButton->setDisabled(true);

    if(stat & MJ_Desktop::S_PENG)
        this->PengButton->setDisabled(false);
    else
        this->PengButton->setDisabled(true);

    if(stat & MJ_Desktop::S_CHI)
        this->ChiButton->setDisabled(false);
    else
        this->ChiButton->setDisabled(true);

    this->show();
}

void MJ_HGPCWidget::HuClicked(bool)
{
    if(selectChiWidget != nullptr)
    {
        delete this->selectChiWidget;
        this->selectChiWidget = nullptr;
    }
    this->hide();
    result = RES_HU;
    emit hgpc_finished();
}

void MJ_HGPCWidget::GangClicked(bool)
{
    if(selectChiWidget != nullptr)
    {
        delete this->selectChiWidget;
        this->selectChiWidget = nullptr;
    }
    this->hide();
    result = RES_GANG;
    emit hgpc_finished();
}

void MJ_HGPCWidget::ChiClicked(bool)
{
    if(this->selectChiWidget != nullptr)
        this->selectChiWidget->show();

    return;
}

void MJ_HGPCWidget::selectChiFinished()
{
    this->hide();
    this->selectChiWidget->getSelected(this->selectChi);
    if(selectChiWidget != nullptr)
    {
        delete this->selectChiWidget;
        this->selectChiWidget = nullptr;
    }
    result = RES_CHI;
    emit hgpc_finished();
}

void MJ_HGPCWidget::PengClicked(bool)
{
    if(selectChiWidget != nullptr)
    {
        delete this->selectChiWidget;
        this->selectChiWidget = nullptr;
    }
    this->hide();
    result = RES_PENG;
    emit hgpc_finished();
}

void MJ_HGPCWidget::CancelClicked(bool)
{
    if(selectChiWidget != nullptr)
    {
        delete this->selectChiWidget;
        this->selectChiWidget = nullptr;
    }
    this->hide();
    result = RES_CANCEL;
    emit hgpc_finished();
}

/*******  *****   *****
    * select widget  *
*/
SelectChiwidget::SelectChiwidget(int nCount, QPoint pt, QWidget *parent) : QWidget(parent)
{
    qDebug() << parent->pos();

    //this->setWindowFlags(Qt::Popup);
    QSize sz(200, nCount * 84);

    this->setGeometry(pt.x()/* + (sz.width()-pt.x()) /2*/, pt.y() - sz.height() - 20, sz.width(), sz.height());

    this->nCount = 0;
}

void SelectChiwidget::addItem(ChiwidgetItem *item)
{
    int x = (this->size().width() - 160)/2;
    this->items[nCount] = item;
    item->move(x, nCount * 84);

    this->nCount++;
}

void SelectChiwidget::getSelected(MJ_Base::CARD *cds) const
{
    if(cds == nullptr)
        return;

    memcpy(cds, this->selected, 4);
}

void SelectChiwidget::ItemClickedSlot(MJ_Base::CARD *cds)
{
    memcpy(this->selected, cds, 4);
    this->hide();

    emit finishSignal();
}


// item widget
ChiwidgetItem::ChiwidgetItem(const MJ_Base::CARD cds[], SelectChiwidget *parent) : QWidget(parent)
{
    memcpy(this->cards, cds, 4);
    this->resize(QSize(3*53, 84));

    connect(this, SIGNAL(clickedSignal(MJ_Base::CARD*)), parent, SLOT(ItemClickedSlot(MJ_Base::CARD*)));

    this->draw_rect = false;
}

void ChiwidgetItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    const QPixmap *pix_bg, *pix_card;
    pix_bg = MJ_widgetBase::getInstance()->operator [](MJ_widgetBase::C_bottom);

    pix_card = MJ_widgetBase::getInstance()->operator [](this->cards[0]);
    painter.drawPixmap(0, 0, *pix_bg);
    painter.drawPixmap(0, 20, *pix_card);

    pix_card = MJ_widgetBase::getInstance()->operator [](this->cards[1]);
    painter.drawPixmap(53, 0, *pix_bg);
    painter.drawPixmap(53, 20, *pix_card);

    pix_card = MJ_widgetBase::getInstance()->operator [](this->cards[2]);
    painter.drawPixmap(106, 0, *pix_bg);
    painter.drawPixmap(106, 20, *pix_card);

    if(draw_rect)
    {
        QPen pen(QBrush(QColor(250, 40, 40)), 4);
        painter.setPen(pen);
        painter.drawRect(2, 2, 52*3, 80);
    }
}

bool ChiwidgetItem::event(QEvent *e)
{
    if(e->type() == QEvent::Enter)
    {
        draw_rect = true;
        update();
    }
    else if(e->type() == QEvent::Leave)
    {
        draw_rect = false;
        update();
    }

    return QWidget::event(e);
}


void ChiwidgetItem::mousePressEvent(QMouseEvent *)
{
    emit clickedSignal(this->cards);
}
