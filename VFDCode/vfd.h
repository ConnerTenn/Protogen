
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


#define CD 1

/*
l0:     layer 0 active/inactive
l1:     layer 0 active/inactive
gs:     graphic display area on/off
grv:    reverse/normal graphic mode
and:    combine layer 1 & 2 with AND operator
xor:    combine layer 1 & 2 with XOR operator
*/
#define DISP_ON_OFF_CMD(l0, l1, gs, grv, and, xor) (\
    ENBITS(DDRC, CD), \
    SPI_TRANSMIT((0b0010<<4) | ((l1)<<3) | ((l0)<<2)), \
    SPI_TRANSMIT(((gs)<<6) | ((grv)<<4) | ((and)<<3) | ((xor)<<2)), \
    DABITS(DDRC, CD) \
    )

/*
bri: Brightness [0x0->0xF] (100% bright -> 6% dark)
*/
#define SET_BRIGHTNESS_CMD(bri) (\
    ENBITS(DDRC, CD), \
    SPI_TRANSMIT((0b0100<<4) | ((bri) & 0xF)), \
    DABITS(DDRC, CD) \
    )

/*
g1c: graphics area 1 cleared / not cleared
g0c: graphics area 0 cleared / not cleared
hm: reset data write position address and start position address
will delay for delay 1ms
*/
#define DISPLAY_CLEAR_CMD(g1c, g0c, hm) (\
    ENBITS(DDRC, CD), \
    SPI_TRANSMIT((0b0101<<4) | ((g1c)<<3) | ((g0c)<<2) | (0b10) | (hm)), \
    /*Delay for 1ms  http://www.bretmulvey.com/avrdelay.html*/ \
    ({ asm volatile ( \
        "    ldi  r18, 21"	"\n" \
        "    ldi  r19, 199"	"\n" \
        "1:  dec  r19"	"\n" \
        "    brne 1b"	"\n" \
        "    dec  r18"	"\n" \
        "    brne 1b"	"\n" \
        ); }), \
    DABITS(DDRC, CD) \
    )

/*
addr: [0->7] display section to set to graphic mode
Only graphic mode is available so all sections must be set to graphic 
Use DISPLAY_AREA_SET_ALL_GRAPHIC_CMD macro
*/
#define  DISPLAY_AREA_SET_GRAPHIC_CMD(addr) ( \
    ENBITS(DDRC, CD), \
    SPI_TRANSMIT(0b01100010), \
    SPI_TRANSMIT((0b01100<<3) | (addr)), \
    DABITS(DDRC, CD), \
    SPI_TRANSMIT(0b11111111) \
    )

#define  DISPLAY_AREA_SET_ALL_GRAPHIC_CMD ({ \
    for (u8 i=0; i<=7; i++) \
    { \
        DISPLAY_AREA_SET_GRAPHIC_CMD(i); \
    } \
    })

/*
addr: [0->0b1111111] [0->127] data write position X
*/
#define SET_WRITE_POS_X_ADDR_CMD(addr) (\
    ENBITS(DDRC, CD), \
    SPI_TRANSMIT(0b01100100), \
    SPI_TRANSMIT((addr)), \
    DABITS(DDRC, CD) \
    )

/*
addr: [0->0b1111] [0->15] data write position Y
*/
#define SET_WRITE_POS_Y_ADDR_CMD(addr) (\
    ENBITS(DDRC, CD), \
    SPI_TRANSMIT(0b01100000), \
    SPI_TRANSMIT((addr)), \
    DABITS(DDRC, CD) \
    )




void foo()
{
    DISPLAY_CLEAR_CMD(0,0,0);
    DISPLAY_AREA_SET_ALL_GRAPHIC_CMD;
}

