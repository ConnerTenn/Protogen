
#include "globals.h"
//#include <avr/io.h>
//#include <util/delay.h>
#include "interfaces.h"
#include "frames.h"

u32 FrameIndex = 1;
u16 FrameDelay = -1;

ISR(TIMER1_COMPA_vect)
{
	PORTC = !PORTC;

	if (SerialAvail())
	{
		u8 in;
		SerialRead(&in, 1);
		FrameIndex=in-'0';
		FrameDelay=FrameHeaderAcc(FrameIndex)->FrameDelay;
	}

	Max7219SendFrame(FrameDataAcc(FrameIndex), 4);
}

ISR(TIMER0_COMPA_vect)
{

	if (FrameDelay!=-1)
	{
		FrameDelay--;
	}
	if (FrameDelay==0)
	{
		FrameDelay=FrameHeaderAcc(FrameIndex)->FrameDelay;
		FrameIndex=FrameHeaderAcc(FrameIndex)->FrameNext;
	}
	
}

void InitTimers()
{
	//Counter 1
	//Page 89
	//Page 108
	OCR1A = 2000; //(60MHz/8)/1000Hz
	TCNT1 = 0;
	TIMSK1 = (1<<OCIE1A); //Enable Interrupt
	//CTC mode & Prescale divide by 64 & start the timer
	TCCR1B = (1<<WGM12) | (0<<CS12)|(1<<CS11)|(0<<CS10); 
	
	//Counter 0
	//Page 84
	TCCR0A = (1<<WGM01);
	OCR0A = 250; //(60MHz/64)/1000Hz
	TCNT0 = 0;
	TIMSK0 = (1<<OCIE0A); //Enable Interrupt
	//CTC mode & Prescaler & start the timer
	TCCR0B = (0<<CS02)|(1<<CS01)|(1<<CS00); 
}

int main()
{
	PRR = 0;//PRR & ~(1<<PRSPI); //Power Reduction Register

	DDRC = 1;

	IntiSPI();
	Max7219Init(4);
	IntiUART();

	InitTimers();

	SerialTransmit("\nSetup Complete\n", 17);


	while (1)
	{

	}

	return 0;
}
