//#include <QCoreApplication>

#include <iostream>
#include "pub/MJ_Player.h"

using namespace std;

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);


    MJ_Player set;
    MJ_Player::CARD cards[16], wang, del, add;
    while(1)
    {
        cin >> cards >> wang;

        set.init(cards, wang);

        set.analysis();

        set.printTest();
        set.printHu();
    }

    return 0;
}
