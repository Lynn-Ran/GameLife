#ifndef _BIRD_WELCOMELAYER_H_
#define _BIRD_WELCOMELAYER_H_

class WelcomeLayer:public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

	virtual bool init();  
    
    CREATE_FUNC(WelcomeLayer);

public:

	void menuStartButton(cocos2d::Ref * sender);

	void moveLand(float dt);

protected:

	cocos2d::Sprite * land1;

	cocos2d::Sprite * land2;

};

#endif


