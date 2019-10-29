
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


/*

def FrameTransLeft(data, numDisplays, num):
	segover = CEILDIV(num,8)
	segov = int(int(num)/int(8)+1)
	bitover = num%8
	for y in range(0, 8): #(int y=0; y<8-num; y--)
		for x in range(0,numDisplays): #(int x=0; x<width; x++)
			if x+segov-1<numDisplays:
				data[y*numDisplays+x] = ( (data[y*numDisplays+x+segov-1]<<bitover) & (0xFF<<bitover) )
			else:
				data[y*numDisplays+x] = 0
			if x+segover<numDisplays:
				data[y*numDisplays+x] |= ( ((data[y*numDisplays+x+segover]) & (~(0xFF>>bitover))) >> (8-bitover) )
			data[y*numDisplays+x] = data[y*numDisplays+x]&0xFF

def FrameTransRight(data, numDisplays, num):
	segover = CEILDIV(num,8)
	segov = int(int(num)/int(8)+1)
	bitover = num%8
	for y in range(0, 8): #(int y=0; y<8-num; y--)
		for x in range(numDisplays-1,-1,-1): #(int x=0; x<width; x++)
			if x-segov+1>=0:
				data[y*numDisplays+x] = ( (data[y*numDisplays+x-segov+1]>>bitover) & (0xFF>>bitover) )
			else:
				data[y*numDisplays+x] = 0
			if x-segover>=0:
				data[y*numDisplays+x] |= ( ((data[y*numDisplays+x-segover]) & (~(0xFF<<bitover))) << (8-bitover) )
			data[y*numDisplays+x] = data[y*numDisplays+x]&0xFF

*/


