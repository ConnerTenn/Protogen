
#include "globals.h"

#include <stdio.h>

#include "interfaces.h"

	
//main callback
ISR(TIMER1_COMPA_vect)
{
	sei(); //Enable Nested Interrupts

	static u8 toggle = 0;
	if (toggle)
	{
		SETBITS(PORTC, 0x1, 0x1);
		SETBITS(PORTC, 0x0, 0x1);
	}
	toggle=!toggle;
}

//Frame delay update
ISR(TIMER0_COMPA_vect)
{
	sei(); //Enable Nested Interrupts

	static u16 div = 50;
	static u16 freq10 = 10;
	static u8 dir = 0;
	if (div==50)
	{
		if (!dir)
		{
			if (freq10<60) { freq10++; }
			else { dir = 1; }
		}
		else
		{
			if (freq10>10) { freq10--; }
			else { dir = 0; }
		}
		OCR1A = (u16)50000 / freq10; //(16000000/(8*2))/(200/10)
		if (TCNT1 >= OCR1A) { TCNT1 = OCR1A-1; }
		div=0;
	}
	else { div++; }
}

void InitTimers()
{
	//Counter 0 (8 bit)
	//Page 84
	TCCR0A = (1<<WGM01);
	OCR0A = 250 - 1; //(16MHz/64)/1000Hz=250  1KHz
	TCNT0 = 0;
	TIMSK0 = (1<<OCIE0A); //Enable Interrupt
	//CTC mode & Prescaler divide by 64 & start the timer
	TCCR0B = (0<<CS02)|(1<<CS01)|(1<<CS00); 

	// //Offset Counter 1 by 1/16 of 1ms (of the overflow value of Counter 0)
	// while (TCNT0<OCR0A/16) {} //Wait till offset is reached

	//Counter 1 (16 bit)
	//Page 89
	//Page 108
	OCR1A = 10000 - 1; //(16MHz/8)/1000Hz  1KHz
	TCNT1 = 0;
	TIMSK1 = (1<<OCIE1A); //Enable Interrupt
	//CTC mode & Prescale divide by 8 & start the timer
	TCCR1B = (1<<WGM12) | (0<<CS12)|(1<<CS11)|(0<<CS10); 
	
}

int main()
{
	PRR = 0;//PRR & ~(1<<PRSPI); //Power Reduction Register

	DDRC = 0xF;
	PORTC = 0x00;
	SETBITS(PORTC, 0x0, 0x2);

	IntiSPI();

	IntiUART();

	InitTimers();

	SerialTransmit("\nSetup Complete\n", 17);

	while (1)
	{

	}

	return 0;
}
