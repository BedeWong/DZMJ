#ifndef MJ_RESPONSE_H
#define MJ_RESPONSE_H
#include <string>
#include <cstring>

#include "../pub/MJ_Base.h"

class MJ_response
{
public:
    enum Type{
        T_Ok = 0x01, //ack，回应此次操作是有效的
        T_confirm, //二次确认   /*（已废弃）*/
        T_Wait,
        T_Init,
        T_ChuPai,//告知client该出牌了，也表示
        T_FaPai,
        T_Hu,
        T_Gang,
        T_Peng,
        T_Chi,
        T_BuGang,  // 补杠
        T_AnGang,  // 暗杠
        T_GMOver,  //game over
        T_UnSucc = 0x400,  // 不成功的操作,有时候由于延时导致的不一致，服务端已经超时做出默认动作了
                           // 或者已经收到高优先级的动作了 才收到 client的请求（或低优先级的请求）
                           // 返回此消息
        T_Max
    };

    enum VER{
        v1 = 100
    };

    enum SendToWho{
        SDT_Broadcast = 4
    };

    enum HuType{
        HU_JiePaio,
        HU_ZiMo,
        HU_QiangGang,
        HU_GangShangHua
    };

    typedef unsigned int size_t;

public:
    MJ_response(int sendTo=0, VER v = v1);

    void addType(Type t);
    void setType(Type t);
    void setVer(VER v);
    void setHuType(HuType hu);
    void setSendTo(int sendto);
    void setWho(int w);
    void setChi(MJ_Base::CARD chi[4]);
    void setgpc(MJ_Base::CARD *g, MJ_Base::CARD *p, MJ_Base::CARD *c);

    void setCard(MJ_Base::CARD c);
    void setInitType(std::string name, int xid, int pid, int fpri, int score);
    void setPaiList(MJ_Base::pCCARD lst);
    void setPaiCount(int count);

    Type getType()const;
    HuType getHuType()const;
    VER getVer()const;
    int getSendTo()const;
    int getWho()const;
    MJ_Base::CARD getCard()const;
    void getInitType(std::string &name, int &xid, int &pid, int &fpri, int &score);
    int getPaiList(MJ_Base::CARD *lst);
    void getChi(MJ_Base::CARD chi[3]);
    void getgpc(MJ_Base::CARD *g, MJ_Base::CARD *p, MJ_Base::CARD *c);
    int getPaiCount();

public:
private:
    VER ver;
    Type type;
    HuType huType;
    int SendTo;// a id, recver's id, range 0-4
    int who;//表示上次动作谁做出来的（例如：出牌，通知其他玩家时标志谁出的牌）
    size_t len;//  msg length
    int paiCount;

    MJ_Base::CARD card;
    MJ_Base::CARD paiList[16];
    MJ_Base::CARD chi[4];
    MJ_Base::CARD g[8];
    MJ_Base::CARD p[8];
    MJ_Base::CARD c[16];
    int P_ID;//玩家ID
    int X_ID;//座位号
    int f_pri;//庄家&闲家
    std::string p_name;//玩家昵称
    int score;//玩家金钱数
};

#endif // MJ_RESPONSE_H
