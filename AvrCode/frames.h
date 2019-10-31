
#include "globals.h"

extern const char _binary_FrameData_bin_start;
extern const char _binary_FrameData_bin_end;
extern const u8 *FrameData;
extern const u8 *FrameDataEnd;

typedef struct 
{
	u32 FrameOffset;
	u16 FrameNext;
	u16 FrameDelay;
} Frame;

#define FrameHeaderAcc(i) ((Frame *)(FrameData+i*8))
#define FrameDataAcc(i) ((u8 *)(FrameData + *(u32 *)(FrameData+i*8)))

void FrameTransUp(u8 *data, u8 numDisplays, u8 num);
void FrameTransDown(u8 *data, u8 numDisplays, u8 num);
void FrameTransLeft(u8 *data, u8 numDisplays, u8 num);
void FrameTransRight(u8 *data, u8 numDisplays, u8 num);
