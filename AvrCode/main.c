
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(SPI_STC_vect)
{
	PORTC = !PORTC;
}

void SPITransfer16(uint16_t data)
{
	SPDR = (data & 0xFF00) >> 8;
	while(!(SPSR & (1<<SPIF) )) {}
	//_delay_us(100);
	SPDR = data & 0xFF;
	while(!(SPSR & (1<<SPIF) )) {}
	//_delay_us(100);
}

#define CSEN (PORTB=(PORTB&(~(1<<2))) | (1<<2))
#define CSDA (PORTB=(PORTB&(~(1<<2))) | (0<<2))

int main()
{
	PRR = 0;//PRR & ~(1<<PRSPI); //Power Reduction Register

	DDRC = 1;
	DDRB = (1<<2) | (1<<3) | (1<<5); //Must set directions before enabling SPI

	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);//|(1<<SPIE); 
	//sei();
	
	CSEN;

	CSDA; for (int d=0; d<4; d++) { SPITransfer16(0x0F00); } CSEN; _delay_us(10); //Test off
	CSDA; for (int d=0; d<4; d++) { SPITransfer16(0x0C00); } CSEN; _delay_us(10); //Disable
	CSDA; for (int d=0; d<4; d++) { SPITransfer16(0x0C01); } CSEN; _delay_us(10); //Enable
	CSDA; for (int d=0; d<4; d++) { SPITransfer16(0x0A00); } CSEN; _delay_us(10); //Intensity 0
	CSDA; for (int d=0; d<4; d++) { SPITransfer16(0x0100); } CSEN; _delay_us(10); //Decode off
	CSDA; for (int d=0; d<4; d++) { SPITransfer16(0x0B07); } CSEN; _delay_us(10); //Scan 7

	for (int l=1; l<=8; l++)
	{
		CSDA;
		for (int d=0; d<4; d++) { SPITransfer16(l<<8); }
		CSEN;
	}

	uint8_t v=0;
	while (1)
	{
		PORTC = !PORTC;

		CSDA; SPITransfer16(0x0400 | v); CSEN;

		_delay_ms(200);

		v++;
	}

	return 0;
}
