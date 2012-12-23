#include "Brick.h"

Brick::Brick(){
	brickSprite = CCSprite::create("brick.png", CCRectMake(0, 0, 64, 25));
	highLight = CCSprite::create("brick.png", CCRectMake(0, 0, 64, 25));
	highLight->setPosition(ccp(32,12.5));
	highLight->setOpacity(0);
	ccBlendFunc bf = {GL_ONE, GL_ONE};
	highLight->setBlendFunc(bf);
	
	brickSprite->addChild(highLight);
}

Brick::~Brick(){

}


void Brick::selectAnimation(){
	brickSprite->setOpacity(200);
	highLight->runAction(
		CCRepeatForever::actionWithAction((CCSequence*)CCSequence::actions( 
			CCFadeTo::actionWithDuration(0.4,255*0.5), 
			CCFadeOut::actionWithDuration(0.2), 
			NULL)));
}

void Brick::stopSelectAnimation(){
	brickSprite->setOpacity(255);
	highLight->stopAllActions();
	highLight->setOpacity(0);
}


void Brick::destoryAnimation(){
}