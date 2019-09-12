

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <asm-generic/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <math.h>

#include <linux/videodev2.h>
//#include <libv4l2.h>
#include <errno.h>


#define MIN(a,b) ((a)<=(b)?(a):(b))
#define MAX(a,b) ((a)>=(b)?(a):(b))

// static int xioctl(int fd, int request, void *arg)
// {
// 	int r;
// 	do { r = ioctl (fd, request, arg); }
// 	while (-1 == r && EINTR == errno);
// 	return r;
// }

struct FloodFillLine
{
	unsigned int X, Y;
};


void FindMaxArea(unsigned int width, unsigned int height, unsigned char *fb0, u_int8_t **circmap, u_int32_t **regionmap, u_int32_t *maxregion)
{
	u_int32_t region=0;
	
	*maxregion=0; 
	u_int32_t maxregionarea = 0;
	
	struct FloodFillLine lineStack[height]; u_int32_t lineStackP=-1;
	
	
	for (unsigned int y=0; y<height; y++)
	{
		for (unsigned int x=0; x<width; x++) 
		{ 
			
			if (circmap[y][x]==1 && regionmap[y][x]==0)
			{ 
				
				//Flood Fill
				region++;
				u_int32_t regionarea = 0;
				
				lineStack[++lineStackP] = (struct FloodFillLine){x,y};
				
				unsigned int yf=0, xf=0;
				
				while(lineStackP<-1)
				{
					xf=lineStack[lineStackP].X;
					yf=lineStack[lineStackP].Y;
					lineStackP--;
					
					
					if (regionmap[yf][xf]==0)
					{
						char ue=1, le=1; 
						
						
						while(xf>0 && circmap[yf][xf-1]) { xf--; }
						
						while(xf<width && circmap[yf][xf])
						{
							regionmap[yf][xf] = region;
							//*(u_int32_t *)(fb0+yf*width*4+xf*4) = 0xFF00FFFF;
							regionarea++;
							
							if (yf>0 && circmap[yf-1][xf]) { if (ue && !regionmap[yf-1][xf]) { lineStack[++lineStackP] = (struct FloodFillLine){xf,yf-1}; ue=0; } }
							else { ue = 1; }
							if (yf<height-1 && circmap[yf+1][xf]) { if (le && !regionmap[yf+1][xf]) { lineStack[++lineStackP] = (struct FloodFillLine){xf,yf+1}; le=0; } }
							else { le = 1; }
							
							xf++; 
						}
					}
					
				}
				
				if (regionarea > maxregionarea) { maxregionarea=regionarea; *maxregion=region; }
				
			}
			
		}
	}
}

u_int64_t Time()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return 1000000 * tv.tv_sec + tv.tv_usec;
}

int main()
{
	unsigned int width=0, height=0, fbsize=0;
	
	srand(Time());

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
	
	
	
	printf("Generating Circles\n");
	
	u_int8_t **circmap = malloc(sizeof(u_int8_t *)*height);
	for (int y=0; y<height; y++)
	{
		circmap[y] = malloc(sizeof(u_int8_t)*width);
		for (int x=0; x<width; x++) { circmap[y][x]=0; }
	}
	//
	
	
	for (u_int8_t n=0; n<20; n++)
	{
		u_int32_t s = rand() % 400 + 50;
		
		int x = rand() % width;
		int y = rand() % height;
		
		for (int yi=MAX(y-s,0); yi<MIN(y+s,height-1); yi++)
		{
			int xl = s-sqrt(s*s - pow(y-yi,2));
			
			for (int xi=xl+x; xi<MIN(2*s-xl+1+x, width-1); xi++)
			{
				circmap[yi][xi]=1;
			}
		}
	}
	
	
	
	printf("Processing Data\n");
	
	u_int32_t **regionmap = malloc(sizeof(u_int32_t **)*height);//[height][width][2]; //Region Count
	
	for (int y=0; y<height; y++)
	{
		regionmap[y] = malloc(sizeof(u_int32_t *)*width);
		for (int x=0; x<width; x++) 
		{ 
			regionmap[y][x] = 0; 
		}
	}
	
	u_int64_t t1 = Time();
	int maxregion=0;
	FindMaxArea(width, height, fb0, circmap, regionmap, &maxregion);
	
	u_int64_t avgX=0, avgY=0, avgC=0;
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			if (regionmap[y][x] == maxregion) { avgX+=x; avgY+=y; avgC++; }
		}
	}
	avgX=avgX/avgC;
	avgY=avgY/avgC;
	u_int64_t t2 = Time();
	printf("Average: (%lu %lu)\n", avgX, avgY);
	
	printf("Process time %lluus\n", t2-t1);
	
	printf("Drawing Processed Data\n");
	for (int y=0; y<height; y++)
	{
		for (int x=0; x<width; x++)
		{
			*(u_int32_t *)(fb0+y*width*4+x*4) = regionmap[y][x] == maxregion ? 0xFF00FF00 : (regionmap[y][x] ? 0xFF00FFFF : (circmap[y][x] ? 0xFFFFFFFF : 0) );
		}
	}
	
	for (int y=MAX((long long)avgY-10,0); y<MIN((long long)avgY+10,height-1); y++)
	{
		for (int x=MAX((long long)avgX-10,0); x<MIN((long long)avgX+10,width-1); x++)
		{
			if (sqrt(pow(x-(long long)avgX,2)+pow(y-(long long)avgY,2)) <= 10)
			{
				*(u_int32_t *)(fb0+y*width*4+x*4) = 0xFFFF0000;
			}
		}
	}
	
	
	printf("Cleanup\n");
	for (int y=0; y<height; y++)
	{
		free(regionmap[y]);
		free(circmap[y]);
	}
	
	free(regionmap);
	free(circmap);
	
	// printf("Starting Main Loop...\n");
	// unsigned int i = 0;
	// while(1)
	// {

	// 	for (unsigned int y=0; y<height; y++)
	// 	{
	// 		for (unsigned int x=0; x<width; x++)
	// 		{
	// 			u_int32_t col=0x00000000;//[4]="\xff\x00\x00\x00";
	// 			switch (((y+i)/60)%3)
	// 			{
	// 			case 0:
	// 				col=0xff<<0; break;
	// 			case 1:
	// 				col=0xff<<8; break;
	// 			case 2:
	// 				col=0xff<<16; break;
	// 			}
	// 			//fwrite(col, 4, 1, fbf);
	// 			//memcpy(fb+y*800*4+x*4, col, 4);
	// 			*(u_int32_t *)(fb0+y*width*4+x*4)=col;
	// 		}
	// 	}

	// 	//memcpy(fb0, fb1, FB_SIZE);

	// 	//usleep(100*1000);
	// 	i+=2;
	// 	//fwrite("\x00", 1, 1, f);
	// 	// fseek(fbf, 0, SEEK_SET);
	// 	// fflush(fbf);
	// }

	munmap(fb0, fbsize);

	//fclose(fbf);

	return 0;
}
