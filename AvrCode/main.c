
#include "globals.h"
#include <avr/io.h>
#include <util/delay.h>
#include "interfaces.h"
#include "frames.h"



int main()
{
	PRR = 0;//PRR & ~(1<<PRSPI); //Power Reduction Register

	DDRC = 1;

	IntiSPI();

	Max7219Init(4);
	
	IntiUART();

	SerialTransmit("Setup Complete\n", 16);

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
