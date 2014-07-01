#ifndef _BIRD_LOADSCENE_H_
#define _BIRD_LOADSCENE_H_

class LoadScene:public cocos2d::Scene
{
public:

	CREATE_FUNC(LoadScene);

protected:

	virtual void onEnter() override;

protected:

	void LoadCallback(cocos2d::Texture2D* tex);
};

#endif
