#ifndef _BRID_GAMELAYER_H_
#define _BRID_GAMELAYER_H_

#include "BirdSprite.h"
#include "LandNode.h"

class GameLayer : public cocos2d::Layer
{
	enum
	{
		eGameStatus_Ready,
		eGameStatus_Tick,
		eGameStatus_Over,
	};
public:

    virtual bool init();  
    
    CREATE_FUNC(GameLayer);

public:
	void OnGameState(int iState);

public:
	void onTouch();
	void onBirdHitPips();

	void movePipes(float dt);
	void birdUpdate();

	void addPipes();
	void resetPipes();
	void randPipeY(cocos2d::Sprite *pipeUp,cocos2d::Sprite * pipeDown);

	void SetShowReady(bool b);
	void ResetBirdInfo(bool bGravity,bool resetPos);
protected:

	cocos2d::PhysicsBody * CreatePhysicsBody(cocos2d::PhysicsShape * pShape,bool bDynamic,bool bEnableGravity,int iBodyMark,int iCollisionMark);

	LandNode		* landNode;
	BridSprite		* pBird;

	int				  iStatus;

	cocos2d::Map< cocos2d::Sprite *,cocos2d::Sprite *> vPipes;
	float	fPipeDiff;
};

#endif

