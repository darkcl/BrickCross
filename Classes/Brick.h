#ifndef __BRICK_H__
#define __BRICK_H__

#include "cocos2d.h"
using namespace cocos2d;

class Brick : public CCObject
{
public:
	CCSprite *brickSprite;
	CCSprite *highLight;
	ccColor3B brickColor;

	int type;
	int tag;
	bool selected;

	Brick();
	~Brick();

	CCSprite *getSprite(){return brickSprite;};
	void setColor(float r,float g,float b){brickSprite->setColor(ccc3(r,g,b));brickColor = ccc3(r,g,b);};
	void selectAnimation();
	void stopSelectAnimation();
	void destoryAnimation();
};

#endif
