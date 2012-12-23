#include "GameOverScene.h"
#include "BrickGameScene.h"
#include "GameStartScene.h"

CCScene* GameOverScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameOverScene *layer = GameOverScene::create();

    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

	CCSprite *gameBG = CCSprite::create("brick_bg.png", CCRectMake(0, 0, 320, 442));
	gameBG->setPosition(ccp(160,250));
	gameBG->setOpacity(220);
	this->addChild(gameBG);

	CCSprite *gameOverLabel = CCSprite::create("game_over.png", CCRectMake(0, 0, 265, 56));
	gameOverLabel->setPosition(ccp(135,383));
	this->addChild(gameOverLabel);

	CCMenuItemImage *pRetryItem = CCMenuItemImage::create(
                                        "retry_button.png",
                                        "retry_button.png",
                                        this,
                                        menu_selector(GameOverScene::retryGame));
	pRetryItem->setPosition(ccp(210,200));
	CCMenuItemImage *pQuitItem = CCMenuItemImage::create(
                                        "quit_button.png",
                                        "quit_button.png",
                                        this,
                                        menu_selector(GameOverScene::quitGame));
	pQuitItem->setPosition(ccp(100,132));

	CCMenu* pMenu = CCMenu::create(pRetryItem,pQuitItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
	score = CCUserDefault::sharedUserDefault()->getIntegerForKey("CurrentScore",-1);
	char tempc[20];
	sprintf(tempc,"Score : %d",score);
	std::string str1 = "";
	str1 = tempc;
	scoreTextLabel = CCLabelTTF::labelWithString(
		str1.c_str(),
		CCSizeMake(320,70),
		kCCTextAlignmentCenter,
		kCCVerticalTextAlignmentCenter,
		"fonts/impact.ttf",
		45);

	scoreTextLabel->setPosition(ccp(160,334));
	scoreTextLabel->setColor(ccc3(68,77,99));
	scoreTextLabel->setOpacity(255*0.8);
	this->addChild(scoreTextLabel,11);

	
	return true;
}

void GameOverScene::retryGame(CCObject* sender){
	CCScene *s = BrickGameScene::scene();
	CCDirector::sharedDirector()->setDepthTest(true);
	CCTransitionScene *transition = CCTransitionSplitRows::create(0.5f, s);
	CCDirector::sharedDirector()->replaceScene(transition);
}

void GameOverScene::quitGame(CCObject* sender){
	CCScene *s = GameStartScene::scene();
	CCDirector::sharedDirector()->setDepthTest(true);
	CCTransitionScene *transition = CCTransitionSplitRows::create(0.5f, s);
	CCDirector::sharedDirector()->replaceScene(transition);
}