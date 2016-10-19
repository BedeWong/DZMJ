#ifndef MJ_RESPONSE_H
#define MJ_RESPONSE_H
#include <string>
#include <cstring>

#include "../pub/MJ_Base.h"

class MJ_response
{
public:
    enum Type{
        T_Ok = 0x01, //ack
        T_confirm, //二次确认
        T_Wait,
        T_Init,
        T_ChuPai,
        T_FaPai,
        T_Hu,
        T_Gang,
        T_Peng,
        T_Chi,
        T_GMOver,//game over
        T_Max
    };

    enum VER{
        v1 = 100
    };

    enum SendToWho{
        SDT_Broadcast = 4
    };

    typedef unsigned int size_t;

public:
    MJ_response(int sendTo=0, VER v = v1);

    void addType(Type t);
    void setType(Type t);
    void setVer(VER v);
    void setSendTo(int sendto);
    void setWho(int w);
    void setChi(MJ_Base::CARD chi[3]);

    void setCard(MJ_Base::CARD c);
    void setInitType(std::string name, int xid, int pid, int fpri, int score);
    void setPaiList(MJ_Base::pCCARD lst);

    Type getType()const;
    VER getVer()const;
    int getSendTo()const;
    int getWho()const;
    MJ_Base::CARD getCard()const;
    void getInitType(std::string &name, int &xid, int &pid, int &fpri, int &score);
    int getPaiList(MJ_Base::CARD *lst);
    void getChi(MJ_Base::CARD chi[3]);

public:
private:
    VER ver;
    Type type;
    int SendTo;// a id, recver's id, range 0-4
    int who;//表示上次动作谁做出来的（例如：出牌，通知其他玩家时标志谁出的牌）
    size_t len;//  msg length

    MJ_Base::CARD card;
    MJ_Base::CARD paiList[16];
    MJ_Base::CARD chi[3];
    int P_ID;//玩家ID
    int X_ID;//座位号
    int f_pri;//庄家&闲家
    std::string p_name;//玩家昵称
    int score;//玩家金钱数
};

#endif // MJ_RESPONSE_H
