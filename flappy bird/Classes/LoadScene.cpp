#include "pch.h"
#include "LoadScene.h"
#include "SimpleAudioEngine.h"
#include "WelcomeLayer.h"

USING_NS_CC;

void LoadScene::onEnter()
{
	Sprite * Bg = Sprite::create("splash.png");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	Bg->setPositionX(origin.x+visibleSize.width/2);
	Bg->setPositionY(origin.y+visibleSize.height/2);
	addChild(Bg);

	Director::getInstance()->getTextureCache()->addImageAsync("atlas.png",CC_CALLBACK_1(LoadScene::LoadCallback,this));
}

void LoadScene::LoadCallback(Texture2D* tex)
{
	SpriteFrameCache * frameCache = SpriteFrameCache::getInstance();

	std::string data = FileUtils::getInstance()->getStringFromFile("atlas.txt");
	unsigned pos = data.find_first_of("\n");
	std::string line = data.substr(0,pos);
	while (line!="")
	{
		char name[256] = {'\0'};
		Size sz;Point p,p1;
		sscanf(line.c_str(), "%s %f %f %f %f %f %f",name, &sz.width, &sz.height, &p.x, &p.y,&p1.x,&p1.y);
		p = p*1024;
		if (name==std::string("land"))
		{
			p.x += 1;
		}
		
		SpriteFrame * pSpriteFrame = SpriteFrame::createWithTexture(tex,Rect(p.x,p.y,sz.width,sz.height));
		frameCache->addSpriteFrame(pSpriteFrame,name);

		data = data.substr(pos + 1);
		pos = data.find_first_of("\n");
		line = data.substr(0, pos);
	}

	// 加载声音资源
	CocosDenshion::SimpleAudioEngine * pAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	pAudioEngine->preloadEffect("sfx_die.ogg");
	pAudioEngine->preloadEffect("sfx_hit.ogg");
	pAudioEngine->preloadEffect("sfx_point.ogg");
	pAudioEngine->preloadEffect("sfx_swooshing.ogg");
	pAudioEngine->preloadEffect("sfx_wing.ogg");
	pAudioEngine->setEffectsVolume(1.f);

	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = CC_CALLBACK_2(LoadScene::onKeyBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);


	//切换场景
	Director::getInstance()->replaceScene(TransitionFade::create(1,WelcomeLayer::createScene()));
}

void LoadScene::onKeyBegan(EventKeyboard::KeyCode keycode, Event*pEvent)
{
	if (keycode==EventKeyboard::KeyCode::KEY_BACKSPACE||
		keycode==EventKeyboard::KeyCode::KEY_MENU )
	{
		Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	}
}