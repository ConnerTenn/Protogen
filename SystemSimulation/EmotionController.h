

#include "Global.h"
#include "Devices.h"
#include "Messaging.h"
#include "DisplayFrames.h"


// struct Display
// {
// 	u8 *Array;
// 	u8 Width;
// 	u8 Height;
// };

// void InitDisplay(Display *display, u8 width, u8 height)
// {
// 	display->Width=width;
// 	display->Height=height;

// 	display->Array=(u8 *)malloc((width)*height);
// }

// void DestroyDisplay(Display *display)
// {
// 	free(display->Array);
// }

// inline void PutDisp(Display *display, u8 x, u8 y, u8 val)
// {
// 	val=(val?1:0);
// 	u16 idx = y*(display->Width) + (x);
// 	//u8 bit = (x%2);
// 	//u8 mask = 1<<bit;
// 	//display->Array[idx] = (display->Array[idx] & ~mask) | (val<<bit);
// 	display->Array[idx] = val;
// }
// inline u8 GetDisp(Display *display, u8 x, u8 y)
// {
// 	u16 idx = y*(display->Width) + (x);
// 	//u8 bit = 1-(x%2);
// 	//u8 mask = 1<<bit;
// 	//return (display->Array[idx] & mask)>>(bit);
// 	return display->Array[idx];
// }

// /*wchar_t PixelLut[] =
// {
// 	L' ', L'▝', L'▘', L'▀',
// 	L'▗', L'▐', L'▚', L'▜',
// 	L'▖', L'▞', L'▌', L'▛',
// 	L'▄', L'▟', L'▙', L'█'
// };*/
// char PixelLut[][2] =
// {
// };
// void PrintDisplay(Display *display, int dx, int dy)
// {
// 	for (u8 y=0; y<display->Height; y++)
// 	{
// 		MOVETO(dx, dy+y);
// 		for (u8 x=0; x<display->Width; x++)
// 		{
// 			u8 pixel = GetDisp(display, x, y);
// 			PRINT("%s  ",(pixel?BWHITE:BBLACK));
// 		}	
// 	}
// 	PRINT("\n");
// }

struct ExpressionState
{
	u16 LeftEye;
	u16 RightEye;
	u16 LeftMouth;
};
extern ExpressionState ExprState;

void PrintDisplay(u8 *display, u8 width, u8 height, int dx, int dy);

/*

Eye movement 
	x: -4 -> 4
	y: -4 -> 4

Expression
	Eye states
	Mouth States
	Nose States

Messages
	EyePos <x> <y>
	ExpressionSet <name>
	ExpressionVariant <number>

*/
