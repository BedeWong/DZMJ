#include "MJ_RequestData.h"

#include <cstring>

MJ_RequestData::MJ_RequestData()
{
    // QMetaType
//    static void *Construct(void *where, const void *t)
//    {
//        if (t)
//            return new (where) T(*static_cast<const T*>(t));
//        return new (where) T;
//    }
}

MJ_RequestData::MJ_RequestData(int id, Type type, MJ_Base::CARD c)
{
    this->ver = 100;
    this->type = type;
    this->senderID = id;
    this->cd = c;
    this->len = 13;

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

void MJ_RequestData::setSenderID(int id)
{
    this->senderID = id;
    this->len += 4;
}

int MJ_RequestData::getData(char *dst)
{
    if(dst == nullptr)
        return 0;

    char *buf = dst;

    *(int *)(buf) = (int)(this->ver);
    buf += 4;

    *(int *)(buf) = (int)(this->type);
    buf += 4;

    *(int *)(buf) = (int)(this->senderID);
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
        memcpy(buf, this->chi, 3);
        buf += 3;
    }

    return buf-dst;
}

int MJ_RequestData::getType() const
{
    return this->type;
}

int MJ_RequestData::getHGPCList(MJ_Base::CARD *h, MJ_Base::CARD *g, MJ_Base::CARD *p, MJ_Base::CARD *c) const
{
    if(h == nullptr || g==nullptr || p==nullptr || c==nullptr)
        return -1;

    if(this->type & R_HGPCList == 0)
        return -1;

    memcpy(h, this->huList, 8);
    memcpy(g, this->gangList, 8);
    memcpy(p, this->pengList, 8);
    memcpy(c, this->chiList, 8);

    return 0;
}

int MJ_RequestData::getChi(MJ_Base::CARD chi[]) const
{
    if(chi == nullptr)
        return -1;

    if(this->type & R_Chi)
    {
        memcpy(chi, this->chi, 3);
    }

    return 0;
}

MJ_Base::CARD MJ_RequestData::getCard() const
{
    return this->cd;
}

int MJ_RequestData::getSenderID() const
{
    return this->senderID;
}





