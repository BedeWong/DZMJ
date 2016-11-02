#include "MJ_response.h"

MJ_response::MJ_response(int sendTo, VER v)
{
    this->SendTo = sendTo;
    this->ver = v;
    this->len = 12;
}

void MJ_response::addType(MJ_response::Type t)
{
    int n = this->type;
    n |= t;
    this->type = static_cast<MJ_response::Type>(n);
}

void MJ_response::setType(MJ_response::Type t)
{
    this->type = t;
}

void MJ_response::setVer(MJ_response::VER v)
{
    this->ver = v;
}

void MJ_response::setSendTo(int sendto)
{
    this->SendTo = sendto;
}

void MJ_response::setWho(int w)
{
    this->who = w;
}

void MJ_response::setChi(MJ_Base::CARD chi[])
{
    if(chi == nullptr)
        return;

    memcpy(this->chi, chi, 4);
}

void MJ_response::setgpc(MJ_Base::CARD *g, MJ_Base::CARD *p, MJ_Base::CARD *c)
{
    memset(this->g, 0, 8);
    memset(this->p, 0, 8);
    memset(this->c, 0, 16);

    memcpy(this->g, g, 8);
    memcpy(this->p, p, 8);
    memcpy(this->c, c, 16);
}

void MJ_response::setCard(MJ_Base::CARD c)
{
    this->card = c;
    this->len += 1;
}

void MJ_response::setInitType(std::__cxx11::string name, int xid, int pid, int fpri, int score)
{
    this->X_ID = xid;
    this->P_ID = pid;
    this->f_pri = fpri;
    this->score = score;

    this->p_name = name;
}

void MJ_response::setPaiList(MJ_Base::pCCARD lst)
{
    memcpy(this->paiList, lst, 16);
    this->len += 16;
}

void MJ_response::setPaiCount(int count)
{
    paiCount = count;
}

MJ_response::Type MJ_response::getType() const
{
    return this->type;
}

MJ_Base::CARD MJ_response::getCard() const
{
    return this->card;
}

MJ_response::VER MJ_response::getVer() const
{
    return this->ver;
}

int MJ_response::getSendTo() const
{
    return this->SendTo;
}

int MJ_response::getWho() const
{
    return this->who;
}

void MJ_response::getInitType(std::__cxx11::string &name, int &xid, int &pid, int &fpri, int &score)
{
    name = this->p_name;
    xid = this->X_ID;
    pid = this->P_ID;
    fpri = this->f_pri;
    score = this->score;
}

int MJ_response::getPaiList(MJ_Base::CARD *lst)
{
    if(lst == nullptr)
        return -1;
    memcpy(lst, this->paiList, 16);

    return 0;
}

void MJ_response::getChi(MJ_Base::CARD chi[])
{
    if(chi == nullptr)
        return;

    memcpy(chi, this->chi, 3);
}

void MJ_response::getgpc(MJ_Base::CARD *g, MJ_Base::CARD *p, MJ_Base::CARD *c)
{
    memcpy(g, this->g, 8);
    memcpy(p, this->p, 8);
    memcpy(c, this->c, 16);
}

int MJ_response::getPaiCount()
{
    return paiCount;
}
