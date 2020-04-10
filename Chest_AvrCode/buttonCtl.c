
#include "buttonCtl.h"

u8 ButtonStructData[BUTTON_STRUCT_SIZE];
u8 Timeout;
u8 CommandData[COMMAND_DATA_LEN];
Button Buttons[NUM_BUTTONS];
Combo *Combos[NUM_COMBOS];
Sequence *Sequences[NUM_SEQUENCES];

void InitButtons()
{
	u16 roff = 0; u8 *woff = ButtonStructData;
	
	Timeout = BUTTONDATA_ACC_8(roff); roff+=1;
	// PRINT_VAL("Timeout:", Timeout);

	// SerialTransmitStr("CommandData:");
	for (u16 i = 0; i < COMMAND_DATA_LEN; i++)
	{
		CommandData[i] = BUTTONDATA_ACC_8(roff); roff+=1;

		// SerialTransmitStr("\\x");
		// SerialTransmitHexVal(CommandData[i] & 0xFF);
	}
	// SerialTransmitStr("\n");

	// SerialTransmitStr("Buttons:\n");
	for (u8 b = 0; b < NUM_BUTTONS; b++)
	{
		Buttons[b].Pressed = 0;
		Buttons[b].Active = 0;
		Buttons[b].ButtonID = BUTTONDATA_ACC_8(roff); roff+=1;
	
		// PRINT_VAL("\tButtonID:", Buttons[b].ButtonID);
	}

	// SerialTransmitStr("Combos:\n");
	for (u8 c = 0; c < NUM_COMBOS; c++)
	{
		Combos[c] = (Combo *)woff;

		Combos[c]->Active = 0; woff+=1;
		Combos[c]->NumButtons = BUTTONDATA_ACC_8(roff); roff+=1; woff+=1;
		// PRINT_VAL("\tNumButtons:", Combos[c]->NumButtons);

		for (u8 b = 0; b < Combos[c]->NumButtons; b++)
		{
			u8 buttonidx = BUTTONDATA_ACC_8(roff); roff+=1;
			Combos[c]->Buttons[b] = &(Buttons[buttonidx]); woff+=2;
			// PRINT_VAL("\t\tButtonIdx:", buttonidx);
		}

	}


	// SerialTransmitStr("Sequences:\n");
	for (u8 s = 0; s < NUM_SEQUENCES; s++)
	{
		Sequences[s] = (Sequence *)woff;

		Sequences[s]->ActiveCombo = 0; woff+=1;
		Sequences[s]->Momentary = BUTTONDATA_ACC_8(roff); roff+=1; woff+=1;
		// SerialTransmitStr("\tMomentary:"); SerialTransmitStr(Sequences[s]->Momentary ? "Y\n" : "N\n");

		Sequences[s]->CommandLen = BUTTONDATA_ACC_8(roff); roff+=1; woff+=1;
		u16 cmdoff = BUTTONDATA_ACC_16(roff); roff+=2;
		Sequences[s]->Command = CommandData + cmdoff; woff+=2;
		// PRINT_VAL("\tCommandLen:", Sequences[s]->CommandLen);
		// PRINT_VAL("\tCommandOff:", cmdoff);

		Sequences[s]->NumCombos = BUTTONDATA_ACC_8(roff); roff+=1; woff+=1;
		// PRINT_VAL("\tNumCombos:", Sequences[s]->NumCombos);
		for (u8 c = 0; c < Sequences[s]->NumCombos; c++)
		{
			u8 comboidx = BUTTONDATA_ACC_8(roff); roff+=1;
			Sequences[s]->Combos[c] = Combos[comboidx]; woff+=2;
			// PRINT_VAL("\t\tComboIdx:", comboidx);
		}
	}

	u8 pB=0, pC=0, pD=0;
	for (u8 b = 0; b < NUM_BUTTONS; b++)
	{
		/*
		 0: B, bit0 //D8
		 1: B, bit1 //D9
		 2: B, bit2 //D10
		 3: B, bit3 //D11
		 4: B, bit4 //D12
		 5: B, bit5 //D13

		 6: C, bit0 //A0
		 7: C, bit1 //A1
		 8: C, bit2 //A2
		 9: C, bit3 //A3
		10: C, bit4 //A4
		11: C, bit5 //A5

		12: D, bit0 //D0
		13: D, bit1 //D1
		14: D, bit2 //D2
		15: D, bit3 //D3
		16: D, bit4 //D4
		17: D, bit5 //D5
		18: D, bit6 //D6
		19: D, bit7 //D7
		*/

#define CFG_BUTTON_PIN(port, bit) p##port |= (1<<bit);
		u8 id = Buttons[b].ButtonID;
		if (id<=5)
		{
			CFG_BUTTON_PIN(B, id);
		}
		else if (id<=11)
		{
			CFG_BUTTON_PIN(C, id-6);
		}
		else if (id<=19)
		{
			CFG_BUTTON_PIN(D, id-12);
		}
	}

	PORTB|=pB; PORTC|=pC; PORTD|=pD;
	pB=~pB;    pC=~pC;    pD=~pD;
	DDRB&=pB;  DDRC&=pC;  DDRD&=pD;

}

