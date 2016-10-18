#ifndef MJPLAYER_H
#define MJPLAYER_H

#include "../pub/MJ_Player.h"
#include "../pub/MJ_otherPlayer.h"

class MJplayer
{
    MJ_Player *player;
    MJ_otherPlayer *other1;
    MJ_otherPlayer *other2;
    MJ_otherPlayer *other3;



public:
    MJplayer();
};

#endif // MJPLAYER_H
