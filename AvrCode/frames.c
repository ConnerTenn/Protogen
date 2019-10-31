
#include "frames.h"

const u8 *FrameData = (const u8 *)(&_binary_FrameData_bin_start);
const u8 *FrameDataEnd = (const u8 *)(&_binary_FrameData_bin_end);


#define FrameCPY(fd, fs, w) (memcpy(fd,fs,w*8)) //fd <- fs

void FrameTransUp(u8 *data, u8 numDisplays, u8 num)
{
	for (int y=0; y<8; y++)
	{
		for (int x=0; x<numDisplays; x++)
		{
			data[y*numDisplays+x] = (y<8-num ? data[(y+num)*numDisplays+x] : 0);
		}
	}
}

void FrameTransDown(u8 *data, u8 numDisplays, u8 num)
{
	for (int y=7; y>=0; y--)
	{
		for (int x=0; x<numDisplays; x++)
		{
			data[y*numDisplays+x] = (y>=num ? data[(y-num)*numDisplays+x] : 0);
		}
	}
}
	
void FrameTransLeft(u8 *data, u8 numDisplays, u8 num)
{
	int segover = CEILDIV(num,8);
	int segov = (int)(((int)num)/((int)8)+1);
	int bitover = num%8;
	for (int y=0; y<8; y++)
	{
		for (int x=0; x<numDisplays; x++)
		{
			if (x+segov-1<numDisplays)
			{
				data[y*numDisplays+x] = ( (data[y*numDisplays+x+segov-1]<<bitover) & (0xFF<<bitover) );
			}
			else
			{
				data[y*numDisplays+x] = 0;
			}
			if (x+segover<numDisplays)
			{
				data[y*numDisplays+x] |= ( ((data[y*numDisplays+x+segover]) & (~(0xFF>>bitover))) >> (8-bitover) );
			}
		}
	}
}

void FrameTransRight(u8 *data, u8 numDisplays, u8 num)
{
	int segover = CEILDIV(num,8);
	int segov = (int)(((int)num)/((int)8)+1);
	int bitover = num%8;
	for (int y=0; y<8; y++)
	{
		for (int x=numDisplays-1; x>=0; x--)
		{
			if (x-segov+1>=0)
			{
				data[y*numDisplays+x] = ( (data[y*numDisplays+x-segov+1]>>bitover) & (0xFF>>bitover) );
			}
			else
			{
				data[y*numDisplays+x] = 0;
			}
			if (x-segover>=0)
			{
				data[y*numDisplays+x] |= ( ((data[y*numDisplays+x-segover]) & (~(0xFF<<bitover))) << (8-bitover) );
			}
		}
	}
}

