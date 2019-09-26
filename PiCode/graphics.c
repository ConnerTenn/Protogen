
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

u8 CharacterMap[];
u8 FontData[];

void DrawText(FrameBuffer fb, char *str, int x, int y, Pixel foreground, Pixel background, u8 scale)
{
	u16 nl=0;
	for (u32 i=0;str[i];i++)
	{
		if (str[i]=='\n') { nl+=1; }
		else
		{
			u16 c = CharacterMap[(u8)str[i]]*5*9;
			for (u16 ci=0; ci<5*9*scale*scale; ci++)
			{
				u16 cx = ci%(5*scale);
				u16 cy = ci/(5*scale);
				SetPixel(fb.Buff, fb.Width, x+i*6*scale+cx, y+cy+10*nl*scale, FontData[c + ((ci)%(5*scale))/scale + (5*(ci/(5*scale*scale)))] ? foreground : background);
			}
		}
	}
}


u8 CharacterMap[] = 
{
//  0
	0,
//  1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  26  28  29  30  31
	95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 

//  32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47
	0,  63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77,

//  48  49  50  51  52  53  54  55  56  57
	53, 54, 55, 56, 57, 58, 59, 60, 61, 62,

//  58  59  60  61  62  63  64
	78, 79, 80, 81, 82, 83, 84,

//  65  66  67  68  69  70  71  72  73  74  75  76  77  78  79  80  81  82  83  84  85  86  87  88  89  90
	1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,

//  91  92  93  94  95  96
	85, 86, 87, 88, 89, 90,

//  97  98  99  100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122
	27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,

//  123 124 125 126 127
	91, 92, 93, 94, 95
};

