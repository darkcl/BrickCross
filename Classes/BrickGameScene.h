#ifndef _BRICKGAME_SCENE_H_
#define _BRICKGAME_SCENE_H_

#include "cocos2d.h"
#include "TimeBar.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

class BrickGameScene : public CCLayer,CCStandardTouchDelegate {
public:
	virtual bool init();
	static CCScene* scene();

	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);

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

	CREATE_FUNC(BrickGameScene);
protected:
	CCArray* rowOfBricks;
	CCArray* grabbedBricks;

	CCLayer* brickContainer;
	CCLabelTTF* scoreTextLabel;
	CCLayerColor* pauseColorLayer;
	CCSprite* bonusSprite;
	CCSprite* dangerSprite;
	CCAction* dangerBlink;
	CCMenu* pMenu2;

	TimeBar* timeBar;

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

};
#endif 