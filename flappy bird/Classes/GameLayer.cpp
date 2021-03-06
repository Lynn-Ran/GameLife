#include "pch.h"
#include "GameLayer.h"
#include "SimpleAudioEngine.h"

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
	background = BackgroundNode::create();
	addChild(background);

	//陆地
	landNode = LandNode::create();
	landNode->startMove(false);
	addChild(landNode,2);

	//边界碰撞
	auto pnode = Node::create();
	Size sz = Size(visibleSize.width,visibleSize.height-landNode->getLandSize().height);
	auto body = CreatePhysicsBody(PhysicsShapeEdgeBox::create(sz),false,false,LandMark,LandCollisionMark);
	pnode->setPhysicsBody(body);
	pnode->setPosition(visibleSize.width/2,landNode->getLandSize().height+sz.height/2);
	addChild(pnode);

	//发现有掉下去情况加个实体的
	pnode = Node::create();
	body = CreatePhysicsBody(PhysicsShapeBox::create(landNode->getLandSize()),false,false,LandMark,LandCollisionMark);
	pnode->setPhysicsBody(body);
	pnode->setPosition(landNode->getLandSize().width/2,landNode->getLandSize().height/2);
	addChild(pnode);

	//分数
	pScore = NumberNode::create();
	pScore->setPosition(Point(origin.x + visibleSize.width/2,origin.y + visibleSize.height * 6 / 7));
	addChild(pScore,3);

	//添加水管
	addPipes();

	bEnableStart = true;
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

void GameLayer::PrepareStart(float)
{
	bEnableStart = true;
}

void GameLayer::SetShowOver(bool b)
{
	if (b)
	{
		Director * pDirector = Director::getInstance();
		Size visibleSize = pDirector->getVisibleSize();
		Point origin = pDirector->getVisibleOrigin();

		Sprite* gameoverSprite = Sprite::createWithSpriteFrameName("text_game_over");
		gameoverSprite->setPosition(Point(origin.x + visibleSize.width / 2, origin.y-gameoverSprite->getContentSize().height/2));
		ActionInterval *up = CCMoveBy::create(0.3f,Point(0, gameoverSprite->getContentSize().height/2 + visibleSize.height *2/3));
		gameoverSprite->runAction(up);
		addChild(gameoverSprite,1,OverTag);

		bEnableStart = false;
		scheduleOnce(schedule_selector(GameLayer::PrepareStart),0.35f);
	}
	else
	{
		removeChildByTag(OverTag);
	}
}

void GameLayer::ResetBirdInfo(bool bNew,bool bGravity,bool resetPos)
{
	Director * pDirector = Director::getInstance();
	Size visibleSize = pDirector->getVisibleSize();
	Point origin = pDirector->getVisibleOrigin();

	if (bNew)
	{
		removeChildByTag(BirdTag);
		pBird = BridSprite::create();
		auto body = CreatePhysicsBody(PhysicsShapeCircle::create(pBird->getContentSize().width*0.35f),true,false,BirdMark,BirdCollisionMark);
		pBird->setPhysicsBody(body);
		pBird->setPosition(origin.x + visibleSize.width*1/3 - 5,origin.y + visibleSize.height/2 + 5);
		pBird->fly();
		pBird->setTag(BirdTag);
		addChild(pBird,2);
	}

	pBird->setRotation(0.f);
	auto body = pBird->getPhysicsBody();
	body->setGravityEnable(bGravity);
	body->setVelocity(Vect::ZERO);

	if(resetPos&&!bNew)
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
	pBird->setRotation(-min(max(-90, (verticalSpeed*0.2 + 60)), 30));
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
			pipeDown->setTag(PIPE_NEW);
			randPipeY(pipeUp,pipeDown);
		}
		else
		{
			pipeUp->setPositionX(fX);
			pipeDown->setPositionX(fX);
			if(fX<pBird->getPositionX()&&pipeDown->getTag()==PIPE_NEW)
			{
				pScore->Add();
				pipeDown->setTag(PIPE_PASS);

				auto pAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
				pAudioEngine->playEffect("sfx_point.ogg");
			}
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
		pipeDown->setTag(PIPE_NEW);
		pipeUp->setPositionX(visibleSize.width+pipeUp->getContentSize().width/2+fOffsetX);

		randPipeY(pipeUp,pipeDown);
		fOffsetX+=PipeGapX;
		it++;
	}
}