u8 FontData[] =
{
	//Space 0
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	// Uppercase

	//A 1
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 1, 1, 1, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	//B 2
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	//C 3
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	//D 4
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	//E 5
	1, 1, 1, 1, 1,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	//F 6
	1, 1, 1, 1, 1,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//G 7
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 0,
	1, 0, 1, 1, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//H 8
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 1, 1, 1, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//I 9
	1, 1, 1, 1, 1,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	//J 10
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//K 11
	1, 0, 0, 0, 1,
	1, 0, 0, 1, 0,
	1, 0, 1, 0, 0,
	1, 1, 0, 0, 0,
	1, 0, 1, 0, 0,
	1, 0, 0, 1, 0,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//L 12
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//M 13
	1, 0, 0, 0, 1,
	1, 1, 0, 1, 1,
	1, 0, 1, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//N 14
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 1, 0, 0, 1,
	1, 0, 1, 0, 1,
	1, 0, 0, 1, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//O 15
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//P 16
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//Q 17
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 1,
	0, 0, 0, 0, 0,
	
	//R 18
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//S 19
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//T 20
	1, 1, 1, 1, 1,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//U 21
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//V 22
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 0, 1, 0,
	0, 1, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//W 23
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 1, 0, 1,
	1, 1, 0, 1, 1,
	1, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//X 24
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 1, 0, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//Y 25
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	//Z 26
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 1, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	// Lowercase

	//a 27
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//b 28
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//c 29
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//d 30
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	0, 1, 1, 1, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//e 31
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 1, 1, 1, 1,
	1, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//f 32
	0, 0, 1, 1, 0,
	0, 1, 0, 0, 1,
	0, 1, 0, 0, 0,
	1, 1, 1, 1, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//g 33
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 1,
	0, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	
	//h 34
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//i 35
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//j 36
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 1, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	
	//k 37
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 1, 0,
	1, 1, 1, 0, 0,
	1, 0, 0, 1, 0,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//l 38
	1, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 0, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//m 39
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 0,
	1, 0, 1, 0, 1,
	1, 0, 1, 0, 1,
	1, 0, 1, 0, 1,
	1, 0, 1, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//n 48
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//o 41
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//p 42
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	
	//a 43
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	
	//r 44
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 0, 1, 1, 0,
	1, 1, 0, 0, 1,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//s 45
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 1,
	1, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//t 46
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	1, 1, 1, 1, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 0, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//u 47
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//v 48
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//w 49
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 1, 0, 1,
	1, 0, 1, 0, 1,
	0, 1, 0, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//x 50
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 0, 0, 0, 1,
	0, 1, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 1, 0, 1, 0,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//y 51
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 1,
	0, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	
	//z 52
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 1,
	0, 0, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 1, 0, 0, 0,
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	// Numbers
	
	//0 53
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 1, 1,
	1, 0, 1, 0, 0,
	1, 1, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	//1 54
	0, 0, 1, 0, 0,
	0, 1, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//2 55
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 1, 0, 0, 0,
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//3 56
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	0, 0, 1, 1, 0,
	0, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//4 57
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//5 58
	0, 1, 1, 1, 1,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 1, 0,
	0, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//6 59
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//7 60
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//8 61
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//9 62
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	// Symbols
	
	//! 63
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	//" 64
	0, 1, 0, 1, 0,
	0, 1, 0, 1, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//# 65
	0, 0, 0, 0, 0,
	0, 1, 0, 1, 0,
	1, 1, 1, 1, 1,
	0, 1, 0, 1, 0,
	0, 1, 0, 1, 0,
	1, 1, 1, 1, 1,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//$ 66
	0, 0, 1, 0, 0,
	0, 1, 1, 1, 1,
	1, 0, 1, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 1, 0, 1,
	1, 1, 1, 1, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//% 67
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 0, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 1, 0, 0, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//& 68
	0, 1, 1, 0, 0,
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
	0, 1, 1, 1, 1,
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
	0, 1, 1, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//' 69
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//( 70
	0, 0, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//) 71
	0, 1, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 1, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//* 72
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 0,
	1, 0, 1, 0, 1,
	0, 1, 1, 1, 0,
	1, 0, 1, 0, 1,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//+ 73
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	1, 1, 1, 1, 1,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//, 74
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 1, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//- 75
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//. 76
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	/// 77
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 1, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//: 78
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//; 79
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 1, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//< 80
	0, 0, 0, 0, 0,
	0, 0, 0, 1, 1,
	0, 1, 1, 0, 0,
	1, 0, 0, 0, 0,
	0, 1, 1, 0, 0,
	0, 0, 0, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//= 81
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	//> 82
	0, 0, 0, 0, 0,
	1, 1, 0, 0, 0,
	0, 0, 1, 1, 0,
	1, 0, 0, 0, 1,
	0, 0, 1, 1, 0,
	1, 1, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//? 83
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	//@ 84
	0, 1, 1, 1, 0,
	1, 0, 0, 1, 1,
	1, 0, 1, 0, 1,
	1, 0, 1, 0, 1,
	1, 0, 0, 1, 1,
	1, 0, 0, 0, 0,
	0, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//[ 85
	0, 0, 1, 1, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//Backslash 86
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 1, 0,
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//] 87
	0, 1, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 1, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	//^ 88
	0, 0, 1, 0, 0,
	0, 1, 0, 1, 0,
	1, 0, 0, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//_ 89
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//` 90
	1, 0, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	//{ 91
	0, 0, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 1, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//| 92
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//} 93
	0, 1, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 1, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
	//~ 94
	0, 1, 0, 0, 1,
	1, 0, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	//Box 95
	1, 1, 1, 1, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	
};

/*
.#####.
##...##
##...##
##...##
#######
##...##
##...##
##...##
##...##

######.
##...##
##...##
##...##
######.
##...##
##...##
##...##
######.

.#####.
##....#
##.....
##.....
##.....
##.....
##.....
##....#
.#####.

######.
##...##
##...##
##...##
##...##
##...##
##...##
##...##
######.

#######
##.....
##.....
##.....
######.
##.....
##.....
##.....
##.....

.#####.
##....#
##.....
##.....
##..###
##...##
##...##
##...##
.#####.

##...##
##...##
##...##
##...##
#######
##...##
##...##
##...##
##...##

#######
..###..
..###..
..###..
..###..
..###..
..###..
..###..
#######

.....##
.....##
.....##
.....##
.....##
.....##
##...##
##...##
.#####.


##...##
##...##
##..##.
##.##..
###....
##.##..
##..##.
##...##
##...##

##.....
##.....
##.....
##.....
##.....
##.....
##.....
##.....
#######

#.....#
##...##
###.###
#.###.#
#..#..#
#.....#
#.....#
#.....#
#.....#

#.....#
##....#
###...#
#.##..#
#..##.#
#...###
#....##
#.....#
#.....#

.#####.
##...##
##...##
##...##
##...##
##...##
##...##
##...##
.#####.


*/