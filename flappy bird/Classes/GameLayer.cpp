#include "pch.h"
#include "GameLayer.h"
#include "BackgroundNode.h"

USING_NS_CC;

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Director * pDirector = Director::getInstance();
	Size visibleSize = pDirector->getVisibleSize();
	Point origin = pDirector->getVisibleOrigin();

	//背景
	auto pBgLayer = BackgroundNode::create();
	addChild(pBgLayer);

	//陆地
	landNode = LandNode::create();
	landNode->startMove(false);
	addChild(landNode,2);

	//底部碰撞
	auto * pnode = Node::create();
	auto body = CreatePhysicsBody(PhysicsShapeBox::create(landNode->getLandSize()),false,false,LandMark,LandCollisionMark);
	pnode->setPhysicsBody(body);
	pnode->setPosition(visibleSize.width/2,landNode->getLandSize().height/2);
	addChild(pnode);

	//小鸟
	pBird = BridSprite::create();
	body = CreatePhysicsBody(PhysicsShapeCircle::create(pBird->getContentSize().width*0.35f),true,false,BirdMark,BirdCollisionMark);
	pBird->setPhysicsBody(body);
	pBird->setPosition(origin.x + visibleSize.width*1/3 - 5,origin.y + visibleSize.height/2 + 5);
	pBird->fly();
	pBird->setTag(BirdTag);
	addChild(pBird,2);

	//添加水管
	addPipes();

	OnGameState(eGameStatus_Ready);
	return true;
}

void GameLayer::SetShowReady(bool b)
{
	if (b)
	{
		Director * pDirector = Director::getInstance();
		Size visibleSize = pDirector->getVisibleSize();
		Point origin = pDirector->getVisibleOrigin();
		
		auto getreadySprite = Sprite::createWithSpriteFrameName("text_ready");
		getreadySprite->setPosition(Point(origin.x + visibleSize.width / 2,origin.y + visibleSize.height *2/3));
		this->addChild(getreadySprite,1,ReadyTag);

		auto tutorialSprite = Sprite::createWithSpriteFrameName("tutorial");
		tutorialSprite->setPosition(Point(origin.x + visibleSize.width / 2,origin.y + visibleSize.height * 1/2));
		this->addChild(tutorialSprite,1,TutorialTag);
	}
	else
	{
		removeChildByTag(ReadyTag);
		removeChildByTag(TutorialTag);
	}
}

void GameLayer::ResetBirdInfo(bool bGravity,bool resetPos)
{
	Director * pDirector = Director::getInstance();
	Size visibleSize = pDirector->getVisibleSize();
	Point origin = pDirector->getVisibleOrigin();

	pBird->setRotation(0.f);
	auto body = pBird->getPhysicsBody();
	body->setGravityEnable(bGravity);
	body->setVelocity(Vect::ZERO);

	if(resetPos)
	{
		pBird->setPosition(origin.x + visibleSize.width*1/3 - 5,origin.y + visibleSize.height/2 + 5);
	}
}

PhysicsBody * GameLayer::CreatePhysicsBody(PhysicsShape * pShape,bool bDynamic,bool bEnableGravity,int iBodyMark,int iCollisionMark)
{
	PhysicsBody * body = PhysicsBody::create();
	body->addShape(pShape);							//设置几何图形
	body->setDynamic(bDynamic);						//是否动态碰撞
	body->setGravityEnable(bEnableGravity);			//是否受重力影响
	body->setLinearDamping(0.f);					//设	置阻	力
	body->setCategoryBitmask( iBodyMark);			//刚体类别
	body->setContactTestBitmask(iCollisionMark);	//与之发生交集类别 
	body->setCollisionBitmask(iCollisionMark);		//与之发生碰撞类别
	body->setRotationEnable(false);
	return body;
}

void GameLayer::birdUpdate()
{
	float verticalSpeed = pBird->getPhysicsBody()->getVelocity().y;
	pBird->setRotation(min(max(-90, (verticalSpeed*0.2 + 60)), 30));
}

void GameLayer::movePipes(float /*dt*/)
{
	cocos2d::Map< cocos2d::Sprite *,cocos2d::Sprite *>::iterator it = vPipes.begin();
	while (it!=vPipes.end())
	{
		Sprite * pipeUp = it->first;
		Sprite * pipeDown = it->second;

		float fX = pipeUp->getPositionX()-PipeMoveXSpeed;
		if (fX<-pipeUp->getContentSize().width/2)
		{
			pipeUp->setPositionX(fX+fPipeDiff);
			pipeDown->setPositionX(fX+fPipeDiff);
			randPipeY(pipeUp,pipeDown);
		}
		else
		{
			pipeUp->setPositionX(fX);
			pipeDown->setPositionX(fX);
		}
		it++;
	}

	birdUpdate();
}

