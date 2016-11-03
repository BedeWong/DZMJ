#ifndef MJ_PLAYER_H
#define MJ_PLAYER_H

#include <map>
#include <string>
#include "MJ_AnalyResult.h"
#include "MJ_Base.h"

class MJ_Player : public MJ_Base
{
    friend class MJ_AnalyResult;

private:

    //  分析过后的  可 胡杠碰吃 集
    CARD cHuList[16];
    CARD cChiList[16];
    CARD cGangList[8];
    CARD cPengList[8];

    CARD wang;

    _L cHuCount;
    _L cPengCount;
    _L cGangCount;
    _L cChiCount;

    std::map<CARD, MJ_AnalyResult> AnalyResults;
private:
    bool inPaiList(CARD c);

    bool hasSanLian(CARD*li, CARD c, CARD **res);
    bool hasErLian(CARD*li, CARD c, CARD **res);
    bool hasKanZhang(CARD*li, CARD c, CARD **res);

    void MJ_sort(typename MJ_Player::CARD *a, int len);

    // 分析可 吃碰杠胡
    // 返回值：>=0匹配结果数， <0：数据有误
    int cChi() ;
    int cPeng() ;
    int cGang() ;
    int cHu();
    int analysis();

public:

    MJ_Player();

    // 开局初始化 并 分析 本家局势
    void init(pCCARD _paiList, CARD _wang) override;

    void setWang(CARD wang);
    CARD getWang()const;

    // 把抓到的牌放入牌列
    void addCard(CARD newCard);

    int ChuPai(CARD c) override;
    // 删掉一张牌（出牌-1，吃-2， 碰-2， 杠-3）
    int DelCard(CARD card);
    //int DelCard(int offset);

    void AnalysisHGPC();

    int getCanChiList(CARD *lst, int c)const;
    int getCanPengList(CARD *lst, int c)const;
    int getCanGangList(CARD *lst, int c)const;
    int getCanHuList(CARD *lst, int c)const;

    int Hu(CARD, pCCARD) override;
    int Gang(CARD c) override;
    int Peng(CARD c) override;
    int Chi(CARD c, pCCARD ll) override;

    //根据一张牌 获取到可以吃的牌组合。
    //返回值：-1  给出的card有误，0表示没有可以吃的，返回正整数表示res中有多少组
    //res存放结果，非线程安全
    int getCChiList(CARD card, CARD (*res)[4]);
    bool testHu(CARD c);
    bool testPeng(CARD c);
    bool testGang(CARD c);
    bool testChi(CARD c);

    // 检测是否可补杠
    bool testBuGang(CARD c);
    bool testZiMo(CARD c);
    CARD testAnGang();

    //svr 中用到，直接设置玩家分析的结果进集合中
    //返回值：此些函数中有验证功能，通过返回（list）个数，其他返回-1
    int setcHuList(CARD _h[]);
    int setcGangList(CARD _g[]);
    int setcPengList(CARD _p[]);
    int setcChiList(CARD _c[]);

    int copy_chiList(MJ_AnalyResult *);
    int copy_pengList(MJ_AnalyResult *);
    int copy_gangList(MJ_AnalyResult *);

    MJ_AnalyResult getAnalyResult(CARD c)const;

#ifdef DEBUG
    void printTest();
    void printHu();
#endif
};


#endif // MJ_Player_H
