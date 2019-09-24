
#include "globals.h"


#define FBWIDTH 800
#define FBHEIGHT 480

#define FB_SIZE (FBHEIGHT*FBWIDTH*4)

#define FBACC(buff,x,y) (ARRACC((buff), (x), (y), FBWIDTH, 4, u32 *))

typedef struct Pixel_t
{
	u8 R;
	u8 G;
	u8 B;
	u8 A;
} Pixel; 

void InitDisplay(u8 **fb0);
void CloseDisplay(u8 *fb0);

Pixel WordToPixel(u32 pix);
u32 PixelToWord(Pixel pix);

void SetPixel(u32 *buffer, int w, int x, int y, Pixel colour);


void DrawLine(u32 *buffer, int w, int x1, int y1, int x2, int y2, Pixel colour);
void DrawRect(u32 *buffer, int w, int x1, int y1, int x2, int y2, Pixel colour);
void DrawRectSize(u32 *buffer, int w, int x, int y, int width, int height, Pixel colour);
void FillRect(u32 *buffer, int w, int x1, int y1, int x2, int y2, Pixel colour);
void FillRectSize(u32 *buffer, int w, int x, int y, int width, int height, Pixel colour);
void DrawCircle(u32 *buffer, int w, int x, int y, int diameter);
void FillCircle(u32 *buffer, int w, int x, int y, int diameter);
void BitBlit(u32 *src, u32 *dest, int sx, int sy, int dx, int dy, int width, int height);
