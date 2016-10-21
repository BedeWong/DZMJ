#ifndef MJ_REQUESTDATA_H
#define MJ_REQUESTDATA_H

#include "../pub/MJ_Base.h"

#define DataSize 512

class MJ_RequestData
{
public:
    enum Type{
        R_NoType,
        R_Hu = 0x01,
        R_Gang = 0x02,
        R_Peng = 0x04,
        R_Chi = 0x08,
        R_CanCel = 0x10,//取消：放弃操作--例如可胡杠碰吃，但玩家不想执行操作
        R_Init = 0x20,//初始化：游戏开局， 断线重连，client数据出错
        R_ChuPai = 0x40,//出牌：
        R_TuoGuan = 0x80,//托管
        R_Exit = 0x100,
        R_HGPCList = 0x200,//胡杠碰吃列表：客户端需要把分析好的数据传到服务端，由服务端做核对
        R_AnGang = 0x400,//四张牌都在我手上，可以暗杠
        R_BuGang = 0x800,//手上有一张牌，正好是之前碰过的，可以补杠
        R_Max
    };

    typedef unsigned int size_t;
public:
    MJ_RequestData(int id, Type type=R_NoType, MJ_Base::CARD c=MJ_Base::MJ_noCard);

    void setType(Type type);
    void addType(Type type);
    void setCard(MJ_Base::CARD c);
    void setHGPCList(MJ_Base::pCCARD h, MJ_Base::pCCARD g, MJ_Base::pCCARD p, MJ_Base::pCCARD c);
    void setChi(MJ_Base::CARD chi[3]);
    void setSenderID(int id);

    int getData(char *dst);//从buf位置填充本请求体，返回值：填充的长度

    int getType()const;
    int getHGPCList(MJ_Base::CARD *h, MJ_Base::CARD *g, MJ_Base::CARD *p, MJ_Base::CARD *c)const;
    int getChi(MJ_Base::CARD chi[3])const;
    MJ_Base::CARD getCard()const;
    int getSenderID()const;
public:

private:
    char data[DataSize];
    size_t len;

    size_t ver;
    int type;
    int senderID;//0-3

    MJ_Base::CARD cd;
    MJ_Base::CARD chiList[8];
    MJ_Base::CARD pengList[8];
    MJ_Base::CARD gangList[8];
    MJ_Base::CARD huList[8];

    MJ_Base::CARD chi[3];
    MJ_Base::CARD peng;
    MJ_Base::CARD gang;
    MJ_Base::CARD hu;
};

#endif // MJ_REQUESTDATA_H
