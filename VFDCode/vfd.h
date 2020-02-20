
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

/*Delay   http://www.bretmulvey.com/avrdelay.html*/
#define DELAY_1_MS ({ \
    asm volatile ( \
        "    ldi  r18, 21"	"\n" \
        "    ldi  r19, 199"	"\n" \
        "1:  dec  r19"	"\n" \
        "    brne 1b"	"\n" \
        "    dec  r18"	"\n" \
        "    brne 1b"	"\n" \
    ); \
    })
    
#define DELAY_1_US ({ \
    asm volatile ( \
        "    ldi  r18, 5"	"\n" \
        "1:  dec  r18"	"\n" \
        "    brne 1b"	"\n" \
        "    nop"	"\n" \
    ); \
    })

/*
l0:     layer 0  1:active  0:inactive
l1:     layer 1  1:active  0:inactive
gs:     graphic display area  1:on  0:off
grv:    1:reverse  0:normal graphic mode
and:    combine layer 1 & 2 with AND operator
xor:    combine layer 1 & 2 with XOR operator
*/
#define DISP_ON_OFF_CMD(l0, l1, gs, grv, and, xor) ( \
    DABITS(PORTB, (1<<DDRB_CS)), \
    ENBITS(PORTB, (1<<CD)), \
    SPI_TRANSMIT((0b0010<<4) | ((l1)<<3) | ((l0)<<2)), \
    SPI_TRANSMIT(((gs)<<6) | ((grv)<<4) | ((and)<<3) | ((xor)<<2)), \
    DABITS(PORTB, (1<<CD)), \
    ENBITS(PORTB, (1<<DDRB_CS)) \
    )

/*
bri: Brightness [0x0->0xF] (100% bright -> 6% dark)
*/
#define SET_BRIGHTNESS_CMD(bri) ( \
    DABITS(PORTB, (1<<DDRB_CS)), \
    ENBITS(PORTB, (1<<CD)), \
    SPI_TRANSMIT((0b0100<<4) | ((bri) & 0xF)), \
    DABITS(PORTB, (1<<CD)), \
    ENBITS(PORTB, (1<<DDRB_CS)) \
    )

/*
g1c: graphics area 1  1:cleared  0:not cleared
g0c: graphics area 0  1:cleared  0:not cleared
hm: reset data write position address and start position address
will delay for delay 1ms
*/
#define DISPLAY_CLEAR_CMD(g1c, g0c, hm) ( \
    DABITS(PORTB, (1<<DDRB_CS)), \
    ENBITS(PORTB, (1<<CD)), \
    SPI_TRANSMIT((0b0101<<4) | ((g1c)<<3) | ((g0c)<<2) | (0b10) | (hm)), \
    DABITS(PORTB, (1<<CD)), \
    ENBITS(PORTB, (1<<DDRB_CS)), \
    DELAY_1_MS, \
    DELAY_1_MS \
    )

/*
addr: [0->7] display section to set to graphic mode
Only graphic mode is available so all sections must be set to graphic 
Use DISPLAY_AREA_SET_ALL_GRAPHIC_CMD macro
*/
#define  DISPLAY_AREA_SET_GRAPHIC_CMD(addr) ( \
    DABITS(PORTB, (1<<DDRB_CS)), \
    ENBITS(PORTB, (1<<CD)), \
    SPI_TRANSMIT(0b01100010), \
    SPI_TRANSMIT((addr)), \
    DABITS(PORTB, (1<<CD)), \
    SPI_TRANSMIT(0b11111111), \
    ENBITS(PORTB, (1<<DDRB_CS)) \
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
#define SET_WRITE_POS_X_ADDR_CMD(addr) ( \
    DABITS(PORTB, (1<<DDRB_CS)), \
    ENBITS(PORTB, (1<<CD)), \
    SPI_TRANSMIT(0b01100100), \
    SPI_TRANSMIT((addr)), \
    DABITS(PORTB, (1<<CD)), \
    ENBITS(PORTB, (1<<DDRB_CS)) \
    )

/*
addr: [0->0b1111] [0->15] data write position Y
*/
#define SET_WRITE_POS_Y_ADDR_CMD(addr) ( \
    DABITS(PORTB, (1<<DDRB_CS)), \
    ENBITS(PORTB, (1<<CD)), \
    SPI_TRANSMIT(0b01100000), \
    SPI_TRANSMIT((addr)), \
    DABITS(PORTB, (1<<CD)), \
    ENBITS(PORTB, (1<<DDRB_CS)) \
    )


/*
addr: [0->0xFF] display horizontal start postion (horizontal shift)
*/
#define SET_START_X_POSITION_ADDR_CMD(addr) ( \
    DABITS(PORTB, (1<<DDRB_CS)), \
    ENBITS(PORTB, (1<<CD)), \
    SPI_TRANSMIT(0b01110000), \
    SPI_TRANSMIT((addr)), \
    DABITS(PORTB, (1<<CD)), \
    ENBITS(PORTB, (1<<DDRB_CS)) \
    )

/*
display vertical start postion (vertical shift)
ud: [0->1]  1:Upwards  0:Downwards
sft:  0b01:Shift by 8 dots  0b10:Shift by 1 dot  0b11:Shift by 2 dots
*/
#define SET_START_Y_POSITION_ADDR_CMD(ud, sft) ( \
    DABITS(PORTB, (1<<DDRB_CS)), \
    ENBITS(PORTB, (1<<CD)), \
    SPI_TRANSMIT((0b10110000) | ((ud)<<3) | ((sft)<<1)), \
    DABITS(PORTB, (1<<CD)), \
    ENBITS(PORTB, (1<<DDRB_CS)) \
    )


/*
igx: [0->1]  1:X-Addr auto increment  0:Fixed X Addr mode
igy: [0->1]  1:Y-Addr auto increment  0:Fixed Y Addr mode
*/
#define SET_ADDR_MODE_CMD(igx, igy) ( \
    DABITS(PORTB, (1<<DDRB_CS)), \
    ENBITS(PORTB, (1<<CD)), \
    SPI_TRANSMIT((0b1000<<4) | ((igx)<<2) | ((igy)<<1)), \
    DABITS(PORTB, (1<<CD)), \
    ENBITS(PORTB, (1<<DDRB_CS)) \
    )


/*
STUB
*/
#define GRAM_READ(haddr, vaddr) ( \
    DABITS(PORTB, (1<<DDRB_CS)), \
    ENBITS(PORTB, (1<<CD)), \
    SPI_TRANSMIT(0b11010100), \
    SPI_TRANSMIT(vaddr), \
    SPI_TRANSMIT(haddr), \
    DELAY_1_MS, \
    DABITS(PORTB, (1<<CD)), \
    ENBITS(PORTB, (1<<DDRB_CS)) \
    )

/*
Bits arranged vertically
 D0
 |
 V
 D7
*/
#define GRAM_WRITE(data) ( \
    DABITS(PORTB, (1<<DDRB_CS)), \
    SPI_TRANSMIT(data), \
    ENBITS(PORTB, (1<<DDRB_CS)) \
    )


void InitDisplay();

