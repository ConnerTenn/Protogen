
extern "C"
{
#include "Global.h"
#include "TextScreen.h"
}
#include "Messaging.h"
#include "Expressions.h"


#define K_UP    0x00415B1B
#define K_DOWN  0x00425B1B
#define K_LEFT  0x00445B1B
#define K_RIGHT 0x00435B1B
#define K_ENTER 0x0000000A

union Ksequ
{
	u8 seq[8];
	u64 val;
};


extern u32 FocusedPane;

void InitUI();
void DestroyUI();
void UpdateUI();

void UIHandleInput(Ksequ key, MessageHandler *messenger);

void TermLogWrite(char *msg);



