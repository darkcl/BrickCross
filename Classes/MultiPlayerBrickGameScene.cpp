#include "MultiPlayerBrickGameScene.h"
#include "GameStartScene.h"
#include "GameOverScene.h"
#include "MultiPlayConstant.h"
#include "Brick.h"
#include "AppDelegate.h"

#define COCOS2D_DEBUG 1
static const ccColor3B colorArray[] = {ccRED,ccBLUE,ccGREEN,ccMAGENTA,ccYELLOW};

CCScene* MultiPlayerBrickGameScene::scene()
{
	 // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MultiPlayerBrickGameScene *layer = MultiPlayerBrickGameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}



bool MultiPlayerBrickGameScene::init()
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
                                        menu_selector(MultiPlayerBrickGameScene::showPauseMenu));
	pPauseItem->setPosition(ccp(winSize.width-42,winSize.height-32));
	CCMenuItemToggle *pMuteItem = CCMenuItemToggle::itemWithTarget(
									this,
									menu_selector(MultiPlayerBrickGameScene::toggleMute),
									CCMenuItemImage::create("mute_on.png","mute_on.png"),
									CCMenuItemImage::create("mute_off.png","mute_off.png"),
									NULL);
	pMuteItem->setPosition(ccp(280,65));
	CCMenu* pMenu = CCMenu::create(pPauseItem, pMuteItem, NULL);
    pMenu->setPosition( CCPointZero );
    //this->addChild(pMenu, 1);

	/*CCSprite *pauseButton = CCSprite::create("pause_button.png", CCRectMake(0, 0, 85, 45));
	pauseButton->setPosition(ccp(winSize.width-42,winSize.height-32));
	this->addChild(pauseButton,10);*/

	CCSprite *scoreLabel = CCSprite::create("score_label.png", CCRectMake(0, 0, 249, 46));
	scoreLabel->setPosition(ccp(winSize.width-124,32));
	scoreLabel->setOpacity(0);
	this->addChild(scoreLabel,10);

	CCSprite *gameBG = CCSprite::create("brick_bg.png", CCRectMake(0, 0, 320, 442));
	gameBG->setPosition(ccp(160,250));
	gameBG->setOpacity(220);
	gameBG->setTag(10000);
	this->addChild(gameBG,11);

	/*bonusSprite = CCSprite::create("bonus.png", CCRectMake(0,0,58,33));
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
					);*/
	
	rowOfBricks = new CCArray();
	for(int i=0;i<5;i++){
		CCArray *collumn = new CCArray();
		rowOfBricks->addObject(collumn);
	}

	/*timeBar = new TimeBar();
	CCLayer* timeContainer = timeBar->getTimeContainer();
	timeContainer->setPosition(ccp(0,34));*/
	
	//this->addChild(timeContainer,9);

	brickContainer = CCLayer::create();
	brickContainer->setContentSize(CCSizeMake(320,325));
	brickContainer->setPosition(ccp(0,100));
	this->addChild(brickContainer,1000);

	touchMoved = false;
	
	this->setTouchEnabled(true);

	/*scoreTextLabel = CCLabelTTF::labelWithString(
		"0",
		CCSizeMake(320,30),
		kCCTextAlignmentRight,
		kCCVerticalTextAlignmentCenter,
		"fonts/impact.ttf",
		20);

	scoreTextLabel->setPosition(ccp(160,27));
	scoreTextLabel->setColor(ccc3(255,255,255));
	scoreTextLabel->setOpacity(0);
	this->addChild(scoreTextLabel,11);*/

	/*pauseColorLayer = new CCLayerColor();
	pauseColorLayer = CCLayerColor::create(ccc4(0,0,0,255*0.0),320,480);
	pauseColorLayer->setPosition(ccp(0,0));
	this->addChild(pauseColorLayer,99999);

	CCMenuItemImage *pResumeItem = CCMenuItemImage::create(
                                        "resume.png",
                                        "resume.png",
                                        this,
                                        menu_selector(MultiPlayerBrickGameScene::dismissPauseMenu));
	pResumeItem->setPosition(ccp(200,198));
	pMenu2 = CCMenu::create(pResumeItem, NULL);
    pMenu2->setPosition(ccp(320,0));
	pMenu2->setOpacity(0);
    pauseColorLayer->addChild(pMenu2, 1);*/

	timeTick = 5.0f;
	countDownTime = 90;
	colorsCount = 3;
	score = 0;
	holdingBrick = false;
	startGame = false;
	gamePause = false;
	startRowCount = 5;

	CCPoint startPoint = ccp(32,325);
	generating = false;
	//this->schedule(schedule_selector(MultiPlayerBrickGameScene::generateBrick), 0.4, 4, 0);

	//this->redrawScene();

	//this->schedule(schedule_selector(MultiPlayerBrickGameScene::countDown), 1.0f, countDownTime, 0);
	grabbedBricks = new CCArray();
	//Network Init
	multiplayerSprite1 = CCMenuItemImage::create("connecting.png","create_room.png");
	multiplayerSprite1->setPosition(ccp(160,328.5));

	multiplayerSprite2 = CCMenuItemImage::create("join_room.png","join_room.png");
	multiplayerSprite2->setPosition(ccp(160,188.5));
	multiplayerSprite2->setOpacity(0);
	pMultiPlayMenu = CCMenu::create(multiplayerSprite1,multiplayerSprite2, NULL);
	pMultiPlayMenu->setPosition(0,0);
	this->addChild(pMultiPlayMenu,1001);

	networkLogic = new NetworkLogic(this,"1.0.0.0");
	networkLogic->registerForStateUpdates(this);
	myInfo = new Player();
	myInfo->setHpSprite(true);
	myInfo->setLabel(true);

	this->addChild(myInfo->hpSprite,1001);
	this->addChild(myInfo->hpLabel,1002);

	oppInfo = new Player();
	oppInfo->setHpSprite(false);
	oppInfo->setLabel(false);
	this->addChild(oppInfo->hpSprite,1001);
	this->addChild(oppInfo->hpLabel,1002);

	networkLogic->attachToGameScene(this);
	this->schedule(schedule_selector(MultiPlayerBrickGameScene::networkLoop), 0.33f, -1, 0);
	
	CCParticleGalaxy* emitter = CCParticleGalaxy::create();
	emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("brick.png") );
	emitter->setPosition(ccp(160,240));
	this->addChild(emitter,-1000);

	AppDelegate *appDelegate = (AppDelegate *)(CCApplication::sharedApplication());
	appDelegate->inMultiPlayer = true;
	appDelegate->networkLogic = networkLogic;
	return true;
}

