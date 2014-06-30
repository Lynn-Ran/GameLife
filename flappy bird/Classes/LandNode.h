#ifndef _BIRD_LANDNODE_H_
#define _BIRD_LANDNODE_H_


class LandNode : public cocos2d::Node
{
public:
    CREATE_FUNC(LandNode);

	void moveLand(float dt);

	void startMove(bool b);

	const cocos2d::Size & getLandSize();

protected:
	virtual bool init();  

public:
	cocos2d::Sprite * land1;

	cocos2d::Sprite * land2;

	bool			  bMove;
};

#endif


