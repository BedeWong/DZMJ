#ifndef MJ_PLAYER_H
#define MJ_PLAYER_H

#include <map>
#include <string>
#include "MJ_AnalyResult.h"

#define DEBUG

class MJ_Player
{
public:
    typedef int _L;
    typedef char CARD;
    typedef const char* pCCARD;

    //pCCARD __cards = "风ABCDEFG  筒RSTUVWXYZ 条abcdefghi 万rstuvwxyz";
    enum MJ_CARDS{
        MJ_noCard=0,
        MJ_JIUWAN = 'r', MJ_BAWAN, MJ_QIWAN, MJ_LIUWAN, MJ_WUWAN, MJ_SIWAN, MJ_SANWAN, MJ_ERWAN, MJ_YIWAN,
        MJ_JIUTIAO = 'a', MJ_BATIAO, MJ_QITIAO, MJ_LIUTIAO, MJ_WUTIAO, MJ_SITIAO, MJ_SANTIAO, MJ_ERTIAO, MJ_YITIAO,
        MJ_JIUTONG = 'R', MJ_BATONG, MJ_QITONG, MJ_LIUTONG, MJ_WUTONG, MJ_SITONG, MJ_SANTONG, MJ_ERTONG, MJ_YITONG,
        MJ_BAN = 'A', MJ_FA, MJ_ZHONG, MJ_BEI, MJ_XI, MJ_NAN, MJ_DONG,
        MJ_WANG = '0', MJ_ANGNG='*'
    };

    friend class MJ_AnalyResult;

private:
    CARD wang;
    CARD NewCard;
    int paiReCount;//出过的牌张数
    int paiCount;//手上的牌张数

    CARD paiRecord[50];//出过的牌集合
    CARD paiList[16];// 手上的牌

    CARD HuList[16];
    CARD cChiList[16];
    CARD cGangList[8];
    CARD cPengList[8];

    CARD gang[8];
    CARD peng[8];
    CARD chi[16];

    _L _g;
    _L _p;
    _L _c;
    _L _h;

    std::map<CARD, MJ_AnalyResult> AnalyResults;

    int stat;
    int x_ID;
    int p_ID;
    unsigned int score;
    int f_pri;
    std::string p_name;

private:
    bool hasSanLian(CARD*li, CARD c, CARD **res);
    bool hasErLian(CARD*li, CARD c, CARD **res);
    bool hasKanZhang(CARD*li, CARD c, CARD **res);

    void MJ_sort(typename MJ_Player::CARD *a, int len);

public:

    MJ_Player();

    // 开局初始化 并 分析 本家局势
    void init(pCCARD _paiList, CARD _wang) ;
    // 抓牌
    void addCard(CARD newCard) ;

    // 出牌
    int delCard(CARD card);
    int delCard(int offset);

    int cChi() ;
    int cPeng() ;
    int cGang() ;
    int analysis();

    int Chi(CARD card, CARD c[3]);
    int Peng(CARD card);
    int Gang(CARD card);
    bool Hu(CARD card);

    //根据一张牌 获取到可以吃的牌组合。
    //返回值：-1  给出的card有误，0表示没有可以吃的，返回正整数表示res中有多少组
    //res存放结果，非线程安全
    int getCChiList(CARD card, CARD (*res)[3]);

    int copy_chiList(MJ_AnalyResult *);
    int copy_pengList(MJ_AnalyResult *);
    int copy_gangList(MJ_AnalyResult *);


#ifdef DEBUG
    void printTest();
    void printHu();
#endif
};

#define isFENG(card) ((card) <= MJ_Player::MJ_DONG && (card) >= MJ_Player::MJ_BAN)
#define isTONG(card) ((card) >= MJ_Player::MJ_JIUTONG && (card) <= MJ_Player::MJ_YITONG)
#define isTIAO(card) ((card) >= MJ_Player::MJ_JIUTIAO && (card) <= MJ_Player::MJ_YITIAO)
#define isWAN(card) ((card) >= MJ_Player::MJ_JIUWAN && (card) <= MJ_Player::MJ_YIWAN)

//  判断是一张合法的牌
#define isMJCARD(card) (isFENG(card) || isTONG(card) || isTIAO(card) || isWAN(card))

//  三张相同的牌
#define hasSanTong(plist) ((plist)[0] == (plist)[1] && (plist)[0] == (plist)[2])
//  三连
//#define hasSanLian(plist) (!isFENG((plist)[0])) && strchr((plist), (plist)[0]-1) && strchr((plist), (plist)[0]-2)
//  对子
#define hasDuiZi(plist) ((plist)[0] == (plist)[1])
//  二连
//#define hasErLian(plist) (!isFENG((plist)[0]) && (strchr((plist), (plist)[0]-1))
//  坎张
//#define hasKanZhang(plist) (!isFENG((plist)[0]) && (strchr((plist),(plist)[0]-2)))

#endif // MJ_Player_H
