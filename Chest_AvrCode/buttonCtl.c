
#include "buttonCtl.h"

u8 Timeout;
u8 CommandData[COMMAND_DATA_LEN];
Button Buttons[NUM_BUTTONS];
Sequence Sequences[NUM_SEQUENCES];

void InitButtons()
{
	u8 off = 0;
	
	Timeout = BUTTONDATA_ACC_8(off); off+=1;

	for (u16 i = 0; i < COMMAND_DATA_LEN; i++)
	{
		CommandData[i] = BUTTONDATA_ACC_8(off); off+=1;
	}

	for (u8 b = 0; b < NUM_BUTTONS; b++)
	{
		Buttons[b].ButtonNum = BUTTONDATA_ACC_8(off); off+=1;
	}

	for (u8 s = 0; s < NUM_SEQUENCES; s++)
	{
		Sequences[s].ActiveCombo = 0;
		Sequences[s].Momentary = BUTTONDATA_ACC_8(off); off+=1;

		Sequences[s].CommandLen = BUTTONDATA_ACC_8(off); off+=1;
		Sequences[s].Command = CommandData + BUTTONDATA_ACC_16(off); off+=1;

		Sequences[s].NumCombos = BUTTONDATA_ACC_8(off); off+=1;
		for (u8 c = 0; c < Sequences[s].NumCombos; c++)
		{
			Sequences[s].Combos[c].Active = 0;
			Sequences[s].Combos[c].NumButtons = BUTTONDATA_ACC_8(off); off+=1;
			
			for (u8 b =0; b < Sequences[s].Combos[c].NumButtons; b++)
			{
				// Sequences[s].Combos[c].Buttons[b].ButtonNum = BUTTONDATA_ACC_8(off); off+=1;
				Sequences[s].Combos[c].Buttons[b] = &(Buttons[BUTTONDATA_ACC_8(off)]); off+=1;
			}	
		}
	}
}

void TriggerCmd(u8 *cmd, u8 len)
{

}

void UpdateButtons()
{
	u8 numActiveButtons = 0;
	for (u8 b = 0; b < NUM_BUTTONS; b++)
	{
		Buttons[b].Active |= ReadButton(Buttons[b].ButtonNum);
		if (Buttons[b].Active) { numActiveButtons++; }
	}

	for (u8 s = 0; s < NUM_SEQUENCES; s++)
	{
		Combo *combo = &(Sequences[s].Combos[Sequences[s].ActiveCombo]);

		u8 numactive = 0;
		u8 allPressed = 1;
		for (u8 b =0; b < combo->NumButtons; b++)
		{
			if (combo->Buttons[b]->Active) { numactive++; }
			else { allPressed = 0; }
		}
		if (numActiveButtons != numactive)
		{
			//Reset Sequence
			combo->Active = 0; 
			Sequences[s].ActiveCombo = 0;
		}
		else if (allPressed == 1)
		{
			//Combo correct!
			combo->Active = 1;
		}

		if (combo->Active)
		{
			u8 allReleased = 0;
			for (u8 b =0; b < combo->NumButtons; b++)
			{
				allReleased = allReleased | combo->Buttons[b]->Active;
			}
			if (allReleased == 0)
			{
				//Advance to next stage of sequence
				combo->Active = 0;
				Sequences[s].ActiveCombo++;

				if (Sequences[s].ActiveCombo == Sequences[s].NumCombos)
				{ 
					//Sequence Correct!
					TriggerCmd(Sequences[s].Command, Sequences[s].CommandLen);
					
				}
			}
		}
	}
}

u8 ReadButton(u8 id)
{
	u8 b;
	switch (id)
	{ //TODO change to sequential numbers to allow Jump-table optimization
	case 20: b = PINB & (1<<0); //D8
	case 21: b = PINB & (1<<1); //D9
	case 22: b = PINB & (1<<2); //D10
	case 23: b = PINB & (1<<3); //D11
	case 24: b = PINB & (1<<4); //D12
	case 25: b = PINB & (1<<5); //D13

	case 30: b = PINC & (1<<0); //A0
	case 31: b = PINC & (1<<1); //A1
	case 32: b = PINC & (1<<2); //A2
	case 33: b = PINC & (1<<3); //A3
	case 34: b = PINC & (1<<4); //A4
	case 35: b = PINC & (1<<5); //A5

	case 40: b = PIND & (1<<0); //D0
	case 41: b = PIND & (1<<1); //D1
	case 42: b = PIND & (1<<2); //D2
	case 43: b = PIND & (1<<3); //D3
	case 44: b = PIND & (1<<4); //D4
	case 45: b = PIND & (1<<5); //D5
	case 46: b = PIND & (1<<6); //D6
	case 47: b = PIND & (1<<7); //D8
	}
	return !!b;
}

void ReadButtons()
{
	
}

