#include "TimeBar.h"

TimeBar::TimeBar(){
	timeBarSprite = CCSprite::create("time.png", CCRectMake(0, 0, 320, 61));
	timeBarSprite->setPosition(ccp(160,12));
	timeBarSprite->setOpacity(255*0.6);
	//timeLabelSprite = CCSprite::create("time_label.png", CCRectMake(0, 0, 97, 44));
	//timeLabelSprite->setPosition(ccp(48,35));

	timeBarContainer = CCLayer::create();
	timeBarContainer->setContentSize(CCSizeMake(320,61));
	timeBarContainer->setPosition(ccp(0,0));

	timeBarContainer->addChild(timeBarSprite,10);
	

	timeTextLabel = CCLabelTTF::labelWithString(
		"1:00",
		CCSizeMake(320,43),
		kCCTextAlignmentLeft,
		kCCVerticalTextAlignmentBottom,
		"fonts/impact.ttf",
		20);

	timeTextLabel->setPosition(ccp(160+5,0));
	timeTextLabel->setColor(ccc3(0,0,0));
	timeTextLabel->setOpacity(255*0.9);
	timeBarContainer->addChild(timeTextLabel,11);
}

TimeBar::~TimeBar(){

}

void TimeBar::setTime(int progress){
	int minutes = progress/60;
	int seconds = progress % 60;
	char tempc[20];
	sprintf(tempc,"%d:%02d",minutes,seconds);
	std::string str1 = "";
	str1 = tempc;
	timeTextLabel->setString(str1.c_str());
}

void TimeBar::setOpacity(float opacity){
	
	//timeLabelSprite->setOpacity(opacity);
}