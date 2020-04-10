
#include "globals.h"

#include <stdio.h>

#include "interfaces.h"
#include "buttonCtl.h"


//Frame delay update
ISR(TIMER0_COMPA_vect)
{
    sei(); //Enable Nested Interrupts

	UpdateButtons();

}

//main callback
ISR(TIMER1_COMPA_vect)
{
    sei(); //Enable Nested Interrupts

	SerialTransmitStr("\033[1F");
	for (u8 b = 0; b < NUM_BUTTONS; b++)
	{
		SerialTransmitStr(Buttons[b].Active ? "1 " : "0 ");
	}
	SerialTransmitStr("\n");
	SerialFlush();

}




void InitTimers()
{
    //Counter 0 (8 bit)
    //Page 84
    TCCR0A = (1<<WGM01);
    OCR0A = 250 - 1; //(16MHz/64)/1000Hz  1KHz
    TCNT0 = 0;
    TIMSK0 = (1<<OCIE0A); //Enable Interrupt
    //CTC mode & Prescaler divide by 64 & start the timer
    TCCR0B = (0<<CS02)|(1<<CS01)|(1<<CS00); 

    // //Offset Counter 1 by 1/16 of 1ms (of the overflow value of Counter 0)
    // while (TCNT0<OCR0A/16) {} //Wait till offset is reached

    //Counter 1 (16 bit)
    //Page 89
    //Page 108
    OCR1A = 6250 - 1; //(16MHz/256)/10Hz=6250
    TCNT1 = 0;
    TIMSK1 = (1<<OCIE1A); //Enable Interrupt
    //CTC mode & Prescale divide by 256 & start the timer
    TCCR1B = (1<<WGM12) | (1<<CS12)|(0<<CS11)|(0<<CS10); 
    
}

int main()
{
    PRR = 0;//PRR & ~(1<<PRSPI); //Power Reduction Register

    DDRC = 0x00;
	PORTC = 0xFF;


    IntiUART();
	SerialTransmitStr("\n");

	InitButtons();

    SerialTransmitStr("\nSetup Complete\n");

	PRINT_VAL("Timeout:", Timeout);
	
	PRINT_VAL("NumSequences:", NUM_SEQUENCES);
	for (u8 s = 0; s < NUM_SEQUENCES; s++)
	{
		PRINT_VAL("Sequence:", s);

		SerialTransmitStr("\tMomentary:");
		SerialTransmitStr(Sequences[s].Momentary ? "Y\n" : "N\n");

		PRINT_VAL("\tCommandLen:", Sequences[s].CommandLen);
		PRINT_VAL("\tCommandOff:", Sequences[s].Command - CommandData);
		SerialTransmitStr("\tCommand:");
		for (u8 i = 0; i < Sequences[s].CommandLen; i++)
		{
			SerialTransmitStr("\\x");
			SerialTransmitHexVal(Sequences[s].Command[i] & 0xFF);
		}
		SerialTransmitStr("\n");

		PRINT_VAL("\tNumCombos:", Sequences[s].NumCombos);
		for (u8 c = 0; c < Sequences[s].NumCombos; c++)
		{
			PRINT_VAL("\t\tNumButtons:", Sequences[s].Combos[c]->NumButtons);
			for (u8 b = 0; b < Sequences[s].Combos[c]->NumButtons; b++)
			{
				PRINT_VAL("\t\t\tButtonsId:", Sequences[s].Combos[c]->Buttons[b]->ButtonID);
			}
		}
	}

	SerialTransmitStr("\n\n\n"); SerialFlush();

    // InitTimers();

    while (1)
    {
    }

    return 0;
}
