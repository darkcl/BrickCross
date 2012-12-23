#ifndef __GAMESTART_SCENE_H__
#define __GAMESTART_SCENE_H__

#include "cocos2d.h"

class GameStartScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void startGame(CCObject* pSender);
	void startSingleGame(CCObject* pSender);

	void addBrick();

	void gameLoop(float dt);

	void spriteMoveFinished(CCNode* sender);

	void testPhoton_lib(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(GameStartScene);
};

#endif 