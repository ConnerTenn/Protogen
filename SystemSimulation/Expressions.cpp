

#include "Expressions.h"

i32 SelectedEmote=0;

void SendExpressionState(MessageHandler *messenger)
{
	Message msg;
	strcpy(msg.Dest,"Emote");
	strcpy(msg.Label,"Update");

	Expression *expr=&ExpressionList[SelectedEmote];
	ExpressionState state;
	
	state.LeftEye=expr->Frags[0]->ID;
	state.RightEye=expr->Frags[1]->ID;
	state.LeftMouth=expr->Frags[2]->ID;
	state.RightMouth=expr->Frags[3]->ID;
	state.CenterMouth=expr->Frags[4]->ID;
	state.Nose=expr->Frags[5]->ID;
	state.Body=expr->Frags[6]->ID;

	msg.ContentLen=sizeof(ExpressionState);
	memcpy(msg.Content, &state, msg.ContentLen);

	SendMessage(messenger, &msg);
}
