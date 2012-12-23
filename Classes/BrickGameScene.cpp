#include "BrickGameScene.h"
#include "GameStartScene.h"
#include "GameOverScene.h"
#include "Brick.h"

static const ccColor3B colorArray[] = {ccRED,ccBLUE,ccGREEN,ccMAGENTA,ccYELLOW};

CCScene* BrickGameScene::scene()
{
	 // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    BrickGameScene *layer = BrickGameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}



bool BrickGameScene::init()
{
	if ( !CCLayer::init() )
    {
        return false;
    }
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCMenuItemImage *pPauseItem = CCMenuItemImage::create(
                                        "pause_button.png",
                                        "pause_button.png",
                                        this,
                                        menu_selector(BrickGameScene::showPauseMenu));
	pPauseItem->setPosition(ccp(winSize.width-42,winSize.height-32));
	CCMenuItemToggle *pMuteItem = CCMenuItemToggle::itemWithTarget(
									this,
									menu_selector(BrickGameScene::toggleMute),
									CCMenuItemImage::create("mute_on.png","mute_on.png"),
									CCMenuItemImage::create("mute_off.png","mute_off.png"),
									NULL);
	pMuteItem->setPosition(ccp(280,65));
	CCMenu* pMenu = CCMenu::create(pPauseItem, pMuteItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

	/*CCSprite *pauseButton = CCSprite::create("pause_button.png", CCRectMake(0, 0, 85, 45));
	pauseButton->setPosition(ccp(winSize.width-42,winSize.height-32));
	this->addChild(pauseButton,10);*/

	CCSprite *scoreLabel = CCSprite::create("score_label.png", CCRectMake(0, 0, 249, 46));
	scoreLabel->setPosition(ccp(winSize.width-124,32));
	this->addChild(scoreLabel,10);

	CCSprite *gameBG = CCSprite::create("brick_bg.png", CCRectMake(0, 0, 320, 442));
	gameBG->setPosition(ccp(160,250));
	gameBG->setOpacity(220);
	gameBG->setTag(10000);
	this->addChild(gameBG,11);

	bonusSprite = CCSprite::create("bonus.png", CCRectMake(0,0,58,33));
	bonusSprite->setPosition(ccp(124,70));
	bonusSprite->setOpacity(0);
	this->addChild(bonusSprite,12);

	dangerSprite = CCSprite::create("danger.png", CCRectMake(0,0,122,34));
	dangerSprite->setPosition(ccp(76,480-32));
	dangerSprite->setOpacity(0);
	this->addChild(dangerSprite,13);
	dangerSignOpacity = 0;
	dangerBlink = CCRepeatForever::actionWithAction(
					(CCSequence*)CCSequence::actions(
						CCFadeTo::actionWithDuration(0.2,255),
						CCFadeOut::actionWithDuration(0.4),
						NULL)
					);
	
	rowOfBricks = new CCArray();
	for(int i=0;i<5;i++){
		CCArray *collumn = new CCArray();
		rowOfBricks->addObject(collumn);
	}

	timeBar = new TimeBar();
	CCLayer* timeContainer = timeBar->getTimeContainer();
	timeContainer->setPosition(ccp(0,34));
	
	this->addChild(timeContainer,9);

	brickContainer = CCLayer::create();
	brickContainer->setContentSize(CCSizeMake(320,325));
	brickContainer->setPosition(ccp(0,100));
	this->addChild(brickContainer,1000);

	touchMoved = false;
	
	this->setTouchEnabled(true);

	scoreTextLabel = CCLabelTTF::labelWithString(
		"0",
		CCSizeMake(320,30),
		kCCTextAlignmentRight,
		kCCVerticalTextAlignmentCenter,
		"fonts/impact.ttf",
		20);

	scoreTextLabel->setPosition(ccp(160,27));
	scoreTextLabel->setColor(ccc3(255,255,255));
	scoreTextLabel->setOpacity(255*0.8);
	this->addChild(scoreTextLabel,11);

	pauseColorLayer = new CCLayerColor();
	pauseColorLayer = CCLayerColor::create(ccc4(0,0,0,255*0.0),320,480);
	pauseColorLayer->setPosition(ccp(0,0));
	this->addChild(pauseColorLayer,99999);

	CCMenuItemImage *pResumeItem = CCMenuItemImage::create(
                                        "resume.png",
                                        "resume.png",
                                        this,
                                        menu_selector(BrickGameScene::dismissPauseMenu));
	
	pResumeItem->setPosition(ccp(200,198));
	pMenu2 = CCMenu::create(pResumeItem, NULL);
    pMenu2->setPosition(ccp(320,0));
	pMenu2->setOpacity(0);
    pauseColorLayer->addChild(pMenu2, 1);

	timeTick = 5.0f;
	countDownTime = 90;
	colorsCount = 3;
	score = 0;
	holdingBrick = false;
	startGame = false;
	gamePause = false;
	startRowCount = 5;

	CCParticleGalaxy* emitter = CCParticleGalaxy::create();
	emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("brick.png") );
	emitter->setPosition(ccp(160,240));
	this->addChild(emitter,-1000);

	CCPoint startPoint = ccp(32,325);
	generating = false;
	this->schedule(schedule_selector(BrickGameScene::generateBrick), 0.4, 4, 0);

	//this->redrawScene();

	this->schedule(schedule_selector(BrickGameScene::countDown), 1.0f, countDownTime, 0);
	
	grabbedBricks = new CCArray();
	return true;
}

void BrickGameScene::toggleMute(CCObject* sender){
	CCMenuItemToggle *item = dynamic_cast<CCMenuItemToggle *>(sender);
	if(item->getSelectedIndex()==0){
		CCLOG("MUTE OFF");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm.wav",true);
	}else{
		CCLOG("MUTE");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
}

void BrickGameScene::dangerFadeIn(CCNode* sender){
	//dangerSprite->runAction(CCFadeTo::actionWithDuration(0.2,dangerSignOpacity));
}

void BrickGameScene::showPauseMenu(CCObject* sender){
	if(!startGame){
		return;
	}
	gamePause = true;
	pMenu2->setPosition(ccp(0,0));
	pMenu2->runAction(CCFadeTo::actionWithDuration(0.4,255));
	pauseColorLayer->runAction(CCSequence::create(
		CCFadeTo::actionWithDuration(0.4,255*0.7),
		CCCallFuncN::create(this,callfuncN_selector(BrickGameScene::pasueGame)), 
		NULL));
}

void BrickGameScene::pasueGame(CCNode* sender){
	CCLOG("pasueGame");
	CCDirector::sharedDirector()->pause();
	this->pauseSchedulerAndActions();
}

void BrickGameScene::dismissPauseMenu(CCObject* sender){
	CCLOG("dismissPauseMenu");
	if(!gamePause){
		return;
	}
	gamePause = false;
	CCDirector::sharedDirector()->resume();
	this->resumeSchedulerAndActions();
	pMenu2->setPosition(ccp(320,0));
	pMenu2->runAction(CCFadeOut::actionWithDuration(0.4));
	pauseColorLayer->runAction(CCFadeOut::actionWithDuration(0.4));
}

void BrickGameScene::resumeGame(CCNode* sender){
	CCLOG("resumeGame");
}

void BrickGameScene::countDown(float dt){
	countDownTime-=1;
	timeBar->setTime(countDownTime);
	if(countDownTime<=15){
		timeDanger = true;
	}else{
		timeDanger = false;
	}

	if(!countDanger){
		if(timeDanger){
			dangerSprite->stopAllActions();
			dangerSprite->runAction(CCRepeatForever::actionWithAction(
					(CCSequence*)CCSequence::actions(
						CCFadeTo::actionWithDuration(0.2,255),
						CCFadeOut::actionWithDuration(0.4),
						NULL)
					));
			timeDanger = true;
		}else{
			dangerSprite->stopAllActions();
			dangerSprite->setOpacity(0);
			timeDanger = false;
		}
	}
	if(countDownTime==0){
		this->goToGameOverScene();
	}
}


void BrickGameScene::generateBrick(float dt){
	//this->setTouchEnabled(false);
	if(generating){
		return;
	}
	generating = true;
	for(int i=0;i<5;i++){
		CCArray* brickCol = dynamic_cast<CCArray*>(rowOfBricks->objectAtIndex(i));
		for(int j=0;j<brickCol->count();j++){
			Brick* bricks = dynamic_cast<Brick*>(brickCol->objectAtIndex(j));
			CCPoint spritePositon = bricks->getSprite()->getPosition();
			CCFiniteTimeAction* transit = CCMoveTo::actionWithDuration(0.2,ccp(spritePositon.x,spritePositon.y-28));
			bricks->getSprite()->runAction(transit);
		}
	}
	//this->redrawScene();
	CCPoint startPoint = ccp(32,325);
	for(int j=0;j<5;j++){
		Brick *bricks = new Brick();
		int randomIndex = rand()%colorsCount;
		ccColor3B tempColor = colorArray[randomIndex];
		bricks->tag = randomIndex;
		bricks->setColor(tempColor.r,tempColor.g,tempColor.b);
		CCSprite *target = bricks->getSprite();
		target->setOpacity(0);
		target->setPosition(ccp(startPoint.x,startPoint.y));
		startPoint.x += 64;
		brickContainer->addChild(target);
		dynamic_cast<CCArray*>(rowOfBricks->objectAtIndex(j))->insertObject(bricks,0);
		CCPoint spritePositon = bricks->getSprite()->getPosition();
		CCFiniteTimeAction* transit = CCMoveTo::actionWithDuration(0.2,ccp(spritePositon.x,spritePositon.y-25));
		if(j!=4){
			target->runAction(transit);
			target->runAction(CCFadeIn::actionWithDuration(0.2));
		}else{
			target->runAction(CCFadeIn::actionWithDuration(0.2));
			target->runAction(CCSequence::create(
										transit,
										CCCallFuncN::create(this, 
															callfuncN_selector(BrickGameScene::enableTouch)), 
										CCCallFuncN::create(this, 
															callfuncN_selector(BrickGameScene::redrawScene)),
										CCCallFuncN::create(this, 
															callfuncN_selector(BrickGameScene::isGameOver)), 
										NULL));
		}
	}
	if(!startGame){
		startRowCount -= 1;
		if(startRowCount == 0){
			startGame = true;
			CCLOG("Start Game");
			this->unschedule(schedule_selector(BrickGameScene::generateBrick));
			this->schedule(schedule_selector(BrickGameScene::gameLoop), timeTick, 1, 0);
		}
	}
	//this->isGameOver(NULL);
}

void BrickGameScene::enableTouch(CCNode* sender){
	//this->setTouchEnabled(true);
	generating = false;
}

void BrickGameScene::redrawScene(CCNode* sender){
	for(int i=0;i<5;i++){
		CCArray* brickCol = dynamic_cast<CCArray*>(rowOfBricks->objectAtIndex(i));
		for(int j=0;j<brickCol->count();j++){
			Brick* bricks = dynamic_cast<Brick*>(brickCol->objectAtIndex(j));
			bricks->getSprite()->setPosition(ccp(32+(64*i),325-25-(25*j)));
		}
	}
}

void BrickGameScene::gameLoop(float dt){
	this->generateBrick(dt);
	this->unschedule(schedule_selector(BrickGameScene::gameLoop));
	this->schedule(schedule_selector(BrickGameScene::gameLoop), timeTick, 1, 0);
}

void BrickGameScene::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent){
	if(gamePause || !startGame){
		return;
	}
	CCTouch *myTouch = (CCTouch*)pTouches->anyObject();
	CCPoint location = myTouch->locationInView();
	lastTocuh = location;
}

void BrickGameScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	if(gamePause || !startGame){
		return;
	}
	touchMoved = true;
	CCTouch *myTouch = (CCTouch*)pTouches->anyObject();
	CCPoint location = myTouch->locationInView();
}

