
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "globals.h"
#include "interfaces.h"





#define CSEN ENBITS(PORTB, 1<<DRB_CS) //(PORTB=(PORTB&(~(1<<2))) | (1<<2))
#define CSDA DABITS(PORTB, 1<<DRB_CS) //(PORTB=(PORTB&(~(1<<2))) | (0<<2))

int main()
{
	PRR = 0;//PRR & ~(1<<PRSPI); //Power Reduction Register

	DDRC = 1;
	ENBITS(DDRB,(1<<2));
	CSDA;

	IntiSPI();
	

	CSEN;
	CSDA; for (int d=0; d<4; d++) { SPITransmit16(0x0C00); } CSEN; //Disable
	CSDA; for (int d=0; d<4; d++) { SPITransmit16(0x0F00); } CSEN;  //Test off
	CSDA; for (int d=0; d<4; d++) { SPITransmit16(0x0A00); } CSEN; //Intensity 0
	CSDA; for (int d=0; d<4; d++) { SPITransmit16(0x0900); } CSEN; //Decode off
	CSDA; for (int d=0; d<4; d++) { SPITransmit16(0x0B07); } CSEN; //Scan 7
	for (int l=1; l<=8; l++)
	{
		CSDA;
		for (int d=0; d<4; d++) { SPITransmit16(l<<8); }
		CSEN;
	}
	CSDA; for (int d=0; d<4; d++) { SPITransmit16(0x0C01); } CSEN; //Enable

	u8 arr[8] = {0,0,0,0, 0,0,0,0};
	while (1)
	{
		if ((arr[0]&0x3F)==0x20)
		{
			PORTC = !PORTC;
		}
		
		for (int i=0; i<8; i++)
  		{
			CSDA; SPITransmit16( ((i+1)<<8) | arr[i]); CSEN;
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
