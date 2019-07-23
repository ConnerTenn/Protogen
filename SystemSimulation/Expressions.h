
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
		.Frags={&EL2,&ER2,&ML1,&MR1,&MC1,&N1,&B1}
	},
	{
		.Name="Concern",
		.Frags={&EL1,&ER2,&ML2,&MR2,&MC1,&N1,&B1}
	},
};

i32 SelectedEmote=0;

void UpdateEmotionState(MessageHandler *messenger)
{
	Message msg;
	strcpy(msg.Dest,"Emote");
	strcpy(msg.Label,"Update");

	Expression *expr=&ExpressionList[SelectedEmote];
	u16 buff[7];
	for (u8 i=0; i<7; i++)
	{
		if (expr->Frags[i]) { buff[i]=expr->Frags[i]->ID; }
		else { buff[i]=-1; }
	}
	msg.ContentLen=sizeof(buff);
	memcpy(msg.Content, buff, msg.ContentLen);

	SendMessage(messenger, &msg);
}