void BrickGameScene::fastForward(){
	if(generating){
		return;
	}
	this->generateBrick(0.0f);
	this->unschedule(schedule_selector(BrickGameScene::gameLoop));
	this->schedule(schedule_selector(BrickGameScene::gameLoop), timeTick, 1, 0);
}

void BrickGameScene::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent){
	if(gamePause || !startGame){
		return;
	}
	
	CCTouch *myTouch = (CCTouch*)pTouches->anyObject();
	CCPoint location = myTouch->locationInView();

	if(touchMoved){
		if(abs(location.y-lastTocuh.y) > 30){
			CCLOG("Touch Moved");
			touchMoved = false;
			this->fastForward();
			return;
		}else{
			CCLOG("CCPOINT(Touch move<30) : {%f,%f}",location.x,location.y);
		}
	}else{
		lastTocuh = location;
		CCLOG("CCPOINT : {%f,%f}",location.x,location.y);
	}
	rowTouched = lastTocuh.x/64;
	if(rowTouched+1>rowOfBricks->count()){
		return;
	}
	if(rowTouched<0){
		rowTouched = 0;
	}
	CCLOG("ROW : %i",rowTouched);
	touchMoved = false;

	if(holdingBrick){
		//Put back brick
		int sameColorCount=0;
		int removeFrom = -1;
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("put_back.wav");
		
		for(int i=0;i<grabbedBricks->count();i++){
			Brick* temp = dynamic_cast<Brick*>(grabbedBricks->objectAtIndex(i));
			temp->stopSelectAnimation();
		}

		CCArray* selectedCol = dynamic_cast<CCArray*>(rowOfBricks->objectAtIndex(rowTouched));
		if(lastRowTouched!=rowTouched){
			CCArray* lastSelectedCol = dynamic_cast<CCArray*>(rowOfBricks->objectAtIndex(lastRowTouched));
			int lastIndex = selectedCol->count()-1;
			for(int i=0;i<grabbedBricks->count();i++){
				Brick* temp = dynamic_cast<Brick*>(grabbedBricks->objectAtIndex(i));
				selectedCol->addObject(temp);
				
				temp->brickSprite->runAction(CCFadeOut::actionWithDuration(0.2f));
				lastSelectedCol->removeObject(temp);
				temp->brickSprite->setPosition(ccp(32+(64*rowTouched),325-25-(25*selectedCol->indexOfObject(temp))));
				temp->brickSprite->runAction(CCFadeIn::actionWithDuration(0.2f));
			}
		}
		

		for(int i=selectedCol->count()-1;i>=0;--i){
			Brick* temp = dynamic_cast<Brick*>(selectedCol->objectAtIndex(i));
			if(selectedColor == temp->tag){
				sameColorCount +=1;
			}else{
				removeFrom = i+1;
				break;
			}
		}

		//Delete brick
		if(sameColorCount>=3){
			if(removeFrom == -1){
				removeFrom = 0;
			}
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("clear.wav");
			for(int i=selectedCol->count()-1;i>=removeFrom;--i){
				Brick* temp = dynamic_cast<Brick*>(selectedCol->objectAtIndex(i));
				temp->brickSprite->runAction(
					cocos2d::CCEaseBounceOut::actionWithAction(
						CCMoveTo::actionWithDuration(0.4,ccp(temp->brickSprite->getPositionX(),0))
					));
				//temp->brickSprite->runAction(CCScaleTo::actionWithDuration(0.4,0,0));
				temp->brickSprite->runAction(
					CCSequence::actions(
						CCFadeOut::actionWithDuration(0.4), 
						CCCallFuncN::actionWithTarget(this,callfuncN_selector(BrickGameScene::removeBrick)), 
						NULL));
				score += 50;

				if(score >= 1500 && score < 2999){
					colorsCount = 4;
					timeTick = 4.0f;
				}else if(score >= 3000 && score < 4499){
					timeTick = 3.5f;
				}else if(score >= 4500 && score < 5999){
					timeTick = 3.0f;
					colorsCount = 5;
				}else if(score >= 6000 && score < 7499){
					timeTick = 2.5f;
				}

				char tempc[20];
				sprintf(tempc,"%d",score);
				std::string str1 = "";
				str1 = tempc;

				scoreTextLabel->setString(str1.c_str());
				CCEaseBounceOut *scale1 = CCEaseBounceOut::actionWithAction(CCScaleTo::actionWithDuration(0.4,1.0,1.8));
				CCEaseOut *scale2 = CCEaseOut::actionWithAction(CCScaleTo::actionWithDuration(0.4,1.0,1.0),1.0);
				scoreTextLabel->runAction(
						CCSequence::actions(
							scale1,
							scale2,
							NULL)
					);
				selectedCol->removeObjectAtIndex(i);

				
			}
		}

		if(sameColorCount>=4){
			countDownTime +=5;
			timeBar->setTime(countDownTime);
			bonusSprite->setOpacity(255);
			bonusSprite->runAction(CCSequence::actions(
				CCFadeIn::actionWithDuration(0.2),
				CCFadeOut::actionWithDuration(0.2),
				NULL));
			bonusSprite->runAction(CCSequence::actions(
				CCMoveTo::actionWithDuration(0.4,ccp(bonusSprite->getPositionX(),480-378)),
				CCCallFuncN::actionWithTarget(this,callfuncN_selector(BrickGameScene::resetBonus)),
				NULL));

		}

		holdingBrick = false;
		grabbedBricks->removeAllObjects();

		this->isGameOver(NULL);

	}else{
		//Hold brick
		CCArray* selectedCol = dynamic_cast<CCArray*>(rowOfBricks->objectAtIndex(rowTouched));
		if(selectedCol->count()==0){
			return;
		}
		Brick* temp = dynamic_cast<Brick*>(selectedCol->objectAtIndex(selectedCol->count()-1));
		CCLOG("Selected Color(tag) : {%i,%i,%i}(%i)",temp->brickColor.r,temp->brickColor.g,temp->brickColor.b,temp->tag);
		selectedColor = temp->tag;
		int tempIndex = selectedCol->count()-1;

		for(int i=tempIndex;i>=0;--i){
			Brick* tempBrick = dynamic_cast<Brick*>(selectedCol->objectAtIndex(i));
			if(selectedColor == tempBrick->tag){
				grabbedBricks->addObject(tempBrick);
				tempBrick->selectAnimation();
			}else{
				break;
			}
		}

		if(grabbedBricks->count()>0){
			holdingBrick=true;
			lastRowTouched = rowTouched;
		}
	}
}

