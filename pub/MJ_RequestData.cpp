#include "MJ_RequestData.h"

#include <cstring>

MJ_RequestData::MJ_RequestData(Type type, CARD c)
{
    this->ver = 100;
    this->type = type;
    this->cd = c;
    this->len = 9;

    memset(this->data, 0, DataSize);
    memset(this->chiList, 0, 8);
    memset(this->pengList, 0, 8);
    memset(this->gangList, 0, 8);
    memset(this->huList, 0, 8);
    memset(this->chi, 0, 3);
    this->hu = MJ_Base::MJ_noCard;
    this->gang = MJ_Base::MJ_noCard;
    this->peng = MJ_Base::MJ_noCard;
}

void MJ_RequestData::setType(MJ_RequestData::Type type)
{
    this->type = type;
}

void MJ_RequestData::addType(MJ_RequestData::Type type)
{
    this->type |= type;
}

void MJ_RequestData::setCard(MJ_Base::CARD c)
{
    this->cd = c;
}

void MJ_RequestData::setHGPCList(MJ_Base::pCCARD h, MJ_Base::pCCARD g, MJ_Base::pCCARD p, MJ_Base::pCCARD c)
{
    memcpy(this->chiList, c, 8);
    memcpy(this->pengList, p, 8);
    memcpy(this->gangList, g, 8);
    memcpy(this->huList, h, 8);

    this->len += 32;
}

void MJ_RequestData::setChi(MJ_Base::CARD chi[])
{
    this->chi[0] = chi[0];
    this->chi[1] = chi[1];
    this->chi[2] = chi[2];
    this->len += 3;
}

int MJ_RequestData::getData(char *dst)
{
    if(buf == nullptr)
        return 0;

    char *buf = dst;

    *(int *)(buf) = (int)(this->ver);
    buf += 4;

    *(int *)(buf) = (int)(this->type);
    buf += 4;

    *buf = (char)(this->cd);
    buf++;

    if(this->type & R_HGPCList)
    {
        memcpy(buf, this->huList, 8);
        buf += 8;
        memcpy(buf, this->gangList, 8);
        buf += 8;
        memcpy(buf, this->pengList, 8);
        buf += 8;
        memcpy(buf, this->chiList, 8);
        buf += 8;
    }
    if(this->type & R_Chi)
    {
        memcpy(buf, this->chi, 8);
        buf += 3;
    }

    return buf-dst;
}





