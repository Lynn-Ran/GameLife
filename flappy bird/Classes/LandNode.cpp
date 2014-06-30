#include "pch.h"
#include "LandNode.h"

USING_NS_CC;

bool LandNode::init()
{
	Director * pDirector = Director::getInstance();

	//Â½µØ
	land1 = Sprite::createWithSpriteFrameName("land");
	land2 = Sprite::createWithSpriteFrameName("land");
	land1->setAnchorPoint(Point::ZERO);
	land2->setAnchorPoint(Point::ZERO);
	land2->setPosition(Point(land1->getContentSize().width-2,0));
	addChild(land1);
	addChild(land2);

	bMove = false;
	startMove(true);
	return true;
}


void LandNode::moveLand(float /*dt*/)
{
	if (land2->getPositionX()<=0)
	{
		land1->setPositionX(0.f);
	}
	land1->setPositionX(land1->getPositionX()-2.f);
	land2->setPositionX(land1->getPositionX()+land1->getContentSize().width-2.f);
}

const Size& LandNode::getLandSize()
{
	return land1->getContentSize();
}

void LandNode::startMove(bool b)
{
	if(bMove!=b)
	{
		Director * pDirector = Director::getInstance();
		if(b)
		{
			schedule(schedule_selector(LandNode::moveLand),pDirector->getAnimationInterval());
		}
		else
		{
			unschedule(schedule_selector(LandNode::moveLand));
		}
		bMove = b;
	}
}