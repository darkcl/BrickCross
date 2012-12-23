
#ifndef __MULTIPLAYCONSTANTS_H
#define __MULTIPLAYCONSTANTS_H

// type of transfer inGame-data ()
const unsigned char PHOTON_UNRELIABLE = 0;
const unsigned char PHOTON_RELIABLE   = 1;

const unsigned char EV_PLAYERINFO	= 0;
const unsigned char EV_HPCHANGE		= 1;
const unsigned char EV_DISCONNECT	= 2;
const unsigned char EV_LOST	= 3;

const nByte USER_NAME_KEY = 101;
const nByte HP_CHANGE_KEY = 102;

// Keys for PlayerStatus-Events
const unsigned char STATUS_PLAYER_HP		= 0;
const unsigned char STATUS_PLAYER_NAME		= 1;
const unsigned char STATUS_PLAYER_PLATFORM  = 2;

#endif