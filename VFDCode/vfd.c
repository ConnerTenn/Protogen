
#include "vfd.h"

void InitDisplay()
{
    ENBITS(DDRB,(1<<CD));

	DISPLAY_CLEAR_CMD(1, 1, 1);
	DISPLAY_AREA_SET_ALL_GRAPHIC_CMD;
}

