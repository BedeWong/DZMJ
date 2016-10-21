#include "MJ_Base.h"

MJ_Base::MJ_Base()
{

}

void MJ_Base::init()
{
    paiRecCount = 0;
    memset(paiRecList, 0, 50);
    memset(paiList, 0, 16);
    memset(gang, 0, 8);
    memset(peng, 0, 8);
    memset(chi, 0, 16);
    NewCard = MJ_noCard;

    _g = 0;
    _p = 0;
    _c = 0;
    _h = 0;
}

MJ_Base::~MJ_Base()
{

}
