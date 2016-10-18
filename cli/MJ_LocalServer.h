#ifndef MJ_LOCALSERVER_H
#define MJ_LOCALSERVER_H

#include <QObject>

#include "../pub/MJ_RequestData.h"
#include "../pub/MJ_response.h"
#include "MJ_RequestLocal.h"

class MJ_LocalServer : public QObject
{
    Q_OBJECT
public:
    explicit MJ_LocalServer(QObject *parent = 0);

    void send(MJ_response &);

signals:
    void responseSignal(MJ_response);

public slots:
    void RecvSlot(MJ_RequestData request);

public:

private:

};

#endif // MJ_LOCALSERVER_H
