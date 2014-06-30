#include "pch.h"
#include "GameScene.h"
#include "GameLayer.h"

USING_NS_CC;

bool GameScene::init()
{
	if (Scene::initWithPhysics())
	{
		auto pPhysicsWorld = getPhysicsWorld();
		pPhysicsWorld->setGravity(Vect(0.f,WorldGravity));
		//pPhysicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); //����ʱʹ�� 

		//��Ϸ
		auto pGameLayer = GameLayer::create();
		addChild(pGameLayer,1);

		//������ײ����
		auto contactListener = EventListenerPhysicsContact::create();
		contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onHitBegan, this);
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, pGameLayer);

		//���Ӱ�����Ӧ
		auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
		auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->addEventListenerWithSceneGraphPriority(listener, pGameLayer);
		return true;
	}
    return false;;
}

void GameScene::onTouchBegan(const std::vector<Touch*>& /*touches*/, Event *pEvent)
{
	GameLayer * pGameLayer = (GameLayer *)pEvent->getCurrentTarget();
	pGameLayer->onTouch();
}

bool GameScene::onHitBegan(PhysicsContact& contact)
{
	Node * pNodeA = contact.getShapeA()->getBody()->getNode();
	Node * pNodeB = contact.getShapeB()->getBody()->getNode();
	if (pNodeA->getTag()==BirdTag||pNodeB->getTag()==BirdTag)
	{
		GameLayer * pGameLayer = (GameLayer *)contact.getCurrentTarget();
		pGameLayer->onBirdHitPips();
	}
	return true;
}