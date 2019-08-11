

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <linux/videodev2.h>
#include <libv4l2.h>

#define FB_SIZE (480*800*4)

int main()
{
	//FILE *fbf = fopen("/dev/fb0", "wb");
	int fd = open("/dev/fb0", O_RDWR);
	printf("File Descriptor: %d\n", fd);
	close(fd);

	unsigned char *fb0 = 0;//
	//unsigned char fb1[FB_SIZE];
	fb0 = mmap(0, FB_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED|MAP_POPULATE|MAP_LOCKED,fd,0);
	
	printf("MMaped Address: %p\n", fb0);

	printf("Starting Main Loop...\n");
	unsigned int i = 0;
	while(1)
	{

		for (unsigned int y=0; y<480; y++)
		{
			for (unsigned int x=0; x<800; x++)
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
				*(u_int32_t *)(fb0+y*800*4+x*4)=col;
			}
		}

		//memcpy(fb0, fb1, FB_SIZE);

		//usleep(100*1000);
		i+=2;
		//fwrite("\x00", 1, 1, f);
		// fseek(fbf, 0, SEEK_SET);
		// fflush(fbf);
	}

	munmap(fb0, 480*800*4);

	//fclose(fbf);

	return 0;
}
