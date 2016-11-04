#include "MJ_gameOverWidget.h"

#include <QPainter>
#include <QDebug>

MJ_gameOverWidget::MJ_gameOverWidget(QWidget *parent) : QWidget(parent)
{
    this->okButton = new QPushButton(QString::fromLocal8Bit("确定"), this);
    this->view = new QListView(this);
    this->cdsWidget = new WinnerPaiListWidget(this);
    this->model = new QStandardItemModel(this);
    //this->winnerLabel = new QLabel(this);

    connect(this->okButton, &QPushButton::clicked, this, [&](bool x){
        model->clear();
        this->hide();
    });

    this->resize(400, 300);
    this->okButton->move(170, 260);
    this->view->move(40, 60);
    this->view->resize(320, 180);
    this->view->setModel(this->model);
    this->cdsWidget->resize(320, 60);
    this->cdsWidget->move(40, 15);
//    this->winnerLabel->resize(400, 30);
//    this->winnerLabel->move(0, 0);

    this->setObjectName(QString("gameOverWidget"));
    this->view->setObjectName(QString("listView"));
    this->okButton->setObjectName("gameOverOk");
//    this->winnerLabel->setObjectName("winnerLabel");
}

void MJ_gameOverWidget::setPai(MJ_Base::CARD *g, MJ_Base::CARD *p, MJ_Base::CARD *c, MJ_Base::CARD *pailist, MJ_Base::CARD card)
{
    this->cdsWidget->setPai(g, p, c, pailist, card);
}

void MJ_gameOverWidget::setFan(std::list<const char *> ll, int fan)
{
    for(auto it : ll)
    {
        this->model->appendRow(new QStandardItem(QString::fromLocal8Bit(it)));
    }
    this->model->appendRow(new QStandardItem(QString::fromLocal8Bit("共计%1番").arg(fan)));
}

void MJ_gameOverWidget::setWinner(QString winner)
{
    QString str(QString::fromLocal8Bit("%1 胡牌").arg(winner));
    //this->winnerLabel->setText(str);
}

void MJ_gameOverWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix(this->width(), this->height());
    pix.fill(QColor(0, 0, 0, 100));

    painter.drawPixmap(0,0,pix);
}


/****************************************************
 *         class WinnerPaiListWidget
 */
#include <QPainter>
WinnerPaiListWidget::WinnerPaiListWidget(QWidget *parent) : QWidget(parent)
{
    this->WidgetBaseInstance = MJ_widgetBase::getInstance();
    this->pix = new QPixmap(800, 84);
    memset(this->g, 0, 8);
    memset(this->p, 0, 8);
    memset(this->c, 0, 16);
    memset(this->pailist, 0, 16);
}

void WinnerPaiListWidget::setPai(MJ_Base::CARD *g, MJ_Base::CARD *p, MJ_Base::CARD *c, MJ_Base::CARD *pailist, MJ_Base::CARD card)
{
    memset(this->g, 0, 8);
    memset(this->p, 0, 8);
    memset(this->c, 0, 16);
    memset(this->pailist, 0, 16);

    memcpy(this->g, g, 8);
    memcpy(this->p, p, 8);
    memcpy(this->c, c, 16);
    memcpy(this->pailist, pailist, 16);
    this->card = card;

    qDebug() << g << p << c << pailist << card;
    update();
}

void WinnerPaiListWidget::paintEvent(QPaintEvent *)
{
    this->pix->fill(QColor(0,0,0,0));
    QPainter painter(pix);

    const QPixmap *gpc_bg = this->WidgetBaseInstance->operator [](MJ_widgetBase::C_bg);
    const QPixmap *pix_card;
    QPoint nextStartPoint(0,0);
    for(int i=0; i<strlen(this->g); i++)
    {
        qDebug() << ":" << i;
        pix_card = WidgetBaseInstance->operator [](g[i]);
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

    for(int i=0; i<strlen(this->p); i++)
    {
        pix_card = WidgetBaseInstance->operator [](p[i]);
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

    for(int i=0; i<strlen(this->c); i++)
    {
        pix_card = WidgetBaseInstance->operator [](c[i]);
        painter.drawPixmap(nextStartPoint, *gpc_bg);
        painter.drawPixmap(nextStartPoint, *pix_card);
        nextStartPoint += QPoint(55, 0);
    }
    nextStartPoint += QPoint(5, 0);

    for(int i=0; i<strlen(this->pailist); i++)
    {
        pix_card = WidgetBaseInstance->operator [](pailist[i]);
        painter.drawPixmap(nextStartPoint, *gpc_bg);
        painter.drawPixmap(nextStartPoint, *pix_card);
        nextStartPoint += QPoint(55, 0);
    }
    nextStartPoint += QPoint(10, 0);
    pix_card = WidgetBaseInstance->operator [](card);
    painter.drawPixmap(nextStartPoint, *gpc_bg);
    painter.drawPixmap(nextStartPoint, *pix_card);

    painter.end();

    painter.begin(this);

    QPixmap draw = this->pix->scaled(this->width(), this->height(),Qt::KeepAspectRatio);
    painter.drawPixmap(0, 0, draw);
}
