#include "pch.h"
#include "WelcomeLayer.h"
#include "BirdSprite.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "BackgroundNode.h"
#include "LandNode.h"

USING_NS_CC;

Scene * WelcomeLayer::createScene()
{
	auto scene = Scene::create();

	auto layer = WelcomeLayer::create();

	scene->addChild(layer);

	return scene;
}

bool WelcomeLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Director * pDirector = Director::getInstance();
	Size visibleSize = pDirector->getVisibleSize();
	Point origin = pDirector->getVisibleOrigin();

	//����
	auto pBgLayer = BackgroundNode::create();
	addChild(pBgLayer);

	//½��
	auto pLandNode = LandNode::create();
	addChild(pLandNode);

	//����
	Sprite * title = Sprite::createWithSpriteFrameName("title");
	title->setPosition(Point(origin.x + visibleSize.width/2,origin.y + visibleSize.height * 5 / 7));
	addChild(title);

	//��Ȩ����
	Sprite * copyright = Sprite::createWithSpriteFrameName("brand_copyright");
	copyright->setPosition(Point(origin.x + visibleSize.width/2, origin.y + visibleSize.height/7));
	addChild(copyright);

	//��ʼ��ť
	Sprite * normalbutton = Sprite::createWithSpriteFrameName("button_play");
	normalbutton->setPositionY(5.f);
	Sprite * selectbutton = Sprite::createWithSpriteFrameName("button_play");
	MenuItemSprite * menuItem = MenuItemSprite::create(normalbutton,selectbutton,CC_CALLBACK_1(WelcomeLayer::menuStartButton,this));
	menuItem->setPosition(Point(origin.x + visibleSize.width/2 ,origin.y + visibleSize.height*3/8));

	auto menu = Menu::create(menuItem,NULL);
	menu->setPosition(origin);
	addChild(menu);

	//����СС��
	BridSprite * bird = BridSprite::create();
	bird->setPosition(Point(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2));
	bird->idle();
	addChild(bird);

	return true;
}

void WelcomeLayer::menuStartButton(Ref * /*sender*/)
{
	CocosDenshion::SimpleAudioEngine * pAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	pAudioEngine->playEffect("sfx_swooshing.ogg");
	Director::getInstance()->replaceScene(TransitionFade::create(1.f, GameScene::create()));
}

void WelcomeLayer::moveLand(float /*dt*/)
{
	if (land2->getPositionX()<=0)
	{
		land1->setPositionX(0.f);
	}
	land1->setPositionX(land1->getPositionX()-2.f);
	land2->setPositionX(land1->getPositionX()+land1->getContentSize().width-2.f);
}