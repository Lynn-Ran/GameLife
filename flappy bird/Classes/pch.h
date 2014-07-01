#ifndef _BRID_PCH_H_
#define _BRID_PCH_H_


#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#define min(X,Y) ((X) < (Y) ? (X) : (Y)) 
#define max(X,Y) ((X) > (Y) ? (X) : (Y)) 

#define BirdMark 0x1
#define BirdCollisionMark 0x6
#define LandMark 0x2
#define LandCollisionMark 0x1
#define PipeMark 0x4
#define PipeCollisionMark 0x1


#define WorldGravity	-800.f			//世界重力
#define UpJumpSpeed		250.f			//小鸟向上速度
#define PipeMoveXSpeed	2.f				//水管移动速度
#define PipeGapY		100.f			//管道开口大小
#define PipeGapX		170.f			//管道间距

#define PIPE_PASS		30
#define PIPE_NEW		31
#define BirdTag			1000
#define ReadyTag		1001
#define TutorialTag		1002
#define OverTag			1003

#endif

