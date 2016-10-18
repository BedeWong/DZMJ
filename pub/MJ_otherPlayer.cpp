#include "MJ_otherPlayer.h"
#include <cstring>

MJ_otherPlayer::MJ_otherPlayer()
{

}

int MJ_otherPlayer::Gang(MJ_Base::CARD c)
{
    this->gang[this->_g++] = c;

    this->paiCount -= 2;

    return 0;
}

int MJ_otherPlayer::Peng(MJ_Base::CARD c)
{
    this->peng[this->_p++] = c;

    this->paiCount -= 2;

    return 0;
}

int MJ_otherPlayer::Chi(CARD c, MJ_Base::pCCARD ll)
{
    this->chi[_c++] = ll[0];
    this->chi[_c++] = ll[1];
    this->chi[_c++] = ll[2];

    this->paiCount -= 2;

    return 0;
}

int MJ_otherPlayer::Hu(MJ_Base::CARD c, MJ_Base::pCCARD plist)
{
    this->NewCard = c;
    memcpy(this->paiList, plist, this->paiCount);

    return 0;
}

int MJ_otherPlayer::ChuPai(MJ_Base::CARD c)
{
    this->paiRecList[this->paiRecCount++] = c;

    return 0;
}