//Network Logic
void MultiPlayerBrickGameScene::stateUpdate(State newState)
{
	CCSprite *createRoomImage = CCSprite::create("create_room.png");
	
	switch(newState){
	case STATE_CONNECTED:
		multiplayerSprite1->setNormalImage(createRoomImage);
		multiplayerSprite1->setSelectedImage(createRoomImage);
		multiplayerSprite2->runAction(CCFadeIn::actionWithDuration(0.5));
		multiplayerSprite1->setTarget(this,menu_selector(MultiPlayerBrickGameScene::createRoom));
		multiplayerSprite2->setTarget(this,menu_selector(MultiPlayerBrickGameScene::joinRoom));
		break;
	case STATE_JOINING:
		
		break;
	case STATE_JOINED:
		CCLOG("JOINED A GAME");
		//networkLogic->sendEvent(EV_PLAYERINFO);
		break;
	default:
		break;
	}
}

void MultiPlayerBrickGameScene::createRoom(CCObject* sender){
	networkLogic->opCreateRoom();
	CCSpriteFrame *searchingImage = CCSpriteFrame::create("searching.png",CCRectMake(0,0,375,211));
	CCSpriteFrame *userDataImage = CCSpriteFrame::create("user_data.png",CCRectMake(0,0,375,211));
	multiplayerSprite1->setNormalSpriteFrame(searchingImage);
	multiplayerSprite1->setSelectedSpriteFrame(searchingImage);
	multiplayerSprite1->setEnabled(false);

	multiplayerSprite2->setNormalSpriteFrame(userDataImage);
	multiplayerSprite2->setSelectedSpriteFrame(userDataImage);
	multiplayerSprite2->setEnabled(false);
}

