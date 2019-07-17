
#include "Global.h"

enum ExpressionMask
{
	LeftEye=		0b0000001,
	RightEye=		0b0000010,
	LeftMouth=		0b0000100,
	RightMouth=		0b0001000,
	CenterMouth=	0b0010000,
	Nose=			0b0100000,
	Body=			0b1000000,
};

struct Expression
{
	std::string Name;
	ExpressionMask Mask;
};

struct ExpressionGroup
{
	std::string Name;
	Expression *Group[7];
};

struct Emotion
{
	std::string Name;
	std::vector<ExpressionGroup *> ExpressionList;
};


std::vector<Emotion> ExpressionList =
{
	{
		.Name="Neutral",
	},
	{
		.Name="Happy",
	},
	{
		.Name="Concern",
	},
};

