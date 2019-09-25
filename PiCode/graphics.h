
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "globals.h"


#define FBWIDTH 800
#define FBHEIGHT 480

#define FB_SIZE (FBHEIGHT*FBWIDTH*4)

#define FBACC(buff,x,y) (ARRACC((buff), (x), (y), FBWIDTH, 1, u32 *))

typedef struct
{
	u8 R;
	u8 G;
	u8 B;
	u8 A;
} Pixel; 

typedef struct
{
	u32 *Buff;
	u32 Width;
	u32 Height;
} FrameBuffer;

void InitDisplay(u32 **fb0);
void CloseDisplay(u32 *fb0);

FrameBuffer CreateFrameBuffer(u16 width, u16 height);
void DestroyFrameBuffer(FrameBuffer fb);

Pixel WordToPixel(u32 pix);
u32 PixelToWord(Pixel pix);

void SetPixel(u32 *buff, int w, int x, int y, Pixel colour);


void DrawLine(FrameBuffer fb, int x1, int y1, int x2, int y2, Pixel colour);
void DrawRect(FrameBuffer fb, int x1, int y1, int x2, int y2, Pixel colour);
void DrawRectSize(FrameBuffer fb, int x, int y, int width, int height, Pixel colour);
void FillRect(FrameBuffer fb, int x1, int y1, int x2, int y2, Pixel colour);
void FillRectSize(FrameBuffer fb, int x, int y, int width, int height, Pixel colour);
void DrawCircle(FrameBuffer fb, int x, int y, int diameter);
void FillCircle(FrameBuffer fb, int x, int y, int diameter);
void BitBlit(
	FrameBuffer src, FrameBuffer dest, 
	int sx, int sy, int dx, int dy,
	int width, int height);


void DrawText(u32 *buffer, char *str, int x, int y, u8 scale);

#endif