void MultiPlayerBrickGameScene::joinRoom(CCObject* sender){
	networkLogic->opJoinRandomRoom();
	CCSpriteFrame *searchingImage = CCSpriteFrame::create("searching.png",CCRectMake(0,0,375,211));
	CCSpriteFrame *userDataImage = CCSpriteFrame::create("user_data.png",CCRectMake(0,0,375,211));
	multiplayerSprite1->setNormalSpriteFrame(searchingImage);
	multiplayerSprite1->setSelectedSpriteFrame(searchingImage);
	multiplayerSprite1->setEnabled(false);

	multiplayerSprite2->setNormalSpriteFrame(userDataImage);
	multiplayerSprite2->setSelectedSpriteFrame(userDataImage);
	multiplayerSprite2->setEnabled(false);
}

void MultiPlayerBrickGameScene::oppDisconnect(){
	AppDelegate *appDelegate = (AppDelegate *)(CCApplication::sharedApplication());
	appDelegate->inMultiPlayer = false;
	networkLogic->disconnect();
	CCLabelTTF *winLabel = CCLabelTTF::labelWithString(
		"YOU WIN",
		CCSizeMake(320,480),
		kCCTextAlignmentCenter,
		kCCVerticalTextAlignmentCenter,
		"fonts/impact.ttf",
		72);
	winLabel->setColor(ccc3(0,0,0));
	winLabel->setPosition(ccp(160,240));
	this->addChild(winLabel,1001);
	winLabel->setOpacity(0);

	CCObject* it = NULL;
	CCArray *bricksArray = brickContainer->getChildren();
	CCARRAY_FOREACH(bricksArray, it){
		CCSprite *brick = dynamic_cast<CCSprite*>(it);
		brick->runAction(
					cocos2d::CCEaseBounceOut::actionWithAction(
						CCMoveTo::actionWithDuration(0.4,ccp(brick->getPositionX(),12.5))
					));
		brick->runAction(CCSequence::actions(
						CCFadeOut::actionWithDuration(0.4), 
						CCCallFuncN::actionWithTarget(this,callfuncN_selector(MultiPlayerBrickGameScene::removeBrick)), 
						NULL));
	}
	winLabel->runAction(CCSequence::actions(
		CCFadeIn::actionWithDuration(3.0),
		CCCallFuncN::actionWithTarget(this,callfuncN_selector(MultiPlayerBrickGameScene::showWin)), 
						NULL));
}

void MultiPlayerBrickGameScene::showWin(CCNode* sender){
	CCScene *s = GameStartScene::scene();
	CCDirector::sharedDirector()->replaceScene(s);
}

void MultiPlayerBrickGameScene::networkLoop(float dt)
{
	networkLogic->run();
}


void MultiPlayerBrickGameScene::onExit()
{
	networkLogic->disconnect();
}

void MultiPlayerBrickGameScene::write(const ExitGames::Common::JString& str)
{
	CCLOG(str.UTF8Representation().cstr());
}

void MultiPlayerBrickGameScene::writeLine(const ExitGames::Common::JString& str)
{
	write(str);
	CCLOG("\n");
}

void MultiPlayerBrickGameScene::netStartGame(){
	this->removeChild(pMultiPlayMenu, true);
	oppInfo->setHp(oppInfo->hp);
	myInfo->setHp(myInfo->hp);
	//oppInfo->hpLabel->setString(oppInfo->user_name);
	this->schedule(schedule_selector(MultiPlayerBrickGameScene::generateBrick), 0.4, 4, 0);
}


