#ifndef _BIRD_NUMBERNODE_H_
#define _BIRD_NUMBERNODE_H_

class NumberNode : public cocos2d::Node
{
public:
	static NumberNode * create(int type=0);

	void SetNumber(int n);

	int GetNumber();

	int Add();
protected:
	int			nNumber;

	std::string strFmt;
};

#endif

