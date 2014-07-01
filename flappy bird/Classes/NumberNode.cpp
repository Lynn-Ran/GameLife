#include "pch.h"
#include "NumberNode.h"

USING_NS_CC;

NumberNode * NumberNode::create(int type)
{
	NumberNode * pNumber = new NumberNode();
	if (pNumber)
	{
		if (type==0)
		{
			pNumber->strFmt = "number_context_0%d";
		}
		else
		{
			pNumber->strFmt = "number_score_0%d";
		}
		pNumber->autorelease();
	}
	pNumber->SetNumber(0);
	return pNumber;
}

void NumberNode::SetNumber(int n)
{
	removeAllChildren();
	if(n<0)
		n=0;
	nNumber = n;

	float w = 0;
	std::vector<Sprite *> vNum;
	do
	{
		int d = n%10;
		n /= 10;
		const char *filename = String::createWithFormat(strFmt.c_str(), d)->getCString();
		auto number = Sprite::createWithSpriteFrameName(filename);
		w += number->getContentSize().width;
		vNum.push_back(number);
	}while(n>0);

	w += vNum.size()-1;
	float fOffsetX = 0.f;
	for (int i = 0; i < vNum.size(); i++)
	{
		auto number = vNum[i];
		float fW = number->getContentSize().width;
		float fH = number->getContentSize().height;
		number->setPosition(Point((w-fW)/2-fOffsetX,fH/2));
		fOffsetX += fW+1;
		addChild(number);
	}
}

int NumberNode::GetNumber()
{
	return nNumber;
}

int NumberNode::Add()
{
	SetNumber(nNumber+1);
	return nNumber;
}