
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
	const char Name[16];
	//ExpressionMask Mask;
	u16 ID;
};


struct Expression
{
	const char Name[16];
	u16 Frags[8];
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

extern i32 SelectedEmote;

void SendExpressionState(MessageHandler *messenger);

//#include "ExpressionData.h"
/*
Expression Data
u16 NumFrags
u16 NumExpressions
{
	char Name[16]
	u16 ID;
}[]
{
	char Name[16]
	u16 Frags[8]
}[]
*/

extern const u8 _binary_Head_ExpressionData_bin_start;
extern const u8 _binary_Head_ExpressionData_bin_end;
extern const u8 *ExpressionData;
extern const u8 *ExpressionDataEnd;

extern u16 NumExprFrags;
extern u16 NumExpressions;

extern ExprFrag *ExprFrags;
extern Expression *Expressions;

void InitExpressions();

#endif