void BrickGameScene::resetBonus(CCNode* sender){
	bonusSprite->setPosition(ccp(124,70));
	bonusSprite->setOpacity(0);
}

void BrickGameScene::goToGameOverScene(){
	CCScene *s = GameOverScene::scene();
	CCDirector::sharedDirector()->setDepthTest(true);
	CCTransitionScene *transition = CCTransitionSplitRows::create(0.5f, s);
	CCDirector::sharedDirector()->replaceScene(transition);
	
}

void BrickGameScene::isGameOver(CCNode* sender){
	int maxCount = 0;
	for(int i=0;i<rowOfBricks->count();i++){
		CCArray* selectedCol = dynamic_cast<CCArray*>(rowOfBricks->objectAtIndex(i));
		if(selectedCol->count()>=14){
			CCLOG("Game Over");
			CCUserDefault::sharedUserDefault()->setIntegerForKey("CurrentScore", score);
			CCObject* it = NULL;
			goToGameOverScene();
			return;
			
		}
		if(selectedCol->count()>maxCount){
			maxCount = selectedCol->count();
		}
	}

	if(maxCount>=11){
		countDanger = true;
	}else{
		countDanger = false;
	}

	if(startGame){
		if(!timeDanger){
			if(countDanger){
				dangerSprite->stopAllActions();
				dangerSprite->runAction(CCRepeatForever::actionWithAction(
						(CCSequence*)CCSequence::actions(
							CCFadeTo::actionWithDuration(0.2,255),
							CCFadeOut::actionWithDuration(0.4),
							NULL)
						));
			}else{
				dangerSprite->stopAllActions();
				dangerSprite->setOpacity(0);
			}
		}
		
	}
	
	//return false;
}

void BrickGameScene::removeBrick(CCNode* sender)
{
  CCSprite *sprite = (CCSprite *)sender;
  this->removeChild(sprite, true);
}
