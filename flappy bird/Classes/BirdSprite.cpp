#include "pch.h"
#include "BirdSprite.h"

USING_NS_CC;

bool BridSprite::init()
{
	int itype = rand()%3;
	const char * birdName = String::createWithFormat("bird%d_0",itype)->getCString();
	const char * birdNameFmt = String::createWithFormat("bird%d_%%d",itype)->getCString();

	Sprite::initWithSpriteFrameName(birdName);

	auto * anim = createAnimation(birdNameFmt);
	auto * animate = Animate::create(anim);
	m_pFlyAction = RepeatForever::create(animate);
	
	ActionInterval *up = CCMoveBy::create(0.4f,Point(0, 8));
	ActionInterval *upBack= up->reverse();
	m_pIdleAction = RepeatForever::create(Sequence::create(up, upBack, NULL));
	return true;
}

void BridSprite::idle()
{
	stopAllActions();
	runAction(m_pFlyAction);
	runAction(m_pIdleAction);
}

void BridSprite::fly()
{
	stopAllActions();
	runAction(m_pFlyAction);
}

void BridSprite::dead()
{
	stopAllActions();
}

Animation * BridSprite::createAnimation(const char * fmt)
{
	auto * frameCache = SpriteFrameCache::getInstance();
	auto * anim = Animation::create();
	anim->setDelayPerUnit(0.1f);
	for (int i = 0; i < 3; i++)
	{
		const char *filename = String::createWithFormat(fmt, i)->getCString();
		anim->addSpriteFrame(frameCache->getSpriteFrameByName(filename));
	}
	return anim;
}
