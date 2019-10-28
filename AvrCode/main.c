
#include "globals.h"
#include <avr/io.h>
#include <util/delay.h>
#include "interfaces.h"
#include "frames.h"

u32 FrameIndex = 1;
u16 FrameDelay = -1;

ISR(TIMER1_COMPA_vect)
{
	//PORTC = !PORTC;
	// {
	// 	static u8 i=0;
	// 	char buff[] = "Tick  \n"; 
	// 	buff[5] = i+'0'; 
	// 	SerialTransmit(buff, 8);
	// 	i=(i>=9?0:i+1);
	// }

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
	OCR1A = 20000; //(60MHz/4)/1000Hz
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

	SerialTransmit("Setup Complete\n", 16);

	InitTimers();

	while (1)
	{
		// {
		// 	static u8 i=0;
		// 	char buff[] = "Tick  \n"; 
		// 	buff[5] = i+'0'; 
		// 	SerialTransmit(buff, 8);
		// 	i=(i>=9?0:i+1);
		// }
		
		
		//_delay_ms(1000);
	}

	u8 arr[8] = {0,0,0,0, 0,0,0,0};
	while (1)
	{
		if ((arr[0]&0x3F)==0x20)
		{
			PORTC = !PORTC;

			static u8 i=0;
			char buff[] = "Tick  \n"; 
			buff[5] = i+'0'; 
			SerialTransmit(buff, 8);
			i=(i>=9?0:i+1);

			if (SerialAvail())
			{
				u8 in=0; 
				char buff2[] = "Recv  \n"; 
				SerialRead(&in, 1);
				buff2[5] = in; 
				SerialTransmit(buff2, 8);
			}
		}
		
		for (int i=0; i<8; i++)
  		{
			Max7219SendCmd( ((i+1)<<8) | arr[i], 4);
		}


		for (int i=8-1; i>=0; i--)
		{
			if (arr[i] == 0xFF)
			{
			arr[i+1]++;
			arr[i] = 0;
			}
		}
		arr[0]++;

		_delay_ms(10);
	}

	return 0;
}
