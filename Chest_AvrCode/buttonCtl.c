
#include "buttonCtl.h"

u8 ButtonStructData[BUTTON_STRUCT_SIZE];
u8 Timeout;
u8 CommandData[COMMAND_DATA_LEN];
Button Buttons[NUM_BUTTONS];
Combo *Combos[NUM_COMBOS];
Sequence *Sequences[NUM_SEQUENCES];

void InitButtons()
{
	//Read offset: Flash data read offset
	//Write offset: Offset into ButtonStructData for writing structures
	u8 *roff = (u8 *)BIN_BUTTONDATA_START; u8 *woff = ButtonStructData;
	
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
	u8 pB=0, pC=0, pD=0;
	for (u8 b = 0; b < NUM_BUTTONS; b++)
	{
		// Buttons[b].Pressed = 0;
		// Buttons[b].Active = 0;
		Buttons[b].ButtonID = BUTTONDATA_ACC_8(roff); roff+=1;
	
		// PRINT_VAL("\tButtonID:", Buttons[b].ButtonID);

		
		//Setup Direction and pull-up registers
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

	//Set bits in registers according to the bits configured
	PORTB|=pB; PORTC|=pC; PORTD|=pD; //Set PORTx to high for Pull-up
	pB=~pB;    pC=~pC;    pD=~pD; //flip bits so that the next register bits can be cleared
	DDRB&=pB;  DDRC&=pC;  DDRD&=pD; //Set DDRx to low for input

	// SerialTransmitStr("Combos:\n");
	for (u8 c = 0; c < NUM_COMBOS; c++)
	{
		Combos[c] = (Combo *)woff;

		// Combos[c]->Active = 0;
		Combos[c]->NumButtons = BUTTONDATA_ACC_8(roff); roff+=1;
		// PRINT_VAL("\tNumButtons:", Combos[c]->NumButtons);

		woff+=1+1;

		for (u8 b = 0; b < Combos[c]->NumButtons; b++)
		{
			u8 buttonidx = BUTTONDATA_ACC_8(roff); roff+=1;
			Combos[c]->Buttons[b] = &(Buttons[buttonidx]);
			// PRINT_VAL("\t\tButtonIdx:", buttonidx);

			woff+=2;
		}

	}


	// SerialTransmitStr("Sequences:\n");
	for (u8 s = 0; s < NUM_SEQUENCES; s++)
	{
		Sequences[s] = (Sequence *)woff;

		Sequences[s]->ActiveCombo = 0;
		Sequences[s]->Momentary = BUTTONDATA_ACC_8(roff); roff+=1;
		// SerialTransmitStr("\tMomentary:"); SerialTransmitStr(Sequences[s]->Momentary ? "Y\n" : "N\n");

		Sequences[s]->CommandLen = BUTTONDATA_ACC_8(roff); roff+=1;
		u16 cmdoff = BUTTONDATA_ACC_16(roff); roff+=2;
		Sequences[s]->Command = CommandData + cmdoff;
		// PRINT_VAL("\tCommandLen:", Sequences[s]->CommandLen);
		// PRINT_VAL("\tCommandOff:", cmdoff);

		Sequences[s]->NumCombos = BUTTONDATA_ACC_8(roff); roff+=1;

		woff+=1+1+1+2+1;

		// PRINT_VAL("\tNumCombos:", Sequences[s]->NumCombos);
		for (u8 c = 0; c < Sequences[s]->NumCombos; c++)
		{
			u8 comboidx = BUTTONDATA_ACC_8(roff); roff+=1;
			Sequences[s]->Combos[c] = Combos[comboidx];
			// PRINT_VAL("\t\tComboIdx:", comboidx);

			woff+=2;
		}
	}


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

	//Update the states of all Physical Buttons
	for (u8 b = 0; b < NUM_BUTTONS; b++)
	{
		//Get the buttons state
		Buttons[b].Pressed = ReadButton(Buttons[b].ButtonID);
		
		allReleased |= Buttons[b].Pressed; //Test of if no buttons are pressed
		Buttons[b].Active |= Buttons[b].Pressed; //Once pressed, it is active until reset

		if (Buttons[b].Active) { numActiveButtons++; } //Count the number of active buttons
	}

	//Check each sequence
	for (u8 s = 0; s < NUM_SEQUENCES && !triggered; s++)
	{
		Combo *combo = Sequences[s]->Combos[Sequences[s]->ActiveCombo];

		u8 numactive = 0;
		u8 allComboPressed = 1;

		//Count the number of active buttons
		//If all the buttons are pressed, then allComboPressed remains true
		for (u8 b = 0; b < combo->NumButtons; b++)
		{
			if (combo->Buttons[b]->Active) { numactive++; }
			else { allComboPressed = 0; }
		}

		//Checking for if there are buttons pressed that are not in this sequence
		if (numActiveButtons != numactive)
		{
			//Reset Sequence
			combo->Active = 0; 
			Sequences[s]->ActiveCombo = 0;
		}
		else if (allComboPressed == 1)
		{
			//Combo correct!
			//Set active and wait for all to be released for the command to trigger
			combo->Active = 1;
		}

		//If all buttons have been now released and the combination is correct
		//then can move on to the next stage of the sequence (or trigger)
		if (allReleased == 0 && combo->Active)
		{
			//Advance to next stage of sequence
			combo->Active = 0;
			Sequences[s]->ActiveCombo++;

			//If the last stage of the sequence has been passed,
			//then the entire sequence has been correct
			if (Sequences[s]->ActiveCombo == Sequences[s]->NumCombos)
			{ 
				//Sequence Correct!
				TriggerCmd(Sequences[s]->Command, Sequences[s]->CommandLen);
				triggered = 1;
			}
		}
	}
	
	//If all buttons have been released, reset their active state
	//This ensures that buttons aren't 'stick' which can cause false triggers
	if (allReleased == 0)
	{
		for (u8 b = 0; b < NUM_BUTTONS; b++)
		{
			Buttons[b].Active = 0;
		}
	}

	//If a command has been triggered, reset all the sequences
	if (triggered)
	{
		for (u8 s = 0; s < NUM_SEQUENCES; s++)
		{
			Sequences[s]->ActiveCombo = 0;
		}
	}
}

u8 ReadButton(u8 id)
{
#define GET_INPUT(port, bit) (PIN##port & (1<<(bit)))

	//See Pin Mapping in buttonCtl.h
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
