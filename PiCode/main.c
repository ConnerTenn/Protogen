

#include <signal.h>

#include "globals.h"
#include "camera.h"
#include "graphics.h"


union Ksequ
{
	u8 seq[8];
	u32 val;
};


struct FloodFillLine
{
	unsigned int X, Y;
};

u8 Run = 1;

void InteruptHandler(int arg) { reset_terminal_mode(); Run=0; }

int main()
{
	signal(SIGINT, InteruptHandler); signal(SIGKILL, InteruptHandler);

	//ioctl(0, KDSETMODE, KD_GRAPHICS);
	set_conio_terminal_mode();


	u32 *fb0 = 0, *fb1 = 0;
	InitDisplay(&fb0);
	CreateFrameBuffer(&fb1, FBWIDTH, FBHEIGHT);

	int camfd; u8 *cambuff;
	InitCamera(&camfd, &cambuff);
	
	int threshold = 127;

	while(Run)
	{
		while(kbhit())
		{
			u8 ch;// = getch();
			union Ksequ sequ = {.val=0};
			u8 seqc=0;
			while(kbhit()) { sequ.seq[seqc++]=getch();} sequ.seq[seqc]=0;
			ch=sequ.seq[0];
			
			printf("PRESS: %d(%X)\n%u(%08X)\n", ch, ch, sequ.val, sequ.val);

			if (sequ.val==0x415B1B) { threshold-=5; }
			if (sequ.val==0x425B1B) { threshold+=5; }
			if (threshold<0) { threshold=0; }
			if (threshold>255) { threshold=255; }
		}


		QueueBuffer(camfd);

		WaitForFrame(camfd);

		DeQueueBuffer(camfd);

		u32 camFrame[CAMHEIGHT][CAMWIDTH];
		for (unsigned int y=0; y<CAMHEIGHT; y++)
		{
			for (unsigned int x=0; x<CAMWIDTH; x++)
			{
				//struct PixelData col={0,0,0,0};//0x00000000;//[4]="\xff\x00\x00\x00";
				
				u32 col = CAMACC(cambuff, x, y);

				u8 val = (((col>>24)&0xFF) + ((col>>16)&0xFF) + (col&0xFF))/3;

				if (val<(u8)threshold) { col = 0x00000000; }
				
				camFrame[y][x] = col;
			}
		}

		u32 regionmap[CAMHEIGHT][CAMWIDTH];
		memset(regionmap, 0, CAMWIDTH*CAMHEIGHT*sizeof(u32));

		u32 maxregion;
		u32 avgX=0, avgY=0, avgC=0;

		{		
			u32 region=0;
			
			maxregion=0;
			u32 maxregionarea = 0;
			
			struct FloodFillLine lineStack[CAMHEIGHT]; u32 lineStackP=-1;

			const unsigned int crop=40;
			for (unsigned int y=crop; y<CAMHEIGHT-crop; y++)
			{
				for (unsigned int x=crop; x<CAMWIDTH-crop; x++)
				{
					//if black and not part of region
					if (camFrame[y][x]==0 && regionmap[y][x]==0)
					{ 
						
						//Flood Fill
						region++;
						u32 regionarea = 0;
						
						lineStack[++lineStackP] = (struct FloodFillLine){x,y};
						
						unsigned int yf=0, xf=0;
						
						//Handle every line that is part of the region
						while(lineStackP<-1)
						{
							//Pop off of stack
							xf=lineStack[lineStackP].X;
							yf=lineStack[lineStackP].Y;
							lineStackP--;
							
							
							//Check if line is incomplete
							if (regionmap[yf][xf]==0)
							{
								char ue=1, le=1; 
								
								//Move to the beginning of the line
								while(xf>crop && camFrame[yf][xf-1]==0) { xf--; }
								
								//Loop forward through the line
								while(xf<CAMWIDTH-crop && camFrame[yf][xf]==0)
								{
									regionmap[yf][xf] = region;
									//*(u32 *)(fb0+yf*width*4+xf*4) = 0xFF00FFFF;
									regionarea++;
									
									//Check if line above or below is unaccounted for. If so, add it to the stack.
									//Only do this at the beginning of a line or after a non-region above/below 
									if (yf>crop && camFrame[yf-1][xf]==0) { if (ue && !regionmap[yf-1][xf]) { lineStack[++lineStackP] = (struct FloodFillLine){xf,yf-1}; ue=0; } }
									else { ue = 1; }
									if (yf<CAMHEIGHT-crop-1 && camFrame[yf+1][xf]==0) { if (le && !regionmap[yf+1][xf]) { lineStack[++lineStackP] = (struct FloodFillLine){xf,yf+1}; le=0; } }
									else { le = 1; }
									
									xf++; 
								}
							}
							
						}
						
						if (regionarea > maxregionarea) { maxregionarea=regionarea; maxregion=region; }
						
					}
				}
			}

			for (int y=0; y<CAMHEIGHT; y++)
			{
				for (int x=0; x<CAMWIDTH; x++)
				{
					if (regionmap[y][x] == maxregion) { avgX+=x; avgY+=y; avgC++; }
				}
			}
			avgX=avgX/avgC;
			avgY=avgY/avgC;
		}

		for (unsigned int y=0; y<FBHEIGHT; y++)
		{
			for (unsigned int x=0; x<FBWIDTH; x++)
			{

				u32 col = 0;
				if (x>=160)
				{
					if (maxregion && regionmap[y][x-160]==maxregion) { col = 0x0000FF00; }
					else if (regionmap[y][x-160]) { col = 0x0000FFFF; }
					else { col = camFrame[y][x-160]; } 
				}
				else { col = 0x00000000; }

	
				if (abs((int)(x-160)-(int)avgX) <= 5 && abs((int)y-(int)avgY) <= 5)
				{
					col = 0x00FF0000;
				}

				FBACC(fb1, x, y) = col;
			}
		}

		FillRect(fb1, FBWIDTH, 50, 50, 400, 400, (Pixel){.R=0xFF, .G=0x00, .B=0xFF, .A=0x7F});

		for (u16 y=0; y<FBHEIGHT; y++)
		{
			for (u16 x=0; x<FBWIDTH; x++)
			{
				FBACC(fb0, x, y) = FBACC(fb1, x, y);
			}
		}

		usleep(1000);
	}

	CloseCamera(camfd, &cambuff);

	
	DesctroyFrameBuffer(fb1);
	CloseDisplay(fb0);

	//ioctl(0, KDSETMODE, KD_TEXT);

	reset_terminal_mode();

	printf("\n Done\n");
	return 0;
}
