
#include <fcntl.h>
#include <sys/mman.h>
#include <asm-generic/mman.h>

#include "graphics.h"


void InitDisplay(u8 **fb0)
{
	int dispfd = open("/dev/fb0", O_RDWR);
	printf("Display File Descriptor: %d\n", dispfd);

	*fb0 = 0;
	*fb0 = mmap(0, FB_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED|MAP_POPULATE|MAP_LOCKED, dispfd, 0);
	close(dispfd);
}

void CloseDisplay(u8 *fb0)
{
	munmap(fb0, FB_SIZE);
}


