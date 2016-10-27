#include "MJ_widgetBase.h"

MJ_widgetBase *MJ_widgetBase::instance = nullptr;

MJ_widgetBase *MJ_widgetBase::getInstance()
{
    if(MJ_widgetBase::instance == nullptr)
        MJ_widgetBase::instance = new MJ_widgetBase();

    return MJ_widgetBase::instance;
}

MJ_widgetBase::~MJ_widgetBase()
{
    delete[] cards;

    delete bottom;
    delete top;
    delete left;
    delete right;
    delete bg;
    delete None;
//    delete bg_top;
//    delete bg_bottom;
//    delete bg_left;
//    delete bg_right;
}

const QPixmap *MJ_widgetBase::operator[](MJ_Base::CARD cd)
{
    if(isMJCARD(cd))
        return this->cards[cd];

    return this->None;
}

const QPixmap *MJ_widgetBase::operator[](MJ_widgetBase::BG_CARD cd)
{
    switch(cd)
    {
    case C_top:
        return this->top;
        break;
    case C_bottom:
        return this->bottom;
        break;
    case C_left:
        return this->left;
        break;
    case C_right:
        return this->right;
        break;
    case C_bg:
        return this->bg;
        break;
    case C_bg_top:
        return this->bg_top;
        break;
    case C_bg_bottom:
        return this->bg_bottom;
        break;
    case C_bg_left:
        return this->bg_left;
        break;
    case C_bg_right:
        return this->bg_right;
        break;
    default:
        return this->None;
    }
}

MJ_widgetBase::MJ_widgetBase()
{
    for(int i=0; i<128; i++)
        cards[i] = nullptr;

    cards[MJ_Base::MJ_YIWAN] = new QPixmap(":/w_1.png");
    cards[MJ_Base::MJ_ERWAN] = new QPixmap(":/w_2.png");
    cards[MJ_Base::MJ_SANWAN] = new QPixmap(":/w_3.png");
    cards[MJ_Base::MJ_SIWAN] = new QPixmap(":/w_4.png");
    cards[MJ_Base::MJ_WUWAN] = new QPixmap(":/w_5.png");
    cards[MJ_Base::MJ_LIUWAN] = new QPixmap(":/w_6.png");
    cards[MJ_Base::MJ_QIWAN] = new QPixmap(":/w_7.png");
    cards[MJ_Base::MJ_BAWAN] = new QPixmap(":/w_8.png");
    cards[MJ_Base::MJ_JIUWAN] = new QPixmap(":/w_9.png");

    cards[MJ_Base::MJ_YITIAO] = new QPixmap(":/tiao_1.png");
    cards[MJ_Base::MJ_ERTIAO] = new QPixmap(":/tiao_2.png");
    cards[MJ_Base::MJ_SANTIAO] = new QPixmap(":/tiao_3.png");
    cards[MJ_Base::MJ_SITIAO] = new QPixmap(":/tiao_4.png");
    cards[MJ_Base::MJ_WUTIAO] = new QPixmap(":/tiao_5.png");
    cards[MJ_Base::MJ_LIUTIAO] = new QPixmap(":/tiao_6.png");
    cards[MJ_Base::MJ_QITIAO] = new QPixmap(":/tiao_7.png");
    cards[MJ_Base::MJ_BATIAO] = new QPixmap(":/tiao_8.png");
    cards[MJ_Base::MJ_JIUTIAO] = new QPixmap(":/tiao_9.png");

    cards[MJ_Base::MJ_YITONG] = new QPixmap(":/tong_1.png");
    cards[MJ_Base::MJ_ERTONG] = new QPixmap(":/tong_2.png");
    cards[MJ_Base::MJ_SANTONG] = new QPixmap(":/tong_3.png");
    cards[MJ_Base::MJ_SITONG] = new QPixmap(":/tong_4.png");
    cards[MJ_Base::MJ_WUTONG] = new QPixmap(":/tong_5.png");
    cards[MJ_Base::MJ_LIUTONG] = new QPixmap(":/tong_6.png");
    cards[MJ_Base::MJ_QITONG] = new QPixmap(":/tong_7.png");
    cards[MJ_Base::MJ_BATONG] = new QPixmap(":/tong_8.png");
    cards[MJ_Base::MJ_JIUTONG] = new QPixmap(":/tong_9.png");

    cards[MJ_Base::MJ_DONG] = new QPixmap(":/dong.png");
    cards[MJ_Base::MJ_NAN] = new QPixmap(":/nan.png");
    cards[MJ_Base::MJ_XI] = new QPixmap(":/xi.png");
    cards[MJ_Base::MJ_BEI] = new QPixmap(":/bei.png");
    cards[MJ_Base::MJ_ZHONG] = new QPixmap(":/zhong.png");
    cards[MJ_Base::MJ_FA] = new QPixmap(":/fa.png");
    cards[MJ_Base::MJ_BAN] = new QPixmap(":/bai.png");

    this->bg = new QPixmap(":/bg.png");
    this->bottom = new QPixmap(":/bottom.png");
    this->top = new QPixmap(":/duimen.png");
    this->left = new QPixmap(":/shangjia.png");
    this->right = new QPixmap(":/xiajia.png");
    this->None = new QPixmap(48, 64);
//    this->bg_bottom = new QPixmap(*this->bg);
//    this->bg_left = new QPixmap(*this->left);
//    this->bg_right = new QPixmap(*this->C_bg_right);
//    this->bg_top = new QPixmap(*this->top);
}