#ifdef DEBUG
u8 *LastCmd;
u8 LastCmdLen;
#endif

void TriggerCmd(u8 *cmd, u8 len)
{
#ifndef DEBUG
	SerialTransmit(cmd, len);
#else
	// SerialTransmitStr("\n\n\n\n");
	// SerialTransmitStr("\tCommand:");
	// for (u8 i = 0; i < len; i++)
	// {
	// 	SerialTransmitStr("\\x");
	// 	SerialTransmitHexVal(cmd[i] & 0xFF);
	// }
	// SerialTransmitStr("\n\n\n\n");
	LastCmd = cmd;
	LastCmdLen = len;
#endif
}

void UpdateButtons()
{
	u8 numActiveButtons = 0;
	u8 allReleased = 0;
	u8 triggered = 0;

	for (u8 b = 0; b < NUM_BUTTONS; b++)
	{
		Buttons[b].Pressed = ReadButton(Buttons[b].ButtonID);
		allReleased |= Buttons[b].Pressed;
		Buttons[b].Active |= Buttons[b].Pressed;
		if (Buttons[b].Active) { numActiveButtons++; }
	}

	for (u8 s = 0; s < NUM_SEQUENCES && !triggered; s++)
	{
		Combo *combo = Sequences[s]->Combos[Sequences[s]->ActiveCombo];

		u8 numactive = 0;
		u8 allComboPressed = 1;
		for (u8 b = 0; b < combo->NumButtons; b++)
		{
			if (combo->Buttons[b]->Active) { numactive++; }
			else { allComboPressed = 0; }
		}
		if (numActiveButtons != numactive)
		{
			//Reset Sequence
			combo->Active = 0; 
			Sequences[s]->ActiveCombo = 0;
		}
		else if (allComboPressed == 1)
		{
			//Combo correct!
			combo->Active = 1;
		}

		if (combo->Active)
		{
			// u8 allReleased = 0;
			// for (u8 b =0; b < combo->NumButtons; b++)
			// {
			// 	allReleased = allReleased | combo->Buttons[b]->Active;
			// }
			if (allReleased == 0)
			{
				//Advance to next stage of sequence
				combo->Active = 0;
				Sequences[s]->ActiveCombo++;

				if (Sequences[s]->ActiveCombo == Sequences[s]->NumCombos)
				{ 
					//Sequence Correct!
					TriggerCmd(Sequences[s]->Command, Sequences[s]->CommandLen);
					triggered = 1;
				}
			}
		}
	}

	if (triggered)
	{
		for (u8 b = 0; b < NUM_BUTTONS; b++)
		{
			Buttons[b].Active = 0;
		}
		for (u8 s = 0; s < NUM_SEQUENCES; s++)
		{
			Sequences[s]->ActiveCombo = 0;
			for (u8 c = 0; c < Sequences[s]->NumCombos; c++)
			{
				Sequences[s]->Combos[c]->Active = 0;
				// for (u8 b = 0; b < Sequences[s]->Combos[c]->NumButtons; b++)
				// {
				// 	Sequences[s]->Combos[c]->Buttons[b].Active = 0;
				// }
			}
		}
	}
}

u8 ReadButton(u8 id)
{

	/*
	0: B, bit0 //D8
	1: B, bit1 //D9
	2: B, bit2 //D10
	3: B, bit3 //D11
	4: B, bit4 //D12
	5: B, bit5 //D13

	6: C, bit0 //A0
	7: C, bit1 //A1
	8: C, bit2 //A2
	9: C, bit3 //A3
	10: C, bit4 //A4
	11: C, bit5 //A5

	12: D, bit0 //D0
	13: D, bit1 //D1
	14: D, bit2 //D2
	15: D, bit3 //D3
	16: D, bit4 //D4
	17: D, bit5 //D5
	18: D, bit6 //D6
	19: D, bit7 //D7
	*/

#define GET_INPUT(port, bit) (PIN##port & (1<<(bit)))

	u8 v = 0;
	if (id<=5)
	{
		v = GET_INPUT(B, id);
	}
	else if (id<=11)
	{
		v = GET_INPUT(C, id-6);
	}
	else if (id<=19)
	{
		v = GET_INPUT(D, id-12);
	}
	return !v;
}
