
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

117: Power manager
127: Power manager Register Summary

379: PORT section
379: PORT Register Summary

432: Serial Communication Interface (SERCOM)

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



	ENBIT(volatile u32, 0x41004400+0x00, 17); //DDRA17:Out 
	DABIT(volatile u32, 0x41004400+0x10, 17); //DRA17 OFF
	ENBIT(volatile u32, 0x41004400+0x10, 17); //DRA17 ON
	DABIT(volatile u32, 0x41004400+0x10, 17); //DRA17 OFF

	//DDR
	//MOSI
	ENBIT(volatile u32, 0x41004400+0x80*1+0x00, 10); //DDRB10:Out 
	//MISO
	DABIT(volatile u32, 0x41004400+0x80*0+0x00, 12); //DDRA12:IN
	//SCK
	ENBIT(volatile u32, 0x41004400+0x80*1+0x00, 11); //DDRB11:Out 

	//PINMUX
	// //SCK (ODD) Peripheral D (SERCOM4 P3), MOSI (EVEN) Peripheral D (SERCOM4 P2)
	// SETREG(volatile u8, 0x41004400+0x80*1+0x30+(10/2), 0x33, 0xFF);
	// //MISO (EVEN) Peripheral D (SERCOM4 P0)
	// SETREG(volatile u8, 0x41004400+0x80*0+0x30+(12/2), 0x03, 0x0F);

	// //PINCFG
	// //MOSI Strong Drive, Pin Mux Enable
	// SETREG(volatile u32, 0x41004400+0x80*1+0x40+10, 0x41, 0xFF); //DDRB10:Out 
	// //MISO Strong Drive, Pin Mux Enable
	// SETREG(volatile u32, 0x41004400+0x80*0+0x40+12, 0x41, 0xFF); //DDRA12:IN
	// //SCK Strong Drive, Pin Mux Enable
	// SETREG(volatile u32, 0x41004400+0x80*1+0x40+11, 0x41, 0xFF); //DDRB11:Out 



	u8 i=0; (void)i;
	while (1)
	{
		ENBIT(volatile u32, 0x41004400+0x10, 17); //DRA17 ON
		DABIT(volatile u32, 0x41004400+0x10, 17); //DRA17 OFF
		// REG(volatile u32, 0x41004400+0x1C) = 1<<17; //DRA17 TOGGLE
	}

	return 0;
}

