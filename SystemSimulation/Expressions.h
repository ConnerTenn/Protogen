
#ifndef _EXPRESSIONS_H_
#define _EXPRESSIONS_H_

#include "Global.h"
#include "Messaging.h"

// enum ExpressionMask
// {
// 	LeftEye=		0b0000001,
// 	RightEye=		0b0000010,
// 	LeftMouth=		0b0000100,
// 	RightMouth=		0b0001000,
// 	CenterMouth=	0b0010000,
// 	Nose=			0b0100000,
// 	Body=			0b1000000,
// };

struct ExprFrag
{
	std::string Name;
	//ExpressionMask Mask;
	u16 ID;
};


struct Expression
{
	std::string Name;
	ExprFrag *Frags[7];
	//i32 DurationMin, DurationMax;
};


struct ExpressionState
{
	u16 LeftEye;
	u16 RightEye;
	u16 LeftMouth;
	u16 RightMouth;
	u16 CenterMouth;
	u16 Nose;
	u16 Body;
};

extern std::vector<Expression> ExpressionList;
extern i32 SelectedEmote;

void SendExpressionState(MessageHandler *messenger);

#endif
