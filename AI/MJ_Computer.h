#ifndef MJ_COMPUTER_H
#define MJ_COMPUTER_H

#include <QObject>

#include "../pub/MJ_Player.h"
#include "../pub/MJ_Request.h"
#include "../pub/MJ_RequestData.h"
#include "../pub/MJ_response.h"

class MJ_Computer : public QObject
{
    Q_OBJECT
public:
    explicit MJ_Computer(QObject *parent = 0);
    ~MJ_Computer();

signals:

public slots:

public:

private:
    MJ_Player *player;
};

#endif // MJ_COMPUTER_H
