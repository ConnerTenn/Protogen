
#include "ExpressionData.h"

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

Expression ExpressionList[] =
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
