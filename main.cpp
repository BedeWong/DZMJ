#include <QApplication>
#include <QMetaType>
#include <QFile>
#include <QDebug>

#define UNDEBUG 1

#if UNDEBUG
#include <iostream>
#include "cli/MJ_Cli.h"
#include "pub/MJ_RequestData.h"
#include "pub/MJ_response.h"

#include "cli/widget/MJ_selfWidget.h"
#include "pub/MJ_Player.h"
#else
#include "pub/MJ_AnalyResult.h"
#include "pub/MJ_Player.h"

#include "cli/widget/MJ_gameOverWidget.h"
#endif
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qss("./DZMJ.qss");
    qss.open(QIODevice::ReadOnly);

    qss.seek(0);
    a.setStyleSheet(qss.readAll());
    qss.close();

#if UNDEBUG
    qRegisterMetaType<MJ_RequestData>("MJ_RequestData");
    qRegisterMetaType<MJ_response>("MJ_response");
    qRegisterMetaType<MJ_Base::CARD>("MJ_Base::CARD");

    MJ_Cli desk;
    desk.show();
#else
    MJ_gameOverWidget wid;

    MJ_Base::CARD pailist[16] = {"yxwgfcbaZYXXX"};
    MJ_Base::CARD g[8]={0}, p[8]={0}, c[16]={0};
    MJ_Base::CARD hucard = 'h';

    MJ_Player win = MJ_Player();
    win.setPengList(p);
    win.setGangList(g);
    win.setChiList(c);
    win.setPaiList(pailist);
    win.setWang('Q');
    win.AnalysisHGPC();

    wid.setPai(g, p, c,pailist, hucard);

    if(win.testHu(hucard))
    {
        MJ_AnalyResult analy(win);
        analy.calc_BeiShu(win, hucard, MJ_AnalyResult::F_JiePao);
        int fan = analy.getFan();
        auto list = analy.HU_names();

        wid.setFan(list, fan);
    }
    wid.show();
#endif

    return a.exec();
}