//Game Logic
void MultiPlayerBrickGameScene::toggleMute(CCObject* sender){
	CCMenuItemToggle *item = dynamic_cast<CCMenuItemToggle *>(sender);
	if(item->getSelectedIndex()==0){
		CCLOG("MUTE OFF");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm.wav",true);
	}else{
		CCLOG("MUTE");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
}

void MultiPlayerBrickGameScene::dangerFadeIn(CCNode* sender){
	//dangerSprite->runAction(CCFadeTo::actionWithDuration(0.2,dangerSignOpacity));
}

void MultiPlayerBrickGameScene::showPauseMenu(CCObject* sender){
	/*if(!startGame){
		return;
	}
	gamePause = true;
	pMenu2->setPosition(ccp(0,0));
	pMenu2->runAction(CCFadeTo::actionWithDuration(0.4,255));
	pauseColorLayer->runAction(CCSequence::create(
		CCFadeTo::actionWithDuration(0.4,255*0.7),
		CCCallFuncN::create(this,callfuncN_selector(MultiPlayerBrickGameScene::pasueGame)), 
		NULL));*/
	networkLogic->setLastInput(INPUT_CREATE_GAME);
}

void MultiPlayerBrickGameScene::pasueGame(CCNode* sender){
	CCLOG("pasueGame");
	CCDirector::sharedDirector()->pause();
	this->pauseSchedulerAndActions();
}

void MultiPlayerBrickGameScene::dismissPauseMenu(CCObject* sender){
	CCLOG("dismissPauseMenu");
	/*if(!gamePause){
		return;
	}
	gamePause = false;
	CCDirector::sharedDirector()->resume();
	this->resumeSchedulerAndActions();
	pMenu2->setPosition(ccp(320,0));
	pMenu2->runAction(CCFadeOut::actionWithDuration(0.4));
	pauseColorLayer->runAction(CCFadeOut::actionWithDuration(0.4));*/
}

void MultiPlayerBrickGameScene::resumeGame(CCNode* sender){
	CCLOG("resumeGame");
}

void MultiPlayerBrickGameScene::countDown(float dt){
	countDownTime-=1;
	//timeBar->setTime(countDownTime);
	if(countDownTime<=15){
		timeDanger = true;
	}else{
		timeDanger = false;
	}

	if(!countDanger){
		if(timeDanger){
			/*dangerSprite->stopAllActions();
			dangerSprite->runAction(CCRepeatForever::actionWithAction(
					(CCSequence*)CCSequence::actions(
						CCFadeTo::actionWithDuration(0.2,255),
						CCFadeOut::actionWithDuration(0.4),
						NULL)
					));*/
			timeDanger = true;
		}else{
			/*dangerSprite->stopAllActions();
			dangerSprite->setOpacity(0);*/
			timeDanger = false;
		}
	}
	if(countDownTime==0){
		this->goToGameOverScene();
	}
}


void MultiPlayerBrickGameScene::generateBrick(float dt){
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
															callfuncN_selector(MultiPlayerBrickGameScene::enableTouch)), 
										CCCallFuncN::create(this, 
															callfuncN_selector(MultiPlayerBrickGameScene::redrawScene)),
										CCCallFuncN::create(this, 
															callfuncN_selector(MultiPlayerBrickGameScene::isGameOver)), 
										NULL));
		}
	}
	if(!startGame){
		startRowCount -= 1;
		if(startRowCount == 0){
			startGame = true;
			CCLOG("Start Game");
			this->unschedule(schedule_selector(MultiPlayerBrickGameScene::generateBrick));
			this->schedule(schedule_selector(MultiPlayerBrickGameScene::gameLoop), timeTick, 1, 0);
		}
	}
	//this->isGameOver(NULL);
}

void MultiPlayerBrickGameScene::enableTouch(CCNode* sender){
	//this->setTouchEnabled(true);
	generating = false;
}

void MultiPlayerBrickGameScene::redrawScene(CCNode* sender){
	for(int i=0;i<5;i++){
		CCArray* brickCol = dynamic_cast<CCArray*>(rowOfBricks->objectAtIndex(i));
		for(int j=0;j<brickCol->count();j++){
			Brick* bricks = dynamic_cast<Brick*>(brickCol->objectAtIndex(j));
			bricks->getSprite()->setPosition(ccp(32+(64*i),325-25-(25*j)));
		}
	}
}

void MultiPlayerBrickGameScene::gameLoop(float dt){
	this->generateBrick(dt);
	this->unschedule(schedule_selector(MultiPlayerBrickGameScene::gameLoop));
	this->schedule(schedule_selector(MultiPlayerBrickGameScene::gameLoop), timeTick, 1, 0);
}

