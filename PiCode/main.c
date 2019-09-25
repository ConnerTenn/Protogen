

#include <signal.h>

#include "globals.h"
#include "camera.h"
#include "graphics.h"


union Ksequ
{
	u8 seq[8];
	u32 val;
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

		u32 eyeTrackBuff[CAMHEIGHT*CAMWIDTH];
		int eyeX=0, eyeY=0;
		EyeTracking(cambuff, &eyeX, &eyeY, threshold, eyeTrackBuff); 

		BitBlit(eyeTrackBuff, fb1, 0, 0, 160, 0, CAMWIDTH, FBWIDTH, CAMWIDTH, CAMHEIGHT);

		FillRect(fb1, FBWIDTH, 50, 50, 400, 400, (Pixel){.R=0xFF, .G=0x00, .B=0xFF, .A=0x7F});

		for (u16 y=0; y<FBHEIGHT; y++)
		{
			for (u16 x=0; x<FBWIDTH; x++)
			{
				FBACC(fb0, x, y) = FBACC(fb1, x, y);
			}
		}
	}

	CloseCamera(camfd, &cambuff);

	
	DesctroyFrameBuffer(fb1);
	CloseDisplay(fb0);

	//ioctl(0, KDSETMODE, KD_TEXT);

	reset_terminal_mode();

	printf("\n Done\n");
	return 0;
}
