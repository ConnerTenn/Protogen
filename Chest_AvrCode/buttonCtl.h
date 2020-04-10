
#include "interfaces.h"
#include <avr/pgmspace.h>

extern PROGMEM const char *const _binary_ButtonData_bin_start;
extern PROGMEM const char *const _binary_ButtonData_bin_end;
#define BIN_BUTTONDATA_START ((const PROGMEM u8 *const)(&_binary_ButtonData_bin_start))

#define BUTTONDATA_ACC_8(off) ( (u8)(pgm_read_byte_near(BIN_BUTTONDATA_START+(off))) )
#define BUTTONDATA_ACC_16(off) ( (u16)(pgm_read_word_near(BIN_BUTTONDATA_START+(off))) )


/*
Binart Data Segment:

u8 Timeout
u8 CommandData[COMMAND_DATA_LEN]
u8 Buttons[NUM_BUTTONS]
struct Sequences
{
	u8 Momentary
	u8 CommandLen
	u16 CommandOffset
	u8 NumCombos
	struct Combos
	{
		u8 NumButtons
		u8 ButtonIdx
	}[NumCombos]
}[NUM_SEQUENCES]

*/

typedef struct
{
	u8 ButtonNum;
	u8 Active;
} Button;

typedef struct
{
	u8 Active;
	u8 NumButtons;
	Button *Buttons[0];
} Combo;

typedef struct
{
	u8 ActiveCombo;
	u8 Momentary;
	u8 CommandLen;
	u8 *Command;
	u8 NumCombos;
	Combo Combos[0];
} Sequence;

extern u8 Timeout;
extern u8 CommandData[COMMAND_DATA_LEN];
extern Button Buttons[NUM_BUTTONS];
extern Sequence Sequences[NUM_SEQUENCES];

void InitButtons();
void UpdateButtons();
u8 ReadButton(u8 id);
void ReadButtons();

