#ifndef _GAMEOVER_SCENE_H_
#define _GAMEOVER_SCENE_H_

#include "cocos2d.h"

using namespace cocos2d;

class GameOverScene : public CCLayer {
public:
	virtual bool init();
	static CCScene* scene();

	void retryGame(CCObject* sender);
	void quitGame(CCObject* sender);
	
	CREATE_FUNC(GameOverScene);

	CCLabelTTF* scoreTextLabel;
	int score;
};

#endif 