void MultiPlayerBrickGameScene::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent){
	if(gamePause || !startGame){
		return;
	}
	CCTouch *myTouch = (CCTouch*)pTouches->anyObject();
	CCPoint location = myTouch->locationInView();
	lastTocuh = location;
}

void MultiPlayerBrickGameScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent){
	if(gamePause || !startGame){
		return;
	}
	touchMoved = true;
	CCTouch *myTouch = (CCTouch*)pTouches->anyObject();
	CCPoint location = myTouch->locationInView();
}

void MultiPlayerBrickGameScene::fastForward(){
	if(generating){
		return;
	}
	this->generateBrick(0.0f);
	this->unschedule(schedule_selector(MultiPlayerBrickGameScene::gameLoop));
	this->schedule(schedule_selector(MultiPlayerBrickGameScene::gameLoop), timeTick, 1, 0);
}

void MultiPlayerBrickGameScene::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent){
	if(gamePause || !startGame){
		return;
	}
	
	CCTouch *myTouch = (CCTouch*)pTouches->anyObject();
	CCPoint location = myTouch->locationInView();

	if(touchMoved){
		if(location.y-lastTocuh.y > 30){
			CCLOG("Touch Moved");
			touchMoved = false;
			//this->fastForward();
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
						CCCallFuncN::actionWithTarget(this,callfuncN_selector(MultiPlayerBrickGameScene::removeBrick)), 
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

				/*char tempc[20];
				sprintf(tempc,"%d",score);
				std::string str1 = "";
				str1 = tempc;*/

				//scoreTextLabel->setString(str1.c_str());
				/*CCEaseBounceOut *scale1 = CCEaseBounceOut::actionWithAction(CCScaleTo::actionWithDuration(0.4,1.0,1.8));
				CCEaseOut *scale2 = CCEaseOut::actionWithAction(CCScaleTo::actionWithDuration(0.4,1.0,1.0),1.0);
				scoreTextLabel->runAction(
						CCSequence::actions(
							scale1,
							scale2,
							NULL)
					);*/
				selectedCol->removeObjectAtIndex(i);
			}
		}

		if(sameColorCount>=4){
			/*countDownTime +=5;
			timeBar->setTime(countDownTime);
			bonusSprite->setOpacity(255);
			bonusSprite->runAction(CCSequence::actions(
				CCFadeIn::actionWithDuration(0.2),
				CCFadeOut::actionWithDuration(0.2),
				NULL));
			bonusSprite->runAction(CCSequence::actions(
				CCMoveTo::actionWithDuration(0.4,ccp(bonusSprite->getPositionX(),480-378)),
				CCCallFuncN::actionWithTarget(this,callfuncN_selector(MultiPlayerBrickGameScene::resetBonus)),
				NULL));*/

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

void MultiPlayerBrickGameScene::resetBonus(CCNode* sender){
	/*bonusSprite->setPosition(ccp(124,70));
	bonusSprite->setOpacity(0);*/
}

void MultiPlayerBrickGameScene::goToGameOverScene(){
	CCScene *s = GameOverScene::scene();
	CCDirector::sharedDirector()->setDepthTest(true);
	CCTransitionScene *transition = CCTransitionSplitRows::create(0.5f, s);
	CCDirector::sharedDirector()->replaceScene(transition);
	
}

void MultiPlayerBrickGameScene::resetGameScene(){
	startGame = false;
	touchMoved = false;
	gamePause = false;
	generating = false;
	holdingBrick = false;
	startRowCount = 5;
	this->unschedule(schedule_selector(MultiPlayerBrickGameScene::generateBrick));

	for(int i=0;i<this->rowOfBricks->count();i++){
		CCArray *bricksArray = dynamic_cast<CCArray*>(rowOfBricks->objectAtIndex(i));
		for(int j=0;j<bricksArray->count();j++){
			Brick *temp = dynamic_cast<Brick*>(bricksArray->objectAtIndex(j));
			temp->stopSelectAnimation();
		}
	}

	CCObject* it = NULL;
	CCArray *bricksArray = brickContainer->getChildren();
	CCARRAY_FOREACH(bricksArray, it){
		CCSprite *brick = dynamic_cast<CCSprite*>(it);
		brick->runAction(
					cocos2d::CCEaseBounceOut::actionWithAction(
						CCMoveTo::actionWithDuration(0.4,ccp(brick->getPositionX(),12.5))
					));
		brick->runAction(CCSequence::actions(
						CCFadeOut::actionWithDuration(0.4), 
						CCCallFuncN::actionWithTarget(this,callfuncN_selector(MultiPlayerBrickGameScene::removeBrick)), 
						NULL));
	}
	for(int i=0;i<this->rowOfBricks->count();i++){
		CCArray *bricksArray1 = dynamic_cast<CCArray *>(rowOfBricks->objectAtIndex(i));
		bricksArray1->removeAllObjects();
	}
	this->netStartGame();
}

void MultiPlayerBrickGameScene::showLose(CCNode* sender){
	AppDelegate *appDelegate = (AppDelegate *)(CCApplication::sharedApplication());
	appDelegate->inMultiPlayer = false;
	networkLogic->disconnect();
	CCLabelTTF *winLabel = CCLabelTTF::labelWithString(
		"YOU LOSE",
		CCSizeMake(320,480),
		kCCTextAlignmentCenter,
		kCCVerticalTextAlignmentCenter,
		"fonts/impact.ttf",
		72);
	winLabel->setColor(ccc3(0,0,0));
	winLabel->setPosition(ccp(160,240));
	this->addChild(winLabel,1001);
	winLabel->setOpacity(0);

	CCObject* it = NULL;
	CCArray *bricksArray = brickContainer->getChildren();
	CCARRAY_FOREACH(bricksArray, it){
		CCSprite *brick = dynamic_cast<CCSprite*>(it);
		brick->runAction(
					cocos2d::CCEaseBounceOut::actionWithAction(
						CCMoveTo::actionWithDuration(0.4,ccp(brick->getPositionX(),12.5))
					));
		brick->runAction(CCSequence::actions(
						CCFadeOut::actionWithDuration(0.4), 
						CCCallFuncN::actionWithTarget(this,callfuncN_selector(MultiPlayerBrickGameScene::removeBrick)), 
						NULL));
	}
	winLabel->runAction(CCSequence::actions(
		CCFadeIn::actionWithDuration(3.0),
		CCCallFuncN::actionWithTarget(this,callfuncN_selector(MultiPlayerBrickGameScene::showWin)), 
						NULL));
}

void MultiPlayerBrickGameScene::isGameOver(CCNode* sender){
	int maxCount = 0;
	for(int i=0;i<rowOfBricks->count();i++){
		CCArray* selectedCol = dynamic_cast<CCArray*>(rowOfBricks->objectAtIndex(i));
		if(selectedCol->count()>=14){
			/*CCLOG("Game Over");
			CCUserDefault::sharedUserDefault()->setIntegerForKey("CurrentScore", score);
			CCObject* it = NULL;
			goToGameOverScene();*/

			//-hp
			int health = myInfo->hp;
			health -=25;
			myInfo->setHp(health);
			if(health>0){
				networkLogic->sendEvent(EV_HPCHANGE);
				resetGameScene();
			}else{
				networkLogic->sendEvent(EV_LOST);
				this->showLose(NULL);
			}
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
				/*dangerSprite->stopAllActions();
				dangerSprite->runAction(CCRepeatForever::actionWithAction(
						(CCSequence*)CCSequence::actions(
							CCFadeTo::actionWithDuration(0.2,255),
							CCFadeOut::actionWithDuration(0.4),
							NULL)
						));*/
			}else{
				/*dangerSprite->stopAllActions();
				dangerSprite->setOpacity(0);*/
			}
		}
		
	}
	
	//return false;
}

void MultiPlayerBrickGameScene::removeBrick(CCNode* sender)
{
  CCSprite *sprite = (CCSprite *)sender;
  this->removeChild(sprite, true);
}
