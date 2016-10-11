//#include <QCoreApplication>

#include <iostream>
#include "pub/MJC_CardSet.h"

using namespace std;

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);


    MJC_CardSet set;
    MJC_CardSet::CARD cards[16], wang, del, add;
    while(1)
    {
        cin >> cards >> wang;

        set.init(cards, wang);

        set.analysis();
    }

    return 0;
}
