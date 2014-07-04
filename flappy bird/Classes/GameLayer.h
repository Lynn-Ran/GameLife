#ifndef _BRID_GAMELAYER_H_
#define _BRID_GAMELAYER_H_

#include "BirdSprite.h"
#include "LandNode.h"
#include "BackgroundNode.h"
#include "NumberNode.h"

class GameLayer : public cocos2d::Layer
{
	enum
	{
		eGameStatus_Ready,
		eGameStatus_Play,
		eGameStatus_PreOver,
		eGameStatus_Over,
	};
public:

    virtual bool init();  
    
    CREATE_FUNC(GameLayer);

public:
	void OnGameState(int iState);

public:
	void onTouch();
	void onBirdHit();

	void movePipes(float dt);
	void birdUpdate();

	void addPipes();
	void resetPipes();
	void randPipeY(cocos2d::Sprite *pipeUp,cocos2d::Sprite * pipeDown);

	void PrepareStart(float dt);
	void SetShowOver(bool);
	void SetShowReady(bool b);
	void ResetBirdInfo(bool bNew,bool bGravity,bool resetPos);
protected:

	cocos2d::PhysicsBody * CreatePhysicsBody(cocos2d::PhysicsShape * pShape,bool bDynamic,bool bEnableGravity,int iBodyMark,int iCollisionMark);

	BackgroundNode	* background;
	LandNode		* landNode;
	BridSprite		* pBird;
	NumberNode		* pScore;

	bool			  bEnableStart;
	int				  iStatus;

	cocos2d::Map< cocos2d::Sprite *,cocos2d::Sprite *> vPipes;
	float	fPipeDiff;
};

#endif

