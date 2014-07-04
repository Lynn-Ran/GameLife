#include "pch.h"
#include "GameScene.h"
#include "GameLayer.h"

USING_NS_CC;

void AddKeyListener(Node * listener);

bool GameScene::init()
{
	if (Scene::initWithPhysics())
	{
		auto pPhysicsWorld = getPhysicsWorld();
		pPhysicsWorld->setGravity(Vect(0.f,WorldGravity));
		pPhysicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); //调试时使用 

		//游戏
		auto pGameLayer = GameLayer::create();
		addChild(pGameLayer,1);

		//添加碰撞监视
		auto contactListener = EventListenerPhysicsContact::create();
		contactListener->onContactPreSolve = CC_CALLBACK_2(GameScene::onHit, this);
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, pGameLayer);

		//添加按键响应
		auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
		auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->addEventListenerWithSceneGraphPriority(listener, pGameLayer);
		
		AddKeyListener(this);
		return true;
	}
    return false;;
}

void GameScene::onTouchBegan(const std::vector<Touch*>& /*touches*/, Event *pEvent)
{
	GameLayer * pGameLayer = (GameLayer *)pEvent->getCurrentTarget();
	pGameLayer->onTouch();
}

bool GameScene::onHit(PhysicsContact& contact,PhysicsContactPreSolve& solve)
{
	Node * pNodeA = contact.getShapeA()->getBody()->getNode();
	Node * pNodeB = contact.getShapeB()->getBody()->getNode();
	if (pNodeA==nullptr||pNodeB==nullptr)
	{
		return false;
	}
	if (pNodeA->getTag()==BirdTag||pNodeB->getTag()==BirdTag)
	{
		GameLayer * pGameLayer = (GameLayer *)contact.getCurrentTarget();
		pGameLayer->onBirdHit();
	}
	return true;
}