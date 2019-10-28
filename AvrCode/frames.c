
#include "frames.h"

const u8 *FrameData = (const u8 *)(&_binary_FrameData_bin_start);
const u8 *FrameDataEnd = (const u8 *)(&_binary_FrameData_bin_end);


#define CEILDIV(n,d) ( ((int)(n)) / ((int)(d)) + ( ((int)(n))%((int)(d))>0 ? 1 : 0 ) )

#define FrameCPY(fd, fs, w) (memcpy(fd,fs,w*8)) //fd <- fs

void FrameTransUp(u8 *data, u8 numDisplays, u8 num)
{
	for (int y=8; y>num; y--)
	{
		for (int x=0; x<width; x++)
		{
			data[y*numDisplays+x]=data[(y-num)*numDisplays+x]
		}
	}
}

void FrameTransDown(u8 *data, u8 numDisplays, u8 num)
{
	for (int y=0; y<8-num; y--)
	{
		for (int x=0; x<width; x++)
		{
			data[y*numDisplays+x]=data[(y+num)*numDisplays+x]
		}
	}
}

void FrameTransLeft(u8 *data, u8 numDisplays, u8 num)
{
	for (int y=0; y<8-num; y--)
	{
		u8 segover=CEILDIV(num,8);
		u8 bitover=num%8;
		u8 mask = 0xFF<<segover;
		for (int x=0; x<width; x++)
		{
			data[y*numDisplays+x]=
				( (data[y*numDisplays+x]<<bitover) & (0xFF<<bitover) ) | 
				( (data[y*numDisplays+x+segover]) & ((~(0xFF>>bitover))>>(7-bitover)) );
		}
	}
}


