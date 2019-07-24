
#include "Global.h"
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

// struct Event
// {
// 	enum 
// 	{
// 		ButtonPress,
// 		Click,
// 		Move,
// 	} Action;
	
// 	Ksequ Key;
// 	void *Data;
// };

// struct UIElem
// {
// 	int X, Y;
// 	void (* Draw)(int x, int y);
// 	void (* Focus)();
// 	void (* Unfocus)();
// 	void (* Event)(Event event);
// };

// extern std::vector<UIElem> UIElementList;
extern u32 FocusedPane;


void InitUI();
void DestroyUI();
void UpdateUI();

void UIHandleInput(Ksequ key, MessageHandler *messenger);

void TermLogWrite(std::string msg);



