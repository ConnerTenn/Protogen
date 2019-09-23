
#include "globals.h"

#include <sys/mman.h>

#define FBWIDTH 800
#define FBHEIGHT 480

#define FB_SIZE (FBHEIGHT*FBWIDTH*4)

void InitDisplay(u8 **fb0);
void CloseDisplay(u8 *fb0);
