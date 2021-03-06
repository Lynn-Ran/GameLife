#include "pch.h"
#include "AppDelegate.h"
#include "LoadScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();

    auto glview = director->getOpenGLView();
    if(!glview) {
		glview = GLView::createWithRect("bird",Rect(0,0,DefaultWidth,DefaultHeight));

		director->setOpenGLView(glview);
    }
	//坑爹呀，老子一直写if里面，试了3小时才发现要写外面
	glview->setDesignResolutionSize(DefaultWidth,DefaultHeight,ResolutionPolicy::SHOW_ALL);

	InitResSearchPath();

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	auto scene = LoadScene::create();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::InitResSearchPath()
{
	std::vector<std::string> paths;
	paths.push_back("fonts");
    paths.push_back("image");
    paths.push_back("sounds");
    FileUtils::getInstance()->setSearchResolutionsOrder(paths);
}

void onKeyReleased(EventKeyboard::KeyCode keycode, Event*pEvent)
{
	log("onKeyReleased-------------->keycode:%d",keycode);
	if (keycode==EventKeyboard::KeyCode::KEY_BACKSPACE||
		keycode==EventKeyboard::KeyCode::KEY_MENU )
	{
		Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	}
}

void onKeyPressed(EventKeyboard::KeyCode keycode, Event*pEvent)
{
	log("onKeyPressed-------------->keycode:%d",keycode);
}

void AddKeyListener(Node * listener)
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = onKeyReleased;
	keyListener->onKeyPressed = onKeyPressed;
	dispatcher->addEventListenerWithSceneGraphPriority(keyListener, listener);

}