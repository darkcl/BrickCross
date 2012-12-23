#ifndef __TIMEBAR_H__
#define __TIMEBAR_H__

#include "cocos2d.h"
using namespace cocos2d;

class TimeBar: public CCObject
{
public:
	CCSprite* timeLabelSprite;
	CCSprite* timeBarSprite;
	CCLayer* timeBarContainer;
	CCLabelTTF* timeTextLabel;

	TimeBar();
	~TimeBar();

	CCSprite* getTimeBar(){return timeBarSprite;};
	CCLayer* getTimeContainer(){return timeBarContainer;};
	
	void setTime(int progress);
	void setOpacity(float opacity);
};

#endif 