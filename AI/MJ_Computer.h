#ifndef MJ_COMPUTER_H
#define MJ_COMPUTER_H

#include <QObject>

#include "../pub/MJ_Player.h"
#include "../cli/MJ_RequestLocal.h"
#include "../pub/MJ_RequestData.h"
#include "../pub/MJ_response.h"

class MJ_Computer : public QObject
{
    Q_OBJECT
public:
    explicit MJ_Computer(QObject *parent = 0);
    ~MJ_Computer();

    void setID(int _id);

private:
    void resl_init(MJ_response&);
    void resl_wait(MJ_response&);
    void resl_Chi(MJ_response&);
    void resl_Peng(MJ_response&);
    void resl_Gang(MJ_response&);
    void resl_Hu(MJ_response&);
    void resl_FaPai(MJ_response&);
    void resl_ChuPai(MJ_response&);
    void resl_OK(MJ_response&);
    void resl_Unsucc(MJ_response&);
    void resl_Over(MJ_response&);
//    void resl_(MJ_response&);

signals:

public slots:
    void responseSlot(MJ_response response);

public:

private:
    MJ_Player *player;
    MJ_RequestLocal *request;

    int Id;
    MJ_Base::CARD card;
    bool f_HGPC;
};

#endif // MJ_COMPUTER_H
