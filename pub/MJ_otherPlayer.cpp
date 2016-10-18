#include "MJ_otherPlayer.h"
#include <cstring>

MJ_otherPlayer::MJ_otherPlayer()
{

}

int MJ_otherPlayer::Gang(MJ_otherPlayer::CARD c)
{
    this->gang[this->_g++] = c;
    return 0;
}

int MJ_otherPlayer::Peng(MJ_otherPlayer::CARD c)
{
    this->peng[this->_p++] = c;
    return 0;
}

int MJ_otherPlayer::Chi(MJ_otherPlayer::CARD c[])
{
    this->chi[_c++] = c[0];
    this->chi[_c++] = c[1];
    this->chi[_c++] = c[2];

    return 0;
}

int MJ_otherPlayer::Hu(MJ_otherPlayer::CARD c, MJ_otherPlayer::pCCARD plist)
{
    this->NewCard = c;
    memcpy(this->paiList, plist, this->paiCount);
}

int MJ_otherPlayer::ChuPai(MJ_otherPlayer::CARD c)
{
    this->paiRecList[this->paiRecCount++] = c;
    this->paiCount--;
}
