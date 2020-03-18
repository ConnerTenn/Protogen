
#include "sam.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define REG(t, r) (*(t *)(r))
#define SETREG(t, r, v, m)  ( REG(t, r) = ((REG(t, r) & (~(m))) | ((v) & (m))) )
#define ENBIT(t, r, b) SETREG(t, r, 1<<(b), 1<<(b))
#define DABIT(t, r, b) SETREG(t, r, 0, 1<<(b))

/*
21: PIN Multiplexing Definitions

28: Address Space

41: Periphial Access controller

48 Peripherals Address Space Summary

95: Generic Clock Controller (GCLK)
102: GCLK Register Summary

117: Power manager
127: Power manager Register Summary

148: System Controller (SYSCTRL)
165: SYSCTRL Register Summary

379: PORT section
387: PORT Register Summary

432: Serial Communication Interface (SERCOM)
440: SERCOM USART
454: SERCOM USART Register Summary
478: SERCOM SPI
489: SERCOM SPI Register Summary

*/

int main()
{

	// REG(volatile u32, 0x40000400) = 0;

	// //Periphial Access 0  Enable EIC, WDT, GCLK, SYSCTRL, PM
	// REG(volatile u32, 0x40000000) = 0x0000005E;

	// //Periphial Access 0  Enable EIC, WDT, GCLK, SYSCTRL, PM
	// REG(volatile u32, 0x40000000) = 0x0000005E;

	// //Periphial Access 1  nable DMAC, PORT, RESETVAL(DSU)
	// REG(volatile u32, 0x41000000) = 0x0000001A;

	// //Periphial Access 2  Enable RESETVAL, SERCOM4
	// REG(volatile u32, 0x42000000) = 0x00800040;


	PORT->Group[0].DIRSET.reg = 1<<17; //DDRA17:Out

	//MOSI: DDRB10 SERCOM4 PAD[2]
	//MISO: DDRA12 SERCOM4 PAD[0]
	//SCK:  DDRB11 SERCOM4 PAD[3]

	//DDR
	//MOSI
	PORT->Group[1].DIRSET.reg = 1<<10; //DDRB10:Out 
	//MISO
	PORT->Group[0].DIRCLR.reg = 1<<12; //DDRA12:IN
	//SCK
	PORT->Group[1].DIRSET.reg = 1<<11; //DDRB11:Out
	//SS
	PORT->Group[1].DIRSET.reg = 1<<9; //DDRB09:Out

	PORT->Group[1].OUTSET.reg = (1<<10) | (0<<11); //DDRB10:Out 


	//PINMUX
	// SCK (ODD) Peripheral D (SERCOM4 P3), MOSI (EVEN) Peripheral D (SERCOM4 P2)
	PORT->Group[1].PMUX[10/2].reg = PORT_PMUX_PMUXO_D | PORT_PMUX_PMUXE_D;
	// MISO (EVEN) Peripheral D (SERCOM4 P0)
	PORT->Group[0].PMUX[12/2].reg = PORT_PMUX_PMUXE_D;
	// SS (ODD) Peripheral D (SERCOM4 P1)
	PORT->Group[1].PMUX[9/2].reg = PORT_PMUX_PMUXO_D;

	//PINCFG
	PORT->Group[1].PINCFG[10].bit.PMUXEN = 1; //MOSI
	PORT->Group[0].PINCFG[12].bit.PMUXEN = 1; //MISO
	PORT->Group[1].PINCFG[11].bit.PMUXEN = 1; //SCK
	PORT->Group[1].PINCFG[ 9].bit.PMUXEN = 1; //SS
	
	//Put Generic Clock Generator 1 as source for SERCOM4
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_SERCOM4_CORE) | // Generic Clock Multiplexer SERCOM4
						GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is source
						GCLK_CLKCTRL_CLKEN;
	//Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) { }

	SERCOM4->SPI.CTRLA.bit.SWRST = 1;
	//Wait for synchronization
	while (SERCOM4->SPI.SYNCBUSY.bit.SWRST) {}

	SERCOM4->SPI.CTRLA.reg = 
		SERCOM_SPI_CTRLA_MODE_SPI_MASTER | //Set Master Mode
		SERCOM_SPI_CTRLA_CPOL | SERCOM_SPI_CTRLA_CPHA | //Mode 3: CLK Idle High, sample rising
		SERCOM_SPI_CTRLA_FORM(0) | //SPI Frame Only
		SERCOM_SPI_CTRLA_DIPO(0) | //DIn(MISO) is PAD[0]
		SERCOM_SPI_CTRLA_DOPO(1) | //DOut(MOSI) is PAD[2], SCK is PAD[3]: Mode 1
		SERCOM_SPI_CTRLA_IBON; //STATUS.BUFOVF asserted immediately upon overflow
	//MSB first: commented out //SERCOM4->SPI.CTRLA.reg |= SERCOM_SPI_CTRLA_DORD; 

	SERCOM4->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_CHSIZE(0) | //8 Bit charsize
		SERCOM_SPI_CTRLB_MSSEN; //Hardware controlled SS pin

	//Set Baud Rate
	SERCOM4->SPI.BAUD.reg = SERCOM_SPI_BAUD_BAUD(23); //Target 1MHz: 48MHz/(2*1MHz) - 1 = 23

	//Enable SPI
	SERCOM4->SPI.CTRLA.bit.ENABLE = 1;
	while (SERCOM4->SPI.SYNCBUSY.bit.ENABLE) {}


	PORT->Group[0].OUTCLR.reg = 1<<17; //SS Low
	SERCOM4->SPI.DATA.reg = 0xF2; //Data Out
	while (!SERCOM4->SPI.INTFLAG.bit.TXC) {} //Wait for transmit complete
	PORT->Group[0].OUTSET.reg = 1<<17; //SS High

	PORT->Group[0].OUTCLR.reg = 1<<17; //SS Low
	SERCOM4->SPI.DATA.reg = 0xC5; //Data Out
	while (!SERCOM4->SPI.INTFLAG.bit.TXC) {} //Wait for transmit complete
	PORT->Group[0].OUTSET.reg = 1<<17; //SS High


	u8 i=0; (void)i;
	while (1)
	{
		PORT->Group[0].OUTTGL.reg = 1<<17; //DRA17: Toggle
		// REG(volatile u32, 0x41004400+0x1C) = 1<<17; //DRA17 TOGGLE
	}

	return 0;
}

