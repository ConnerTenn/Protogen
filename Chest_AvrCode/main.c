
#include "globals.h"

#include <stdio.h>

#include "interfaces.h"


//Frame delay update
ISR(TIMER0_COMPA_vect)
{
    sei(); //Enable Nested Interrupts

	static u16 div = 1;
	if (div == 100)
	{
		volatile u8 buttons = PINC;
		for (u8 i = 0; i < 8; i++)
		{
			char buff = ((buttons>>(7-i))&1)+'0';
			SerialTransmit(&buff, 1);
		}
		SerialTransmit("\n", 1);
		div = 1;
	}
	else { div++; }
}

//main callback
ISR(TIMER1_COMPA_vect)
{
    sei(); //Enable Nested Interrupts


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

    //Offset Counter 1 by 1/16 of 1ms (of the overflow value of Counter 0)
    while (TCNT0<OCR0A/16) {} //Wait till offset is reached

    //Counter 1 (16 bit)
    //Page 89
    //Page 108
    OCR1A = 2000 - 1; //(16MHz/8)/1000Hz  1KHz
    TCNT1 = 0;
    TIMSK1 = (1<<OCIE1A); //Enable Interrupt
    //CTC mode & Prescale divide by 8 & start the timer
    TCCR1B = (1<<WGM12) | (0<<CS12)|(1<<CS11)|(0<<CS10); 
    
}

int main()
{
    PRR = 0;//PRR & ~(1<<PRSPI); //Power Reduction Register

    DDRC = 0x00;
	PORTC = 0xFF;

	// PORTC = 0xFF;

    IntiSPI();

    IntiUART();

	

	// PORTC = 0xFF;
    InitTimers();
	// PORTC = 0x00;

    SerialTransmit("\nSetup Complete\n", 17);


    while (1)
    {
    }

    return 0;
}