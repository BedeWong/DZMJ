#ifndef MJ_REQUESTLOCAL_H
#define MJ_REQUESTLOCAL_H

#include <QObject>

#include "../pub/MJ_Request.h"

class MJ_RequestLocal : public QObject, MJ_Request
{
    Q_OBJECT
public:
    explicit MJ_RequestLocal(QObject *parent = 0);

    void send(MJ_response &) override;

signals:

public slots:

private:

};

#endif // MJ_REQUESTLOCAL_H
