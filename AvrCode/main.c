
#include "globals.h"
//#include <avr/io.h>
//#include <util/delay.h>

#include <stdio.h>

#include "interfaces.h"
#include "frames.h"

u16 FrameIndex = 1;
u16 FrameDelay = -1;

//u8 FrameBuff1[8*4];

ISR(TIMER1_COMPA_vect)
{
	sei(); //Enable Nested Interrupts
	PORTC = !PORTC;

	if (SerialAvail()>=2)
	{
		/*
		New serial format:
		u8 DeviceAddr
		u8 DestDisplay (implies Display type)
		u16 FrameNumber
		u16 MaskNumber
		u16 BorderNumber
		*/
		u16 in;
		SerialRead((u8 *)&in, 2);
		
		FrameIndex=in;//(in-'0';
		FrameDelay=FrameHeaderAcc(FrameIndex)->FrameDelay;
		
	}

	//memcpy_P(FrameBuff1, FrameDataAcc(FrameIndex), 8*4);
	//memcpy(FrameBuff1, FrameDataAcc(FrameIndex), 8*4);
	//FrameTransDown(FrameBuff1, 4, 2);
	//FrameTransRight(FrameBuff1, 4, 2);

	//Max7219SendFrame(FrameBuff1, 4);
	Max7219Send4Frame(FrameDataAcc(FrameIndex));
	//Max7219SendFrame((u8 *)FrameData + ((Frame *)(FrameData+(u32)0x77790))->FrameOffset, 4);
}

ISR(TIMER0_COMPA_vect)
{
	sei(); //Enable Nested Interrupts
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

	char dat[40];
	//u32 a = FrameHeaderAcc(0x5)->FrameOffset;
	//u32 b = FrameHeaderAcc(0x4)->FrameOffset;
	//sprintf(dat, "%d %d\n", , FrameHeaderAcc(0x04)->FrameOffset);
	SerialTransmit(dat, strlen(dat));

	while (1)
	{

	}

	return 0;
}
