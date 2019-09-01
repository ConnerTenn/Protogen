

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <math.h>


// static int xioctl(int fd, int request, void *arg)
// {
// 	int r;
// 	do { r = ioctl (fd, request, arg); }
// 	while (-1 == r && EINTR == errno);
// 	return r;
// }

int main()
{
	unsigned int width=0, height=0, fbsize=0;

	int sizefd = open("/sys/class/graphics/fb0/virtual_size", O_RDONLY);
	char sizebuff[32];
	read(sizefd, sizebuff, 32);
	width=atoi(sizebuff);
	height=atoi(sizebuff+(int)log10(width)+2);
	fbsize=width*height*4;
	close(sizefd);
	printf("Width:%d Height:%d Size:%d\n", width, height, fbsize);

	//FILE *fbf = fopen("/dev/fb0", "wb");
	int dispfd = open("/dev/fb0", O_RDWR);
	printf("Display File Descriptor: %d\n", dispfd);

	unsigned char *fb0 = 0;//
	//unsigned char fb1[FB_SIZE];
	fb0 = mmap(0, fbsize, PROT_WRITE|PROT_READ, MAP_SHARED|MAP_POPULATE|MAP_LOCKED,dispfd,0);
	printf("mmap Address: %p\n", fb0);
	if (fb0==MAP_FAILED) { perror("Error: mmap failed: "); exit(1); }
	close(dispfd);
	
	printf("Starting Main Loop...\n");
	unsigned int i = 0;
	while(1)
	{

		for (unsigned int y=0; y<height; y++)
		{
			for (unsigned int x=0; x<width; x++)
			{
				u_int32_t col=0x00000000;//[4]="\xff\x00\x00\x00";
				switch (((y+i)/60)%3)
				{
				case 0:
					col=0xff<<0; break;
				case 1:
					col=0xff<<8; break;
				case 2:
					col=0xff<<16; break;
				}
				//fwrite(col, 4, 1, fbf);
				//memcpy(fb+y*800*4+x*4, col, 4);
				*(u_int32_t *)(fb0+y*width*4+x*4)=col;
			}
		}

		//memcpy(fb0, fb1, FB_SIZE);

		//usleep(100*1000);
		i+=2;
		//fwrite("\x00", 1, 1, f);
		// fseek(fbf, 0, SEEK_SET);
		// fflush(fbf);
	}

	munmap(fb0, fbsize);

	//fclose(fbf);

	return 0;
}
