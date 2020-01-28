
#include "globals.h"

#include <stdio.h>

#include "interfaces.h"
#include "frames.h"

typedef struct
{
	u8 CSPin;
	u8 NumSegments;
	u16 FrameIndex;
	u16 FrameDelay;
} Max7219;

Max7219 DisplayList[] = 
	{
		{0, 7, 1, -1},
		{1, 4, 1, -1},
		{2, 2, 1, -1},
	};

ISR(TIMER1_COMPA_vect)
{
	sei(); //Enable Nested Interrupts

	while (SerialAvail()>=4)
	{
		/*
		New serial format:
		u8 DeviceAddr
		u8 DestDisplay (implies Display type)
		u16 FrameNumber
		u16 MaskNumber
		u16 BorderNumber
		*/
		u8 in[4];
		SerialRead(in, 4);

		if (in[0] == 0x56)
		{
			u8 display = in[1];
			u16 frame = *(u16 *)(in+2);
			// FrameIndex=in;//(in-'0';
			// FrameDelay=FrameHeaderAcc(FrameIndex)->FrameDelay;
			DisplayList[display].FrameIndex=frame;
			DisplayList[display].FrameDelay=FrameHeaderAcc(frame)->FrameDelay;
		}
		
	}

	for (u8 i=0; i<sizeof(DisplayList)/sizeof(DisplayList[0]); i++)
	{
		switch (DisplayList[i].NumSegments)
		{
		case 1:
			Max7219Send1Frame(FrameDataAcc(DisplayList[i].FrameIndex), DisplayList[i].CSPin);
			break;
		case 2:
			Max7219Send2Frame(FrameDataAcc(DisplayList[i].FrameIndex), DisplayList[i].CSPin);
			break;
		case 4:
			Max7219Send4Frame(FrameDataAcc(DisplayList[i].FrameIndex), DisplayList[i].CSPin);
			break;
		}
	}
}

ISR(TIMER0_COMPA_vect)
{
	sei(); //Enable Nested Interrupts

	for (u8 i=0; i<sizeof(DisplayList)/sizeof(DisplayList[0]); i++)
	{
		if (DisplayList[i].FrameDelay!=-1)
		{
			DisplayList[i].FrameDelay--;
		}
		if (DisplayList[i].FrameDelay==0)
		{
			DisplayList[i].FrameDelay=FrameHeaderAcc(DisplayList[i].FrameIndex)->FrameDelay;
			DisplayList[i].FrameIndex=FrameHeaderAcc(DisplayList[i].FrameIndex)->FrameNext;
		}
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

	DDRC = 0xF;

	IntiSPI();
	for (u8 i=0; i<sizeof(DisplayList)/sizeof(DisplayList[0]); i++)
	{
		Max7219Init(DisplayList[i].NumSegments, DisplayList[i].CSPin);
	}
	IntiUART();

	InitTimers();

	SerialTransmit("\nSetup Complete\n", 17);

	while (1)
	{

	}

	return 0;
}
