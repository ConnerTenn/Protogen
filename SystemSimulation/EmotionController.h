

#include "Global.h"
#include "Devices.h"
#include "Messaging.h"
#include "Frames.h"


struct ExpressionState
{
	u16 LeftEye;
	u16 RightEye;
	u16 LeftMouth;
	u16 RightMouth;
	u16 CenterMouth;
	u16 Nose;
	u16 Body;
};
extern ExpressionState ExprState;

void PrintDisplay(const u8 *display, u8 width, u8 height, int dx, int dy);
