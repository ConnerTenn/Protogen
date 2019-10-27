
#include "interfaces.h"


ISR(SPI_STC_vect)
{
	//PORTC = !PORTC;
}

void IntiSPI()
{
	//Setup GPIO Pins
	//Must set directions before enabling SPI
	ENBITS(DDRB,(1<<DDRB_MOSI) | (1<<DDRB_SCK) | (1<<DDRB_CS));

	//Setup SPI
	SPCR = (1<<SPE) | (1<<MSTR);//|(1<<SPR0);//|(1<<SPIE); 
	SPSR = (1<<SPI2X);

	//sei();
}

void SPITransmit16(u16 data)
{
	SPDR = (data & 0xFF00) >> 8;
	while(!(SPSR & (1<<SPIF) )) {}
	//_delay_us(100);
	SPDR = data & 0xFF;
	while(!(SPSR & (1<<SPIF) )) {}
	//_delay_us(100);
}



#define CSEN ENBITS(PORTB, 1<<DDRB_CS) //(PORTB=(PORTB&(~(1<<2))) | (1<<2))
#define CSDA DABITS(PORTB, 1<<DDRB_CS) //(PORTB=(PORTB&(~(1<<2))) | (0<<2))

void Max7219Init(u8 numDisplays)
{
	CSEN;

	Max7219SendCmd(0x0C00, numDisplays); //Disable
	Max7219SendCmd(0x0F00, numDisplays); //Test off
	Max7219SendCmd(0x0A00, numDisplays); //Intensity 0
	Max7219SendCmd(0x0900, numDisplays); //Decode off
	Max7219SendCmd(0x0B07, numDisplays); //Scan 7

	for (int l=1; l<=8; l++)
	{
		Max7219SendCmd(l<<8, numDisplays); 
	}

	Max7219SendCmd(0x0C01, numDisplays);  //Enable
}

void Max7219SendCmd(u16 cmd, u8 numDisplays)
{
	CSDA; 
	for (u8 d=0; d<numDisplays; d++) 
	{ 
		SPITransmit16(cmd); 
	} 
	CSEN;
}