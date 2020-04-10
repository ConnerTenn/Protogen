
#include "interfaces.h"
#include <avr/pgmspace.h>

extern PROGMEM const char *const _binary_ButtonData_bin_start;
extern PROGMEM const char *const _binary_ButtonData_bin_end;
#define BIN_BUTTONDATA_START ((const PROGMEM u8 *const)(&_binary_ButtonData_bin_start))

#define BUTTONDATA_ACC_8(off) ( (u8)(pgm_read_byte_near(BIN_BUTTONDATA_START+(off))) )
#define BUTTONDATA_ACC_16(off) ( (u16)(pgm_read_word_near(BIN_BUTTONDATA_START+(off))) )


#define PRINT_VAL(txt, val) SerialTransmitStr(txt); SerialTransmitHexVal(val); SerialTransmitStr("\n"); SerialFlush();
	
/*
Binart Data Segment:

u8 Timeout
u8 CommandData[COMMAND_DATA_LEN]
u8 Buttons[NUM_BUTTONS]
struct Combos
{
	u8 NumButtons
	u8 ButtonIdx[NumButtons]
}[NUM_COMBOS]
struct Sequences
{
	u8 Momentary
	u8 CommandLen
	u16 CommandOffset
	u8 NumCombos
	u8 ComboIdx[NumCombos]
}[NUM_SEQUENCES]

*/

typedef struct
{
	u8 Active;
	u8 ButtonID;
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
	Combo *Combos[0];
} Sequence;

extern u8 ButtonStructData[BUTTON_STRUCT_SIZE];
extern u8 Timeout;
extern u8 CommandData[COMMAND_DATA_LEN];
extern Button Buttons[NUM_BUTTONS];
extern Combo *Combos[NUM_COMBOS];
extern Sequence *Sequences[NUM_SEQUENCES];

#ifdef DEBUG
extern u8 *LastCmd;
extern u8 LastCmdLen;
#endif

void InitButtons();
void UpdateButtons();
u8 ReadButton(u8 id);

