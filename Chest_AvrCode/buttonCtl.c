
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
	PRINT_VAL("Timeout:", Timeout);

	SerialTransmitStr("CommandData:");
	for (u16 i = 0; i < COMMAND_DATA_LEN; i++)
	{
		CommandData[i] = BUTTONDATA_ACC_8(roff); roff+=1;

		SerialTransmitStr("\\x");
		SerialTransmitHexVal(CommandData[i] & 0xFF);
	}
	SerialTransmitStr("\n");

	SerialTransmitStr("Buttons:\n");
	for (u8 b = 0; b < NUM_BUTTONS; b++)
	{
		Buttons[b].ButtonID = BUTTONDATA_ACC_8(roff); roff+=1;
		Buttons[b].Active = 0;
	
		PRINT_VAL("\tButtonID:", Buttons[b].ButtonID);
	}

	SerialTransmitStr("Combos:\n");
	for (u8 c = 0; c < NUM_COMBOS; c++)
	{
		Combos[c] = (Combo *)woff;

		Combos[c]->Active = 0; woff+=1;
		Combos[c]->NumButtons = BUTTONDATA_ACC_8(roff); roff+=1; woff+=1;
		PRINT_VAL("\tNumButtons:", Combos[c]->NumButtons);

		for (u8 b = 0; b < Combos[c]->NumButtons; b++)
		{
			u8 buttonidx = BUTTONDATA_ACC_8(roff); roff+=1;
			Combos[c]->Buttons[b] = &(Buttons[buttonidx]); woff+=2;
			PRINT_VAL("\t\tButtonIdx:", buttonidx);
		}

	}


	SerialTransmitStr("Sequences:\n");
	for (u8 s = 0; s < NUM_SEQUENCES; s++)
	{
		Sequences[s] = (Sequence *)woff;

		Sequences[s]->ActiveCombo = 0; woff+=1;
		Sequences[s]->Momentary = BUTTONDATA_ACC_8(roff); roff+=1; woff+=1;
		SerialTransmitStr("\tMomentary:"); SerialTransmitStr(Sequences[s]->Momentary ? "Y\n" : "N\n");

		Sequences[s]->CommandLen = BUTTONDATA_ACC_8(roff); roff+=1; woff+=1;
		u16 cmdoff = BUTTONDATA_ACC_16(roff); roff+=2;
		Sequences[s]->Command = CommandData + cmdoff; woff+=2;
		PRINT_VAL("\tCommandLen:", Sequences[s]->CommandLen);
		PRINT_VAL("\tCommandOff:", cmdoff);

		Sequences[s]->NumCombos = BUTTONDATA_ACC_8(roff); roff+=1; woff+=1;
		PRINT_VAL("\tNumCombos:", Sequences[s]->NumCombos);
		for (u8 c = 0; c < Sequences[s]->NumCombos; c++)
		{
			u8 comboidx = BUTTONDATA_ACC_8(roff); roff+=1;
			Sequences[s]->Combos[c] = Combos[comboidx]; woff+=2;
			PRINT_VAL("\t\tComboIdx:", comboidx);
		}
	}


	for (u8 b = 0; b < NUM_BUTTONS; b++)
	{
		switch(Buttons[b].ButtonID)
		{ //TODO change to sequential numbers to allow Jump-table optimization

#define CFG_BUTTON_PIN(port, bit) \
		DDR##port = DABITS(DDR##port, 1<<bit); \
		PORT##port = ENBITS(PORT##port, 1<<bit); \
		break;

		case 20: CFG_BUTTON_PIN(B, 0) //D8
		case 21: CFG_BUTTON_PIN(B, 1) //D9
		case 22: CFG_BUTTON_PIN(B, 2) //D10
		case 23: CFG_BUTTON_PIN(B, 3) //D11
		case 24: CFG_BUTTON_PIN(B, 4) //D12
		case 25: CFG_BUTTON_PIN(B, 5) //D13

		case 30: CFG_BUTTON_PIN(C, 0) //A0
		case 31: CFG_BUTTON_PIN(C, 1) //A1
		case 32: CFG_BUTTON_PIN(C, 2) //A2
		case 33: CFG_BUTTON_PIN(C, 3) //A3
		case 34: CFG_BUTTON_PIN(C, 4) //A4
		case 35: CFG_BUTTON_PIN(C, 5) //A5

		case 40: CFG_BUTTON_PIN(D, 0) //D0
		case 41: CFG_BUTTON_PIN(D, 0) //D1
		case 42: CFG_BUTTON_PIN(D, 0) //D2
		case 43: CFG_BUTTON_PIN(D, 0) //D3
		case 44: CFG_BUTTON_PIN(D, 0) //D4
		case 45: CFG_BUTTON_PIN(D, 0) //D5
		case 46: CFG_BUTTON_PIN(D, 0) //D6
		case 47: CFG_BUTTON_PIN(D, 0) //D7

#undef CFG_BUTTON_PIN

		}
	}
}

void TriggerCmd(u8 *cmd, u8 len)
{
	SerialTransmit(cmd, len);
	SerialTransmit("\n", 1);
}

void UpdateButtons()
{
	/*
	u8 numActiveButtons = 0;
	for (u8 b = 0; b < NUM_BUTTONS; b++)
	{
		Buttons[b].Active |= ReadButton(Buttons[b].ButtonID);
		if (Buttons[b].Active) { numActiveButtons++; }
	}

	for (u8 s = 0; s < NUM_SEQUENCES; s++)
	{
		Combo *combo = Sequences[s].Combos[Sequences[s].ActiveCombo];

		u8 numactive = 0;
		u8 allPressed = 1;
		for (u8 b = 0; b < combo->NumButtons; b++)
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
	*/
}

u8 ReadButton(u8 id)
{
	u8 b;
	switch (id)
	{ //TODO change to sequential numbers to allow Jump-table optimization

#define GET_INPUT(port, bit) \
		b = PIN##port & (1<<bit); \
		break;

	case 20: GET_INPUT(B, 0) //D8
	case 21: GET_INPUT(B, 1) //D9
	case 22: GET_INPUT(B, 2) //D10
	case 23: GET_INPUT(B, 3) //D11
	case 24: GET_INPUT(B, 4) //D12
	case 25: GET_INPUT(B, 5) //D13

	case 30: GET_INPUT(C, 0) //A0
	case 31: GET_INPUT(C, 1) //A1
	case 32: GET_INPUT(C, 2) //A2
	case 33: GET_INPUT(C, 3) //A3
	case 34: GET_INPUT(C, 4) //A4
	case 35: GET_INPUT(C, 5) //A5

	case 40: GET_INPUT(D, 0) //D0
	case 41: GET_INPUT(D, 0) //D1
	case 42: GET_INPUT(D, 0) //D2
	case 43: GET_INPUT(D, 0) //D3
	case 44: GET_INPUT(D, 0) //D4
	case 45: GET_INPUT(D, 0) //D5
	case 46: GET_INPUT(D, 0) //D6
	case 47: GET_INPUT(D, 0) //D7

#undef GET_INPUT

	}
	return !b;
}
