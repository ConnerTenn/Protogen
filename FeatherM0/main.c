
#include "interfaces.h"




/*
Pin Definitions

         A         B              C          D          E           F           G           H        
PIN      EIC       ADC,AC,DAC     SERCOM     SERCOMALT  TC/TCC      TCC         COM         AC/GCLK  
-----------------------------------------------------------------------------------------------------
40:RST   _         _              _          _          _           _           _           _        
3.3V     _         _              _          _          _           _           _           _        
4 :PA03  EINT[3]   AIN[1]         _          _          _           _           _           _        
GND      _         _              _          _          _           _           _           _        
3 :PA02  EINT[2]   AIN[0],DAC     _          _          _           _           _           _        
7 :PB08  EINT[8]   AIN[2]         _          SERCOM4:0  TC4/WO[0]   _           _           _        
8 :PB09  EINT[9]   AIN[3]         _          SERCOM4:1  TC4/WO[1]   _           _           _        
9 :PA04  EINT[4]   AIN[4],AIN[0]  _          SERCOM0:0  TCC0/WO[0]  _           _           _        
10:PA05  EINT[5]   AIN[5],AIN[1]  _          SERCOM0:1  TCC0/WO[1]  _           _           _        
47:PB02  EINT[2]   AIN[10]        _          SERCOM5:0  TC6/WO[0]   _           _           _        
20:PB11  EINT[11]  _              _          SERCOM4:3  TC5/WO[1]   TCC0/WO[5]  I2S/SCK[1]  GCLK[5]  
19:PB10  EINT[10]  _              _          SERCOM4:2  TC5/WO[0]   TCC0/WO[4]  I2S/MCK[1]  GCLK[4]  
21:PA12  EINT[21]  _              SERCOM2:0  SERCOM4:0  TCC2/WO[0]  TCC0/WO[6]  _           AC/CMP[0]
16:PA11  EINT[11]  AIN[19]        SERCOM0:3  SERCOM2:3  TCC1/WO[1]  TCC0/WO[3]  I2S/FS[0]   GCLK[5]  
15:PA10  EINT[10]  AIN[18]        SERCOM0:2  SERCOM2:2  TCC1/WO[0]  TCC0/WO[2]  I2S/SCK[0]  GCLK[4]  
GND      _         _              _          _          _           _           _           _        
                                                                                                     
VBAT     _         _              _          _          _           _           _           _        
En       _         _              _          _          _           _           _           _        
VBUS     _         _              _          _          _           _           _           _        
26:PA17  EINT[1]   _              SERCOM1:1  SERCOM3:1  TCC2/WO[1]  TCC0/WO[7]  _           GCLK[3]  
28:PA19  EINT[3]   _              SERCOM1:3  SERCOM3:3  TCC3/WO[1]  TCC0/WO[3]  I2S/SD[0]   AC/CMP[1]
25:PA16  EINT[0]   _              SERCOM1:0  SERCOM3:0  TCC2/WO[0]  TCC0/WO[6]  _           GCLK[2]  
27:PA18  EINT[2]   _              SERCOM1:2  SERCOM3:2  TCC3/WO[0]  TCC0/WO[2]  _           AC/CMP[0]
12:PA07  EINT[7]   AIN[7],AIN[3]  _          SERCOM0:3  TCC1/WO[1]  _           I2S/SD[0]   _        
29:PA20  EINT[4]   _              SERCOM5:2  SERCOM3:2  TC7/WO[0]   TCC0/WO[6]  I2S/SCK[0]  GCLK[4]  
24:PA15  EINT[15]  _              SERCOM2:3  SERCOM4:3  TC3/WO[1]   TCC0/WO[5]  _           GCLK[1]  
32:PA23  EINT[7]   _              SERCOM3:1  SERCOM5:1  TC4/WO[1]   TCC0/WO[5]  USB/SOF     GCLK[7]  
31:PA22  EINT[6]   _              SERCOM3:0  SERCOM5:0  TC4/WO[0]   TCC0/WO[4]  _           GCLK[6]  
                                                                                                     
13:PA08
30:PA21
11:PA06


*/

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

