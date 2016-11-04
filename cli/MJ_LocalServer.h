#ifndef MJ_LOCALSERVER_H
#define MJ_LOCALSERVER_H

#include <QObject>
#include <QTimer>
#include <random>
#include <ctime>

#include "../pub/MJ_RequestData.h"
#include "../pub/MJ_response.h"
#include "../pub/MJ_Player.h"
#include "MJ_RequestLocal.h"

class MJ_LocalServer : public QObject
{
    Q_OBJECT
    explicit MJ_LocalServer(QObject *parent = 0);
public:
    static MJ_LocalServer *Instance;
    static MJ_LocalServer *getInstance();
    ~MJ_LocalServer();

    void send(MJ_response &);

    void start();//开局
    void faPai_NoCard();
    void faPai();
    void resl_chuPai(MJ_RequestData &);
    void resl_HGPCList(MJ_RequestData &);
    void resl_Hu(MJ_RequestData &);
    void resl_Gang(MJ_RequestData &);
    void resl_Peng(MJ_RequestData &);
    void resl_Chi(MJ_RequestData &);
    void resl_Cancel(MJ_RequestData &);
    void resl_AnGang(MJ_RequestData &);
    void resl_BuGang(MJ_RequestData &);

private:
    void _FaPaiRequest();

signals:
    void responseSignal(MJ_response);

    void _postToRecvSlot(MJ_RequestData request);  // 给自己发送，避免函数直接调用带来的混乱

public slots:
    void RecvSlot(MJ_RequestData request);
    void tmOutSlot();
    void tmChuPaiSlot();

public:
    enum SVR_STAT{
        SVR_normal,
        SVR_vote
    };

    enum Policy{
        P_None = 0x0,
        P_Chi = 0x01,
        P_Peng = 0x10,
        P_Gang = 0x100,
        P_Hu = 0x1000,
        P_Max
    };

private:
    MJ_Player *player[4];

    int mem_policy[4];//存放每个玩家胡杠碰吃信息
    int current_policy;//Policy类型,保存当前级别最高的（胡>杠[碰]>吃）
    int current_policy_ID;
    bool f_HGPC_valid;//定义 胡杠碰吃 有效，有时候有可能响应或延迟问题导致下家已经发牌了，上家才送来碰牌消息
    bool gang_falg;
    MJ_Base::CARD chi[4];

    int cur_id;
    int zhuang_id;
    int winner_id;

    MJ_Base::CARD wang;
    MJ_Base::CARD card;//最近发的一张牌
    MJ_Base::CARD tmOutCard;//胡杠碰吃超时出这张

    int shaizi1, shaizi2;//色子
    int wj_ready;

    int paiCount;//当前牌数，增长到maxPaiCount本局游戏结束
    int maxPaiCount;
    MJ_Base::CARD paiList[136];//所有的牌

    SVR_STAT stat;
    QTimer *tmOut;
    QTimer *tmChuPai;
};

#define resp_unsuccessful(who, to) do{\
    MJ_response resp;\
    resp.setType(MJ_response::T_UnSucc);\
    resp.setWho((who));\
    resp.setSendTo((to));\
    send(resp);\
}while(0)

#endif // MJ_LOCALSERVER_H
