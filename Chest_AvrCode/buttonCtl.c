
#include "buttonCtl.h"

u8 Timeout;
// u8 NumSequences;
Sequence Sequences[NUM_SEQUENCES];

void InitButtons()
{
	u8 off = 0;
	Timeout = BUTTONDATA_ACC_8(off); off+=1;

	for (u8 s = 0; s < NUM_SEQUENCES; s++)
	{
		Sequences[s].Momentary = BUTTONDATA_ACC_8(off); off+=1;
		Sequences[s].NumCombos = BUTTONDATA_ACC_8(off); off+=1;
		
		for (u8 c = 0; c < Sequences[s].NumCombos; c++)
		{
			Sequences[s].Combos[c].NumButtons = BUTTONDATA_ACC_8(off); off+=1;
			
			for (u8 b = 0; b < Sequences[s].Combos[c].NumButtons; b++)
			{
				Sequences[s].Combos[c].Buttons[b].ButtonNum = BUTTONDATA_ACC_8(off); off+=1;
			}	
		}
	}
}

