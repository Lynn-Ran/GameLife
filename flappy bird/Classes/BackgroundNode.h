#ifndef _BRID_BACKGROUNDNODE_H_
#define _BRID_BACKGROUNDNODE_H_

class BackgroundNode : public cocos2d::Node
{
public:

    CREATE_FUNC(BackgroundNode);

	bool IsNight();
protected:

	virtual bool init();  

	bool	bNight;
};


#endif

