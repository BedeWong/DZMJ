#ifndef MJC_CARDSET_H
#define MJC_CARDSET_H

#include <map>
#include "MJ_AnalyResult.h"

#define DEBUG

class MJC_CardSet
{
public:
    typedef int _L;
    typedef char CARD;
    typedef const char* pCCARD;

private:
    CARD wang;
    CARD NewCard;
    int paiCount;

    CARD paiList[16];

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

private:
    bool hasSanLian(CARD*li, CARD c, CARD **res);
    bool hasErLian(CARD*li, CARD c, CARD **res);
    bool hasKanZhang(CARD*li, CARD c, CARD **res);

public:

//    pCCARD __cards = "风ABCDEFG  筒RSTUVWXYZ 条abcdefghi 万rstuvwxyz";
    enum MJ_CARDS{
        MJ_noCard=0,
        MJ_JIUWAN = 'r', MJ_BAWAN, MJ_QIWAN, MJ_LIUWAN, MJ_WUWAN, MJ_SIWAN, MJ_SANWAN, MJ_ERWAN, MJ_YIWAN,
        MJ_JIUTIAO = 'a', MJ_BATIAO, MJ_QITIAO, MJ_LIUTIAO, MJ_WUTIAO, MJ_SITIAO, MJ_SANTIAO, MJ_ERTIAO, MJ_YITIAO,
        MJ_JIUTONG = 'R', MJ_BATONG, MJ_QITONG, MJ_LIUTONG, MJ_WUTONG, MJ_SITONG, MJ_SANTONG, MJ_ERTONG, MJ_YITONG,
        MJ_BAN = 'A', MJ_FA, MJ_ZHONG, MJ_BEI, MJ_XI, MJ_NAN, MJ_DONG,
        MJ_WANG = '0'
    };

    MJC_CardSet();

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

    int getChiList(CARD card, CARD (*res)[3]);

    int copy_chiList(MJ_AnalyResult *);
    int copy_pengList(MJ_AnalyResult *);
    int copy_gangList(MJ_AnalyResult *);

    void MJ_sort(typename MJC_CardSet::CARD *a, int len);

#ifdef DEBUG
    void printTest();
#endif
};

#define isFENG(card) ((card) <= MJC_CardSet::MJ_DONG && (card) >= MJC_CardSet::MJ_BAN)
#define isTONG(card) ((card) >= MJC_CardSet::MJ_JIUTONG && (card) <= MJC_CardSet::MJ_YITONG)
#define isTIAO(card) ((card) >= MJC_CardSet::MJ_JIUTIAO && (card) <= MJC_CardSet::MJ_YITIAO)
#define isWAN(card) ((card) >= MJC_CardSet::MJ_JIUWAN && (card) <= MJC_CardSet::MJ_YIWAN)

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

#endif // MJC_CARDSET_H
