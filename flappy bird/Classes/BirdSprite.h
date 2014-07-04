#ifndef _BIRD_BIRDSPRITE_H_
#define _BIRD_BIRDSPRITE_H_

class BridSprite : public cocos2d::Sprite
{
public:
	virtual bool init();

	CREATE_FUNC(BridSprite);

	void idle();

	void fly();

	void dead();
protected:

	cocos2d::Animation * createAnimation(const char * fmt);

	cocos2d::Action * m_pIdleAction;
	cocos2d::Action * m_pFlyAction;
};

#endif


