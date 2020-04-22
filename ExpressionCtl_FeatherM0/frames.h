

#ifndef _FRAMES_H_
#define _FRAMES_H_

#include "globals.h"

extern const u8 _binary_FrameData_bin_start;
extern const u8 _binary_FrameData_bin_end;
#define FRAME_DATA ( (u8 *)&_binary_FrameData_bin_start )
#define FRAME_LEN ( (u32)((&_binary_FrameData_bin_end)-(&_binary_FrameData_bin_start)) )

typedef struct 
{
	u16 FrameOffset;
	u16 FrameNext;
	u16 FrameDelay;
} FrameDat;

#define FRAME_HEADER_ACC(i) ((FrameDat *)(FRAME_DATA+(i)*sizeof(FrameDat)))
#define FRAME_DATA_ACC(i) ((u8 *)(FRAME_DATA + (u32)(FRAME_HEADER_ACC(i)->FrameOffset)))


extern const u8 _binary_DisplayData_bin_start;
extern const u8 _binary_DisplayData_bin_end;
#define DISPLAY_DATA ( (u8 *)&_binary_DisplayData_bin_start )

typedef struct 
{
	u16 FrameIndex;
	u16 EndIndex;
	u8 NumSegments;
	u8 _padding;
} DisplayDat;

#define DISPLAY_HEADER_ACC(i) ((DisplayDat *)(DISPLAY_DATA+(i)*sizeof(DisplayDat)))


// void FrameTransUp(u8 *data, u8 numSegments, u8 num);
// void FrameTransDown(u8 *data, u8 numSegments, u8 num);
// void FrameTransLeft(u8 *data, u8 numSegments, u8 num);
// void FrameTransRight(u8 *data, u8 numSegments, u8 num);

#endif
