
#include "globals.h"


#define FBWIDTH 800
#define FBHEIGHT 480

#define FB_SIZE (FBHEIGHT*FBWIDTH*4)

#define FBACC(buff,x,y) (ARRACC((buff), (x), (y), FBWIDTH, 4, u32 *))

void InitDisplay(u8 **fb0);
void CloseDisplay(u8 *fb0);

void DrawLine();
void DrawRect();
void FillRect();
void DrawCircle();
void FillCircle();
void BitBlit();
