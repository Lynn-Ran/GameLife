#include "pch.h"
#include "BackgroundNode.h"

USING_NS_CC;

bool BackgroundNode::IsNight()
{
	return bNight;
}

bool BackgroundNode::init()
{
	Director * pDirector = Director::getInstance();
	Size visibleSize = pDirector->getVisibleSize();
	Point origin = pDirector->getVisibleOrigin();

	//根据时间点设置不同背景
	time_t t = time(NULL);
	tm * ptm = localtime(&t);
	int hour = ptm->tm_hour;
	Sprite * pBg = NULL;
	if(hour>=7 && hour <=18)
	{
		bNight = false;
		pBg = Sprite::createWithSpriteFrameName("bg_day");
	}
	else
	{
		bNight = true;
		pBg = Sprite::createWithSpriteFrameName("bg_night");
	}
	pBg->setPosition(Point(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2));
	addChild(pBg,0);

	return true;
}

