
#include "vfd.h"

void InitDisplay()
{
    ENBITS(DDRB, (1<<CD) | (1<<DDRB_CS));
	ENBITS(PORTB, (1<<CD) | (1<<DDRB_CS));
	//DELAY_1_MS;
	//DELAY_1_US;
	DISPLAY_CLEAR_CMD(1, 1, 1);
	//DELAY_1_US;
	DISPLAY_AREA_SET_ALL_GRAPHIC_CMD;
}

