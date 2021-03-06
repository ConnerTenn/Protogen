
#ifndef _FRAMES_H_
#define _FRAMES_H_

#include "Global.h"

// Eyes: 16 x 8
// Mouth: 32 x 8
// Mouth Center: 8 x 8
// Nose: 8 x 8

// Encoding
/*
Bytes record horizontal pixel data in rows. Each bit represents 1 pixel
*/

/*
Eye sequence
+0+0
-2-2 -1-2 +0-2 +1-2 +2-2
-2-1 -1-1 +0-1 +1-1 +2-1
-2+0 -1+0      +1+0 +2+0
-2+1 -1+1 +0+1 +1+1 +2+1
-2+2 -1+2 +0+2 +1+2 +2+2

x+y
*/

/*
FrameData.bin Format
Header:
{
	u8 Next
	u8 Delay
	u8 DelayCounter
	u64 DataOffset
}[]
Data:
u8 FrameData[]

*/

typedef struct Frame_t
{
	// u8 Type;
	u16 Next;
	u8 Delay;
	u8 DelayCounter;
	//u8 _padding[5];
	u64 DataOffset;
} Frame;


extern const char _binary_FrameData_bin_start;
extern const char _binary_FrameData_bin_end;
const u8 *FrameData = (const u8 *)(&_binary_FrameData_bin_start);
const u8 *FrameDataEnd = (const u8 *)(&_binary_FrameData_bin_end);

//#include "FrameData.h"

#endif



