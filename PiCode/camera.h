
#include <sys/types.h>

#define CAMWIDTH 640
#define CAMHEIGHT 480


void InitCamera(int *camfd, u_int8_t **buffer);
void DestroyCamera(int camfd, u_int8_t **buffer);
void QueueBuffer(int camfd);
void DeQueueBuffer(int camfd);

