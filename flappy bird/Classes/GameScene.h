#ifndef __BRID_GAME_SCENE_H__
#define __BRID_GAME_SCENE_H__


class GameScene : public cocos2d::Scene
{
public:
	virtual bool init();  
    
    CREATE_FUNC(GameScene);

public:

	bool onHitBegan(cocos2d::PhysicsContact& contact);

	void onTouchBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent);
};

#endif // __HELLOWORLD_SCENE_H__
