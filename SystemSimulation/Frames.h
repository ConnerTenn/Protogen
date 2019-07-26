
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
-2-2 -1-2 +0-2 +1-2 +2-2
-2-1 -1-1 +0-1 +1-1 +2-1
-2+0 -1+0 +0+0 +1+0 +2+0
-2+1 -1+1 +0+1 +1+1 +2+1
-2+2 -1+2 +0+2 +1+2 +2+2

x+y
*/

typedef struct Frame_t
{
	u8 *Data;
	u8 DelayMin, DelayMax;
	u8 Next;
} Frame;

#include "FrameData.h"

#endif



