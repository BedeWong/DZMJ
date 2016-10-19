#include "MJ_RequestLocal.h"

MJ_RequestLocal::MJ_RequestLocal(QObject *parent) : QObject(parent)
{

}

void MJ_RequestLocal::req_send(MJ_RequestData &req)
{
    emit RequestSignal(req);
    return;
}
