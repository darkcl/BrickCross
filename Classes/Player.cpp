#include "Player.h"
using namespace std;

Player::Player(){
	hp=100;
	id=-1;
	std::strcpy(user_name,"unknown");

	hpLabel = CCLabelTTF::labelWithString(
		"UNKNOWN",
		CCSizeMake(320,30),
		kCCTextAlignmentLeft,
		kCCVerticalTextAlignmentBottom,
		"fonts/impact.ttf",
		20);
}

Player::~Player(){

}

void Player::setHp(int health){
	hp = health;
	std::string *labelString;
	CCString *str = CCString::stringWithFormat("%s : %d",(const char *)user_name,hp);
	hpLabel->setString(str->getCString());
}

void Player::setHpSprite(bool isMySelf){
	if(isMySelf){
		hpSprite = CCSprite::create("your_hp.png",CCRectMake(0,0,320,12));
		hpSprite->setPosition(ccp(160,30.5));
	}else{
		hpSprite = CCSprite::create("opp_hp.png",CCRectMake(0,0,320,12));
		hpSprite->setPosition(ccp(160,63.5));
	}
}

void Player::setLabel(bool isMySelf){
	if(isMySelf){
		hpLabel->setString("YOUR HP");
		hpLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
		hpLabel->setPosition(ccp(160,12+30.5));
	}else{
		hpLabel->setString("RIVAL HP");
		hpLabel->setHorizontalAlignment(kCCTextAlignmentRight);
		hpLabel->setPosition(ccp(160,12+63.5));
	}
}