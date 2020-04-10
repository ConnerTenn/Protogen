
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

	SerialTransmit("\033[1F", 4);
	for (u8 b = 0; b < NUM_BUTTONS; b++)
	{
		SerialTransmit(Buttons[b].Active ? "1 " : "0 ", 2);
	}
	SerialTransmit("\n", 1);

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

	InitButtons();

    SerialTransmitStr("\nSetup Complete\n");

#define PRINT_VAL(txt, val) SerialTransmitStr(txt); SerialTransmitHexVal(val); SerialTransmitStr("\n"); SerialFlush();
	
	PRINT_VAL("Timeout:", Timeout);
	
	PRINT_VAL("NumButtons:", NUM_BUTTONS);
	for (u8 b = 0; b < NUM_BUTTONS; b++)
	{
		PRINT_VAL("\t", Buttons[b].ButtonNum);
	}
	
	PRINT_VAL("NumSequences:", NUM_SEQUENCES);
	for (u8 s = 0; s < NUM_SEQUENCES; s++)
	{
		PRINT_VAL("Sequence:", s);

		SerialTransmitStr("\tMomentary:");
		SerialTransmitStr(Sequences[s].Momentary ? "Y\n" : "N\n");

		SerialTransmitStr("\tCommand:");
		for (u8 i = 0; i < Sequences[s].CommandLen; i++)
		{
			SerialTransmitStr("\\x");
			SerialTransmitHexVal(Sequences[s].Command[i] & 0xFF);
		}
		SerialTransmitStr("\n");

		PRINT_VAL("\tNumCombos:", Sequences[s].NumCombos);
	}

	SerialTransmitStr("\n\n\n");

    InitTimers();

    while (1)
    {
    }

    return 0;
}
