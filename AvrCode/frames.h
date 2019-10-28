
#include "globals.h"

extern const char _binary_FrameData_bin_start;
extern const char _binary_FrameData_bin_end;
extern const u8 *FrameData;
extern const u8 *FrameDataEnd;

#define FrameDataAcc(i) ((u8 *)(FrameData + *(u32 *)(FrameData+i*8)))