void GameLayer::addPipes()
{
	std::string UpName,DownName;
	if (background->IsNight())
	{
		UpName = "pipe2_up";
		DownName = "pipe2_down";
	}
	else
	{
		UpName = "pipe_up";
		DownName = "pipe_down";
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	fPipeDiff = 0.f;
	float fDiffX_MAX = visibleSize.width+PipeGapX;
	while (fPipeDiff<=fDiffX_MAX)
	{
		Sprite * pipeDown = Sprite::createWithSpriteFrameName(UpName);
		PhysicsBody *body = CreatePhysicsBody(PhysicsShapeBox::create(pipeDown->getContentSize()),false,false,PipeMark,PipeCollisionMark);
		pipeDown->setPhysicsBody(body);
		pipeDown->setTag(PIPE_NEW);
		addChild(pipeDown,1);

		Sprite * pipeUp = Sprite::createWithSpriteFrameName(DownName);
		body = CreatePhysicsBody(PhysicsShapeBox::create(pipeUp->getContentSize()),false,false,PipeMark,PipeCollisionMark);
		if (background->IsNight())
		{
			pipeUp->setRotation(180);
		}
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

	int hightMin = landNode->getLandSize().height-pipeDown->getContentSize().height+40;
	int hightMax = visibleSize.height-pipeDown->getContentSize().height-PipeGapY-40;
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
			OnGameState(eGameStatus_Play);
		}
	case eGameStatus_Play:
		{
			auto pAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
			pAudioEngine->playEffect("sfx_wing.ogg");
			auto body = pBird->getPhysicsBody();
			Vect v = body->getVelocity();
			if (v.y>0)
			{
				v.y += UpJumpSpeed;
				if (v.y>UpJumpSpeedMax)
				{
					v.y = UpJumpSpeedMax;
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
			if (bEnableStart)
			{
				OnGameState(eGameStatus_Ready);
			}
		}
	default:
		break;
	}
}

void GameLayer::onBirdHit()
{			
	if(iStatus==eGameStatus_Play)
	{
		auto pAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
		pAudioEngine->playEffect("sfx_die.ogg");
		OnGameState(eGameStatus_PreOver);
	}
	else if (iStatus==eGameStatus_PreOver)
	{
		auto body = pBird->getPhysicsBody();
		Vect v = body->getVelocity();
		if (v.x >0.0001f || v.y >0.0001f)
			return;
		OnGameState(eGameStatus_Over);
	}
}

void GameLayer::OnGameState(int itype)
{
	Director * pDirector = Director::getInstance();
	switch (itype)
	{
	case eGameStatus_Ready:
		{	
			resetPipes();
			SetShowOver(false);
			SetShowReady(true);
			ResetBirdInfo(true,false,true);
			landNode->startMove(false);
			pScore->SetNumber(0);
		}
		break;
	case eGameStatus_Play:
		{
			ResetBirdInfo(false,true,false);
			SetShowReady(false);
			landNode->startMove(true);
			schedule(schedule_selector(GameLayer::movePipes),pDirector->getAnimationInterval());
		}
		break;
	case eGameStatus_PreOver:
		{
			pBird->dead();
			landNode->startMove(false);
			unschedule(schedule_selector(GameLayer::movePipes));
		}
		break;
	case eGameStatus_Over:
		{
			SetShowOver(true);	
		}
		break;
	default:
		break;
	}
	iStatus = itype;
}