
#include "interfaces.h"


ISR(SPI_STC_vect)
{
	//PORTC = !PORTC;
}

void IntiSPI()
{
	//Setup GPIO Pins
	//Must set directions before enabling SPI
	ENBITS(DDRB,(1<<DDR_MOSI) | (1<<DDR_SCK));

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

