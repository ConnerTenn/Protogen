
#include "globals.h"

#define CAMWIDTH 640
#define CAMHEIGHT 480

#define CAMACC(buff,x,y) (ARRACC((buff), (x), (y), CAMWIDTH, 3, u32 *)&0xFFFFFF)

void InitCamera(int *camfd, u8 **buffer);
void CloseCamera(int camfd, u8 **buffer);
void QueueBuffer(int camfd);
void DeQueueBuffer(int camfd);
void WaitForFrame(int camfd);

