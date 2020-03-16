
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
379: PORT Register Summary

432: Serial Communication Interface (SERCOM)
454: SERCOM Register Summary

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

	//DDR
	//MOSI
	PORT->Group[1].DIRSET.reg = 1<<10; //DDRB10:Out 
	//MISO
	PORT->Group[0].DIRCLR.reg = 1<<11; //DDRA12:IN
	//SCK
	PORT->Group[1].DIRSET.reg = 1<<11; //DDRB11:Out


	//PINMUX
	// SCK (ODD) Peripheral D (SERCOM4 P3), MOSI (EVEN) Peripheral D (SERCOM4 P2)
	PORT->Group[1].PMUX[10/2].reg = PORT_PMUX_PMUXE(PORT_PMUX_PMUXE_D_Val) | PORT_PMUX_PMUXO(PORT_PMUX_PMUXO_D);
	// MISO (EVEN) Peripheral D (SERCOM4 P0)
	PORT->Group[0].PMUX[12/2].reg = PORT_PMUX_PMUXE(PORT_PMUX_PMUXE_D_Val);

	//PINCFG
	PORT->Group[1].PINCFG[10].bit.PMUXEN = 1; //MOSI
	PORT->Group[0].PINCFG[12].bit.PMUXEN = 1; //MISO
	PORT->Group[1].PINCFG[11].bit.PMUXEN = 1; //SCK
	

	u8 i=0; (void)i;
	while (1)
	{
		PORT->Group[0].OUTTGL.reg = 1<<17; //DRA17: Toggle
		// REG(volatile u32, 0x41004400+0x1C) = 1<<17; //DRA17 TOGGLE
	}

	return 0;
}

