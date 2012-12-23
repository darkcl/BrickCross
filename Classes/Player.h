#ifndef __PLAYER_H
#define __PLAYER_H

#include "cocos2d.h"
using namespace cocos2d;
class Player : public CCObject
{
public:
	static const int MAX_USERNAME_LEN = 128;

	Player();
	~Player();
	int hp;
	int id;
	char user_name[MAX_USERNAME_LEN];

	CCSprite *hpSprite;
	CCLabelTTF* hpLabel;

	void setLabel(bool isMySelf);
	void setHpSprite(bool isMySelf);
	void setHp(int health);
};

#endif