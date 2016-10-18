#ifndef MJ_OTHERPLAYER_H
#define MJ_OTHERPLAYER_H
#include <string>

class MJ_otherPlayer
{
public:
    typedef int _L;
    typedef char CARD;
    typedef const char* pCCARD;

    enum MJ_CARDS{
        MJ_noCard=0,
        MJ_JIUWAN = 'r', MJ_BAWAN, MJ_QIWAN, MJ_LIUWAN, MJ_WUWAN, MJ_SIWAN, MJ_SANWAN, MJ_ERWAN, MJ_YIWAN,
        MJ_JIUTIAO = 'a', MJ_BATIAO, MJ_QITIAO, MJ_LIUTIAO, MJ_WUTIAO, MJ_SITIAO, MJ_SANTIAO, MJ_ERTIAO, MJ_YITIAO,
        MJ_JIUTONG = 'R', MJ_BATONG, MJ_QITONG, MJ_LIUTONG, MJ_WUTONG, MJ_SITONG, MJ_SANTONG, MJ_ERTONG, MJ_YITONG,
        MJ_BAN = 'A', MJ_FA, MJ_ZHONG, MJ_BEI, MJ_XI, MJ_NAN, MJ_DONG,
        MJ_WANG = '0', MJ_ANGNG='*'
    };

private:
    int stat;//玩家状态
    int x_ID;//座位号
    int p_ID;//玩家ID
    unsigned int score;//玩家分数
    int f_pri;//庄家&闲家
    std::string p_name;//玩家昵称

    int paiCount;
    int paiRecCount;

    CARD paiRecList[50];
    CARD paiList[16];
    CARD gang[8];
    CARD peng[8];
    CARD chi[16];
    CARD NewCard;

    _L _g;
    _L _p;
    _L _c;
    _L _h;
public:
    MJ_otherPlayer();

    void init();

    int Gang(CARD c);
    int Peng(CARD c);
    int Chi(CARD c[3]);
    int Hu(CARD c, pCCARD plist);

    int ChuPai(CARD c);
};

#endif // MJ_OTHERPLAYER_H
