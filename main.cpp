#include <QApplication>

#include <iostream>
#include "cli/MJ_Cli.h"

#include "cli/widget/MJ_selfWidget.h"
#include "pub/MJ_Player.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MJ_Cli desk;
    desk.show();
//    MJ_Base::CARD mjlist[] = {"ABCczyA"};
//    MJ_Player player;

//    player.init(mjlist, 'z');

//    player.Gang('f');
//    MJ_Base::CARD ll[4] = "def";
//    player.Chi('d', ll);

//    player.setNewCard('c');
//    player.setPaiList(mjlist);

//    MJ_selfWidget wid;

//    wid.setSize(QSize(840, 100));
//    wid.setModel(&player);
//    wid.draw_NewCard(true);
//    wid.draw_PaiList();

//    wid.show();


    return a.exec();
}



