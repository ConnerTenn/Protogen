
#include "Expressions.h"

ExprFrag EL1{"EL1",1};
ExprFrag EL2{"EL2",2};
ExprFrag ER1{"ER1",3};
ExprFrag ER2{"ER2",4};
ExprFrag ML1{"ML1",5};
ExprFrag ML2{"ML2",6};
ExprFrag MR1{"MR1",7};
ExprFrag MR2{"MR2",8};
ExprFrag MC1{"MC1",9};
ExprFrag MC2{"MC2",10};
ExprFrag N1{"N1",11};
ExprFrag N2{"N2",12};
ExprFrag B1{"B1",13};
ExprFrag B2{"B2",14};


std::vector<Expression> ExpressionList =
{
	{
		.Name="Neutral",
		.Frags={&EL1,&ER1,&ML1,&MR1,&MC1,&N1,&B1}
	},
	{
		.Name="Happy",
		.Frags={&EL2,&ER2,&ML2,&MR2,&MC1,&N1,&B1}
	},
	{
		.Name="Concern",
		.Frags={&EL1,&ER2,&ML2,&MR2,&MC2,&N2,&B2}
	},
};

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
