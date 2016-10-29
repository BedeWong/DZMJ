#include "MJ_RequestLocal.h"
#include "MJ_LocalServer.h"

MJ_RequestLocal::MJ_RequestLocal(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(RequestSignal(MJ_RequestData)),
            MJ_LocalServer::getInstance(), SLOT(RecvSlot(MJ_RequestData)), Qt::QueuedConnection);
}

void MJ_RequestLocal::req_send(MJ_RequestData &req)
{
    emit RequestSignal(req);
    return;
}
