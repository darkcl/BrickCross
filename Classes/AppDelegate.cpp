#include "AppDelegate.h"
#include "GameStartScene.h"
#include "NetworkLogic.h"
#include "SimpleAudioEngine.h" 
#include "MultiPlayConstant.h"
USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}


bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
	CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
	CCSize designSize = CCSizeMake(320, 480);
    CCSize resourceSize = CCSizeMake(320, 480);

	if (screenSize.height > 320){
        resourceSize = CCSizeMake(640, 960);
        CCFileUtils::sharedFileUtils()->setResourceDirectory("hd");
    }else{
		CCFileUtils::sharedFileUtils()->setResourceDirectory("sd");
	}
	pDirector->setContentScaleFactor(resourceSize.height/designSize.height);
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);

    // turn on display FPS
    pDirector->setDisplayStats(false);
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm.wav",true);
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = GameStartScene::scene();

    // run
    pDirector->runWithScene(pScene);

	inMultiPlayer = false;
	networkLogic = NULL;
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
	if(inMultiPlayer){
		//CCDirector *pDirector = CCDirector::sharedDirector();
		if(networkLogic!=NULL){
			networkLogic->sendEvent(EV_DISCONNECT);
			networkLogic->disconnect();
		}
		//pDirector->getRunningScene()->onExit();
	}
    CCDirector::sharedDirector()->pause();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->resume();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	if(inMultiPlayer){
		CCScene *s = GameStartScene::scene();
		CCDirector::sharedDirector()->replaceScene(s);
		inMultiPlayer = false;
		if(networkLogic!=NULL){
			networkLogic->sendEvent(EV_DISCONNECT);
			networkLogic->disconnect();
		}
		networkLogic = NULL;
	}
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
