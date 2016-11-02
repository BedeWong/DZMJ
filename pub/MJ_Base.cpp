#include "MJ_Base.h"
#include <QDebug>

MJ_Base::MJ_Base()
{

}

void MJ_Base::init()
{
    paiCount = 0;
    paiRecCount = 0;
    gpc_seq_count = 0;
    memset(gpc_seq, 0, sizeof(gpc_seq));
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

void MJ_Base::setNewCard(MJ_Base::CARD c)
{
    this->NewCard = c;
}

MJ_Base::~MJ_Base()
{

}

int MJ_Base::getPaiList(MJ_Base::CARD *cds) const
{
    if(cds != nullptr)
    {
        memcpy(cds, this->paiList, 16);
    }
    return this->paiCount;
}

int MJ_Base::getPaiRecList(MJ_Base::CARD *cds) const
{
    if(cds != nullptr)
    {
        memcpy(cds, this->paiRecList, 50);
    }
    return this->paiRecCount;
}

int MJ_Base::getGang(MJ_Base::CARD *cds) const
{
    if(cds != nullptr)
    {
        memcpy(cds, this->gang, 8);
    }
    return this->_g;
}

int MJ_Base::getPeng(MJ_Base::CARD *cds) const
{
    if(cds != nullptr)
    {
        memcpy(cds, this->peng, 8);
    }
    return this->_p;
}

int MJ_Base::getChi(MJ_Base::CARD *cds) const
{
    if(cds != nullptr)
    {
        memcpy(cds, this->chi, 16);
    }
    return this->_c;
}

int MJ_Base::getGPCseq(int *cds) const
{
    if(cds != nullptr)
    {
        memcpy(cds, this->gpc_seq, 8*sizeof(int));
    }
    return this->gpc_seq_count;
}

int MJ_Base::setPaiCount(int count)
{
    this->paiCount = count;

    return 0;
}

int MJ_Base::setPaiList(MJ_Base::CARD *lst)
{
    if(lst == nullptr)
        return -1;

    memcpy(this->paiList, lst, 16);
    int len = strlen(lst);
    this->paiCount = len;

    return 0;
}

int MJ_Base::setPengList(MJ_Base::CARD *lst, int len)
{
    if(len == -1)
    {
        len = strlen(lst);
    }
    memset(this->peng, 0, 8);
    memcpy(this->peng, lst, len);
    this->_p = len;
    return 0;
}

int MJ_Base::setChiList(MJ_Base::CARD *lst, int len)
{
    if(len == -1)
    {
        len = strlen(lst);
    }
    memset(this->chi, 0, 16);
    memcpy(this->chi, lst, len);
    this->_c = len;
    return 0;
}

int MJ_Base::setGangList(MJ_Base::CARD *lst, int len)
{
    if(len == -1)
    {
        len = strlen(lst);
    }

    memset(this->gang, 0, 8);
    memcpy(this->gang, lst, len);
    this->_g = len;
    return 0;
}

// ***** ******   ÓĞ¿Ó´ıÌî
bool MJ_Base::getNewCard(CARD &cd)
{
    cd = NewCard;

    return true;
}

MJ_Base::CARD MJ_Base::getLastCard()
{
    qDebug() << __FUNCTION__ << __LINE__ << this->paiList << this->paiCount << endl;
    return this->paiList[this->paiCount-1];
}
