
#include <fcntl.h>
#include <sys/mman.h>
#include <asm-generic/mman.h>

#include "graphics.h"


void InitDisplay(u32 **fb0)
{
	int dispfd = open("/dev/fb0", O_RDWR);
	printf("Display File Descriptor: %d\n", dispfd);

	*fb0 = 0;
	*fb0 = mmap(0, FB_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED|MAP_POPULATE|MAP_LOCKED, dispfd, 0);
	close(dispfd);
}

void CloseDisplay(u32 *fb0)
{
	munmap(fb0, FB_SIZE);
}

FrameBuffer CreateFrameBuffer(u16 width, u16 height)
{
	FrameBuffer fb;
	fb.Buff = malloc(width * height * 4);
	fb.Width=width;
	fb.Height=height;
	return fb;
}
void DestroyFrameBuffer(FrameBuffer fb)
{
	free(fb.Buff);
}


Pixel WordToPixel(u32 pix)
{
	return (Pixel){ .R=(pix&0x00FF0000)>>(8*2), .G=(pix&0x0000FF00)>>(8*1), .B=pix&0x000000FF, .A=(pix&0xFF000000)>>(8*3) };
}
u32 PixelToWord(Pixel pix)
{
	return (pix.R << (8*2)) | (pix.G << (8*1)) | (pix.B) | (pix.A << (8*3));
}

void SetPixel(u32 *buff, int w, int x, int y, Pixel colour)
{
	Pixel background = WordToPixel(buff[y*w + x]);
	colour.R = (colour.R * colour.A + background.R * (0xFF-colour.A) ) / 0xFF;
	colour.G = (colour.G * colour.A + background.G * (0xFF-colour.A) ) / 0xFF;
	colour.B = (colour.B * colour.A + background.B * (0xFF-colour.A) ) / 0xFF;
	buff[y*w + x] = PixelToWord(colour);
}


/*
plotLineLow(x0,y0, x1,y1)
  dx = x1 - x0
  dy = y1 - y0
  yi = 1
  if dy < 0
    yi = -1
    dy = -dy
  end if
  D = 2*dy - dx
  y = y0

  for x from x0 to x1
    plot(x,y)
    if D > 0
       y = y + yi
       D = D - 2*dx
    end if
    D = D + 2*dy

plotLineHigh(x0,y0, x1,y1)
  dx = x1 - x0
  dy = y1 - y0
  xi = 1
  if dx < 0
    xi = -1
    dx = -dx
  end if
  D = 2*dx - dy
  x = x0

  for y from y0 to y1
    plot(x,y)
    if D > 0
       x = x + xi
       D = D - 2*dy
    end if
    D = D + 2*dx

plotLine(x0,y0, x1,y1)
  if abs(y1 - y0) < abs(x1 - x0)
    if x0 > x1
      plotLineLow(x1, y1, x0, y0)
    else
      plotLineLow(x0, y0, x1, y1)
    end if
  else
    if y0 > y1
      plotLineHigh(x1, y1, x0, y0)
    else
      plotLineHigh(x0, y0, x1, y1)
    end if
  end if
*/
void DrawLineLow(u32 *buff, int w, int x1, int y1, int x2, int y2, Pixel colour)
{
	int dx = x2 - x1, dy = y2 - y1, yi = 1;
	if (dy < 0) { yi = -1; dy = -dy; }
	int D = 2*dy - dx, y = y1;

	for (int x=x1; x<x2; x++)
	{
		SetPixel(buff, w, x, y, colour);
		if (D > 0) { y = y + yi; D = D - 2*dx; }
		D = D + 2*dy;
	}
}
void DrawLineHigh(u32 *buff, int w, int x1, int y1, int x2, int y2, Pixel colour)
{
	int dx = x2 - x1, dy = y2 - y1, xi = 1;
	if (dx < 0) { xi = -1; dx = -dx; }
	int D = 2*dx - dy, x = x1;

	for (int y=y1; y<y2; y++)
	{
		SetPixel(buff, w, x, y, colour);
		if (D > 0) { x = x + xi; D = D - 2*dy; }
		D = D + 2*dx;
	}
}

void DrawLine(FrameBuffer fb, int x1, int y1, int x2, int y2, Pixel colour)
{
	if (abs(y2 - y1) < abs(x2 - x1))
    {
		if (x1 > x2) { DrawLineLow(fb.Buff, fb.Width, x2, y2, x1, y1, colour); }
		else { DrawLineLow(fb.Buff, fb.Width, x1, y1, x2, y2, colour); }
	}
	else
	{
		if (y1 > y2) { DrawLineHigh(fb.Buff, fb.Width, x2, y2, x1, y1, colour); }
		else { DrawLineHigh(fb.Buff, fb.Width, x1, y1, x2, y2, colour); }
	}
}

void DrawRect(FrameBuffer fb, int x1, int y1, int x2, int y2, Pixel colour)
{
	int t;
	if (x1>x2) { t=x1; x1=x2; x2=t; }
	if (y1>y2) { t=y1; y1=y2; y2=t; }
	
	for (int i=x1; i<=x2; i++)
	{
		SetPixel(fb.Buff, fb.Width, x1+i, y1, colour);
		SetPixel(fb.Buff, fb.Width, x1+i, y2, colour);
	}
	for (int i=y1; i<=y2; i++)
	{
		SetPixel(fb.Buff, fb.Width, x1, y1+i, colour);
		SetPixel(fb.Buff, fb.Width, x2, y1+i, colour);
	}
}

void DrawRectSize(FrameBuffer fb, int x, int y, int width, int height, Pixel colour)
{
	DrawRect(fb, x, y, x+width, y+height, colour);
}

void FillRect(FrameBuffer fb, int x1, int y1, int x2, int y2, Pixel colour)
{
	int t;
	if (x1>x2) { t=x1; x1=x2; x2=t; }
	if (y1>y2) { t=y1; y1=y2; y2=t; }

	for (int y=y1; y<=y2; y++)
	{
		for (int x=x1; x<=x2; x++)
		{
			SetPixel(fb.Buff, fb.Width, x, y, colour);
		}
	}
}

void FillRectSize(FrameBuffer fb, int x, int y, int width, int height, Pixel colour)
{
	FillRect(fb, x, y, x+width, y+height, colour);
}

void DrawCircle(FrameBuffer fb, int x, int y, int diameter)
{
}

void FillCircle(FrameBuffer fb, int x, int y, int diameter)
{

}

void BitBlit(
	FrameBuffer src, FrameBuffer dest, 
	int sx, int sy, int dx, int dy,
	int width, int height)
{
	for (u32 y=0; y<height; y++)
	{
		for (u32 x=0; x<width; x++)
		{
			dest.Buff[(dy+y)*dest.Width+(x+dx)] = src.Buff[(sy+y)*src.Width+(x+sx)];
		}
	}
}

