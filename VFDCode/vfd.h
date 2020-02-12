
#include "globals.h"
#include "interfaces.h"

/*
Connector Pinout

      1  2 Gnd
      3  4 Gnd
      5  6 Gnd
      7  8 Gnd
      9 10 Gnd
     11 12 Gnd
TX   13 14 Gnd
RX   15 16 Gnd
     17 18 Gnd
!C/D 19 20 Gnd
SCK  21 22 Gnd
!CS  21 22 Gnd
FRP  25 26 !Reset

1 VCC
2 
3 Gnd

!C/D: Command/Data
FRP: Frame pulse signal (output)

*/

u8 DisplayBuffer[0xF][0x7F];

void foo()
{
	DisplayBuffer[0];
}

#define CD 1

#define DISP_ON_OFF_CMD(l0, l1, gs, grv, and, xor) (\
	ENBITS(DDRC, CD), \
	SPI_TRANSMIT((0b0010<<4) | (l1<<3) | (l0<<2)), \
	SPI_TRANSMIT((gs<<6) | (grv<<4) | (and<<3) | (xor<<2)), \
	DABITS(DDRC, CD) \
	) 


//#define CMD_DISP_ON_OFF(l0, l1

