#ifndef _MULTIBRICKGAME_SCENE_H_
#define _MULTIBRICKGAME_SCENE_H_

#include "cocos2d.h"
#include "TimeBar.h"
#include "SimpleAudioEngine.h"
#include "NetworkLogic.h"
#include "OutputListener.h"
#include "Player.h"


using namespace cocos2d;

class MultiPlayerBrickGameScene : public CCLayer,CCStandardTouchDelegate,OutputListener,NetworkLogicListener {
public:
	virtual bool init();
	static CCScene* scene();

	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void onExit();

	void gameLoop(float dt);
	void generateBrick(float dt);
	void countDown(float dt);
	void redrawScene(CCNode* sender);

	void enableTouch(CCNode* sender);
	void removeBrick(CCNode* sender);

	void goToGameOverScene();

	void isGameOver(CCNode* sender);
	void dangerFadeIn(CCNode* sender);
	void pasueGame(CCNode* sender);
	void resumeGame(CCNode* sender);
	void showPauseMenu(CCObject* sender);
	void dismissPauseMenu(CCObject* sender);
	void toggleMute(CCObject* sender);

	void fastForward();
	void resetBonus(CCNode* sender);

	//Photon Lib 
	virtual void write(const ExitGames::Common::JString& str);
	virtual void writeLine(const ExitGames::Common::JString& str);

	virtual void stateUpdate(State newState);
	void onEvent(const ExitGames::Photon::EventData& eventData);
	void networkLoop(float dt);

	void createRoom(CCObject* sender);
	void joinRoom(CCObject* sender);
	void netStartGame();

	void attackRival();
	void moveGauage();
	void showWin(CCNode* sender);
	void showLose(CCNode* sender);

	void oppDisconnect();
	void showAttackTips();
	void resetGameScene();

	Player *myInfo;
	Player *oppInfo;

	CCSprite *attactTips;
	CCSprite *attackBar;

	int gauage;

	CREATE_FUNC(MultiPlayerBrickGameScene);
protected:
	CCArray* rowOfBricks;
	CCArray* grabbedBricks;

	CCLayer* brickContainer;
	CCMenu* pMenu2;

	int colorsCount;
	int dangerSignOpacity;
	float timeTick;
	int countDownTime;
	int score;
	int startRowCount;
	int selectedColor;
	int lastRowTouched;
	int rowTouched;

	bool touchMoved;
	bool holdingBrick;
	bool startGame;
	bool generating;
	bool gamePause;
	bool countDanger;
	bool timeDanger;

	CCPoint lastTocuh;

	//Photon Lib related
	NetworkLogic *networkLogic;

	CCMenuItemImage* multiplayerSprite1;
	CCMenuItemImage* multiplayerSprite2;
	CCMenu* pMultiPlayMenu;
};
#endif 