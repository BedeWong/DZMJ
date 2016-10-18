#ifndef MJ_LOCALSERVER_H
#define MJ_LOCALSERVER_H

#include <QObject>

#include "../pub/MJ_response.h"

class MJ_LocalServer : public QObject
{
    Q_OBJECT
public:
    explicit MJ_LocalServer(QObject *parent = 0);


    void send(MJ_response &);

signals:

public slots:
    void RecvSlot(MJ_response response);
private:

public:
};

#endif // MJ_LOCALSERVER_H
