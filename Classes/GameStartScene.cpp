#include "GameStartScene.h"
#include "MultiPlayerBrickGameScene.h"
#include "BrickGameScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"
#include "Brick.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* GameStartScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameStartScene *layer = GameStartScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameStartScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCMenuItemImage *pSinglePlayItem = CCMenuItemImage::create(
                                        "singleplay.png",
                                        "singleplay.png",
                                        this,
                                        menu_selector(GameStartScene::startSingleGame) );
	pSinglePlayItem->setPosition(ccp(186.5,92));
	CCMenuItemImage *pMultiPlayItem = CCMenuItemImage::create(
                                        "multiplay.png",
                                        "multiplay.png",
                                        this,
                                        menu_selector(GameStartScene::startGame) );
    pMultiPlayItem->setPosition(ccp(221.5,157.5));
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pSinglePlayItem,pMultiPlayItem,NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
    
	CCSprite *logoSprite = CCSprite::create("logo.png", CCRectMake(0, 0, 260, 121));
	logoSprite->setPosition(ccp(130,480-90));
	CCParticleGalaxy* emitter = CCParticleGalaxy::create();
	emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("brick.png") );
	emitter->setPosition(ccp(160,240));
	this->addChild(emitter,-1000);

	this->addChild(logoSprite,100);
	this->schedule(schedule_selector(GameStartScene::gameLoop), 0.33);

    return true;
}

void GameStartScene::testPhoton_lib(CCObject* pSender){
}

void GameStartScene::gameLoop(float dt){
	this->addBrick();
}

void GameStartScene::spriteMoveFinished(CCNode* sender)
{
  CCSprite *sprite = (CCSprite *)sender;
  this->removeChild(sprite, true);
}

void GameStartScene::addBrick(){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	Brick *bricks = new Brick();
	bricks->setColor(rand()%255,rand()%255,rand()%255);
	CCSprite *target = bricks->getSprite();
	int randomRange = (winSize.height-target->getContentSize().height/2) - (target->getContentSize().height/2);
	int randomY = (rand()%randomRange) + (target->getContentSize().height/2);
	target->setPosition(ccp(winSize.width+target->getContentSize().width/2,randomY));
	target->setScale(rand()%2 + 0.5);
	target->setOpacity(rand()%255);
	this->addChild(target);
	randomRange = 3;
	int randomDuration = (rand()%randomRange) + 2;

	CCFiniteTimeAction* actionMove = CCMoveTo::actionWithDuration((float)randomDuration, ccp(0 - target->getContentSize().width/2, randomY));
	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::actionWithTarget(this,callfuncN_selector(GameStartScene::spriteMoveFinished));
	target->runAction( CCSequence::actions(actionMove, actionMoveDone, NULL));
}


void GameStartScene::startGame(CCObject* pSender)
{
	CCScene *s = MultiPlayerBrickGameScene::scene();
	CCDirector::sharedDirector()->setDepthTest(true);
	CCTransitionScene *transition = CCTransitionSplitRows::create(0.5f, s);
	CCDirector::sharedDirector()->replaceScene(transition);

}

void GameStartScene::startSingleGame(CCObject* pSender)
{
	CCScene *s = BrickGameScene::scene();
	CCDirector::sharedDirector()->setDepthTest(true);
	CCTransitionScene *transition = CCTransitionSplitRows::create(0.5f, s);
	CCDirector::sharedDirector()->replaceScene(transition);

}


