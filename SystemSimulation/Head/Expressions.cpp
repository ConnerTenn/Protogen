

#include "Expressions.h"

i32 SelectedEmote=0;

const u8 *ExpressionData = (const u8 *)(&_binary_Head_ExpressionData_bin_start);
const u8 *ExpressionDataEnd = (const u8 *)(&_binary_Head_ExpressionData_bin_end);

u16 NumExprFrags;
u16 NumExpressions;

ExprFrag *ExprFrags;
Expression *Expressions;

void InitExpressions()
{
	NumExprFrags=*(u16 *)ExpressionData;
	NumExpressions=*(u16 *)(ExpressionData+2);
	ExprFrags = (ExprFrag *)(ExpressionData+4);
	Expressions = (Expression *)(ExpressionData+4+NumExprFrags*sizeof(ExprFrag));
}

void SendExpressionState(MessageHandler *messenger)
{
	Message msg;
	strcpy(msg.Dest,"Emote");
	strcpy(msg.Label,"Update");

	Expression *expr=&Expressions[SelectedEmote];
	ExpressionState state;
	
	state.LeftEye=expr->Frags[0];
	state.RightEye=expr->Frags[1];
	state.LeftMouth=expr->Frags[2];
	state.RightMouth=expr->Frags[3];
	state.CenterMouth=expr->Frags[4];
	state.Nose=expr->Frags[5];
	state.Body=expr->Frags[6];

	msg.ContentLen=sizeof(ExpressionState);
	memcpy(msg.Content, &state, msg.ContentLen);

	SendMessage(messenger, &msg);
}
