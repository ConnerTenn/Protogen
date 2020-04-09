
#include "interfaces.h"
#include <avr/pgmspace.h>

extern PROGMEM const char *const _binary_ButtonData_bin_start;
extern PROGMEM const char *const _binary_ButtonData_bin_end;
#define BIN_BUTTONDATA_START ((const PROGMEM u8 *const)(&_binary_ButtonData_bin_start))

#define BUTTONDATA_ACC_8(off) ( (u8)(pgm_read_byte_near(BIN_BUTTONDATA_START+(off))) )
#define BUTTONDATA_ACC_16(off) ( (u16)(pgm_read_word_near(BIN_BUTTONDATA_START+(off))) )


/*

u8 Timeout
// u8 NumSequences
struct Sequences
{
	u8 Momentary
	u8 NumCombos
	struct Combos
	{
		u8 NumButtons
		struct Buttons
		{
			u8 ButtonNum
		}
	}[NumCombos]
	u8 CommandLen
	u8 Command[CommandLen]
}[NumSequences]

*/

typedef struct
{
	u8 ButtonNum;
} Button;

typedef struct
{
	u8 Active;
	u8 NumButtons;
	Button Buttons[0];
} Combo;

typedef struct
{
	u8 Momentary;
	u8 NumCombos;
	Combo Combos[0]; 
} Sequence;

extern u8 Timeout;
// const u8 NumSequences = NUM_SEQUENCES;
extern Sequence Sequences[NUM_SEQUENCES];

void InitButtons();
u8 ReadButton(u8 id);