void GameLayer::resetPipes()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Map< cocos2d::Sprite *,cocos2d::Sprite *>::iterator it = vPipes.begin();

	float fOffsetX = 0.f;
	while (it!=vPipes.end())
	{
		Sprite * pipeUp = it->first;
		Sprite * pipeDown = it->second;

		pipeDown->setPositionX(visibleSize.width+pipeDown->getContentSize().width/2+fOffsetX);
		pipeUp->setPositionX(visibleSize.width+pipeUp->getContentSize().width/2+fOffsetX);

		randPipeY(pipeUp,pipeDown);
		fOffsetX+=PipeGapX;
		it++;
	}
}

void GameLayer::addPipes()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	fPipeDiff = 0.f;
	float fDiffX_MAX = visibleSize.width+PipeGapX;
	while (fPipeDiff<=fDiffX_MAX)
	{
		Sprite * pipeDown = Sprite::createWithSpriteFrameName("pipe_up");
		PhysicsBody *body = CreatePhysicsBody(PhysicsShapeBox::create(pipeDown->getContentSize()),false,false,PipeMark,PipeCollisionMark);
		pipeDown->setPhysicsBody(body);
		addChild(pipeDown,1);

		Sprite * pipeUp = Sprite::createWithSpriteFrameName("pipe_down");
		body = CreatePhysicsBody(PhysicsShapeBox::create(pipeUp->getContentSize()),false,false,PipeMark,PipeCollisionMark);
		pipeUp->setPhysicsBody(body);
		addChild(pipeUp,1);

		pipeDown->setPositionX(visibleSize.width+pipeDown->getContentSize().width/2+fPipeDiff);
		pipeUp->setPositionX(visibleSize.width+pipeUp->getContentSize().width/2+fPipeDiff);

		vPipes.insert(pipeUp,pipeDown);
		randPipeY(pipeUp,pipeDown);

		fPipeDiff+=PipeGapX;
	}
}

void GameLayer::randPipeY(Sprite *pipeUp,Sprite * pipeDown)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	int hightMin = landNode->getLandSize().height-pipeDown->getContentSize().height+20;
	int hightMax = landNode->getLandSize().height-20;
	int randSize = hightMax-hightMin;

	int downHight = hightMin+rand()%randSize+pipeDown->getContentSize().height/2;
	pipeDown->setPositionY(downHight);

	int upHight = downHight+pipeDown->getContentSize().height/2+pipeUp->getContentSize().height/2+PipeGapY;
	pipeUp->setPositionY(upHight);
}

void GameLayer::onTouch()
{
	switch (iStatus)
	{
	case eGameStatus_Ready:
		{
			OnGameState(eGameStatus_Tick);
		}
	case eGameStatus_Tick:
		{
			auto pAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
			pAudioEngine->playEffect("sfx_wing.ogg");
			auto body = pBird->getPhysicsBody();
			Vect v = body->getVelocity();
			static const float fMaxJumpSpeed =  UpJumpSpeed*1.5f;
			if (v.y>0)
			{
				v.y += UpJumpSpeed;
				if (v.y>fMaxJumpSpeed)
				{
					v.y = fMaxJumpSpeed;
				}
			}
			else
			{
				v.y = UpJumpSpeed;
			}
			body->setVelocity(v);
		}
		break;
	case eGameStatus_Over:
		{
			OnGameState(eGameStatus_Ready);
		}
	default:
		break;
	}
}

void GameLayer::onBirdHitPips()
{
	OnGameState(eGameStatus_Over);
}

void GameLayer::OnGameState(int itype)
{
	Director * pDirector = Director::getInstance();
	switch (itype)
	{
	case eGameStatus_Ready:
		{
			ResetBirdInfo(false,true);
			SetShowReady(true);
			landNode->startMove(false);
		}
		break;
	case eGameStatus_Tick:
		{
			ResetBirdInfo(true,false);
			SetShowReady(false);
			landNode->startMove(true);
			schedule(schedule_selector(GameLayer::movePipes),pDirector->getAnimationInterval());
		}
		break;
	case eGameStatus_Over:
		{
			resetPipes();
			ResetBirdInfo(false,true);
			landNode->startMove(false);
			unschedule(schedule_selector(GameLayer::movePipes));
		}
		break;
	default:
		break;
	}
	iStatus = itype;
}