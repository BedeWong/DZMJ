#ifndef MJ_REQUESTLOCAL_H
#define MJ_REQUESTLOCAL_H

#include <QObject>

#include "../pub/MJ_Request.h"
#include "../pub/MJ_RequestData.h"

class MJ_RequestLocal : public QObject, MJ_Request
{
    Q_OBJECT
public:
    explicit MJ_RequestLocal(QObject *parent = 0);

    void send(MJ_RequestData &) override;

signals:
    void RequestSignal(MJ_RequestData);

public slots:

private:

};

#endif // MJ_REQUESTLOCAL_H
