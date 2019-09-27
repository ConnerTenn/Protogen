

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


	u32 *fb0 = 0;
	InitDisplay(&fb0);
	FrameBuffer fb1 = CreateFrameBuffer(FBWIDTH, FBHEIGHT);

	int camfd; u8 *cambuff;
	InitCamera(&camfd, &cambuff);
	
	int threshold = 25;//127;

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

			if (sequ.val==0x415B1B) { threshold-=2; }
			if (sequ.val==0x425B1B) { threshold+=2; }
			if (threshold<0) { threshold=0; }
			if (threshold>255) { threshold=255; }
		}


		QueueBuffer(camfd);

		WaitForFrame(camfd);

		DeQueueBuffer(camfd);

		memset(fb1.Buff, 0, fb1.Width*fb1.Height*4);

		u32 eyeTrackBuff[CAMHEIGHT*CAMWIDTH];
		int eyeX=0, eyeY=0;
		EyeTracking(cambuff, &eyeX, &eyeY, threshold, eyeTrackBuff);

		BitBlit((FrameBuffer){.Buff=eyeTrackBuff,.Width=CAMWIDTH}, fb1, 0, 0, 0, 0, CAMWIDTH, CAMHEIGHT);

		FillRect(fb1, 50, 50, 400, 400, (Pixel){.R=0xFF, .G=0x00, .B=0xFF, .A=0x7F});

		DrawText(fb1, "ABCDEFGHIJLMNOPQRSTUVWXYZ", 10, 20, (Pixel){.R=0xFF,.B=0x00,.G=0x00,.A=0xFF}, (Pixel){.R=0x00,.B=0x00,.G=0x00,.A=0x00}, 1);
		DrawText(fb1, "ABCDEFGHIJLMNOPQRSTUVWXYZ", 10, 60, (Pixel){.R=0xFF,.B=0x00,.G=0x00,.A=0xFF}, (Pixel){.R=0x00,.B=0x00,.G=0x00,.A=0x00}, 2);
		DrawText(fb1, "ABCDEFGHIJLMNOPQRSTUVWXYZ", 10, 100, (Pixel){.R=0xFF,.B=0x00,.G=0x00,.A=0xFF}, (Pixel){.R=0x00,.B=0x00,.G=0x00,.A=0x00}, 3);
		DrawText(fb1, "ABCDEFGHIJLMN\nOPQRSTUVWXYZ", 10, 140, (Pixel){.R=0xFF,.B=0x00,.G=0x00,.A=0xFF}, (Pixel){.R=0x00,.B=0x00,.G=0x00,.A=0x00}, 4);

		{
			char printbuff[255];
			u8 j=0;
			for (u8 i=1; i<128; i++)
			{
				printbuff[j++]=(char)i;
				if (i%26==0) { printbuff[j++] = '\n'; }
			}
			DrawText(fb1, printbuff, 10, 220, (Pixel){.R=0xFF,.B=0x00,.G=0xFF,.A=0xFF}, (Pixel){.R=0x00,.B=0x00,.G=0x00,.A=0x00}, 4);
		}

		for (u32 y=0; y<FBHEIGHT; y++)
		{
			for (u32 x=0; x<FBWIDTH; x++)
			{
				FBACC(fb0, x, y) = fb1.Buff[y*fb1.Width+x];
			}
		}
	}

	CloseCamera(camfd, &cambuff);

	
	DestroyFrameBuffer(fb1);
	CloseDisplay(fb0);

	//ioctl(0, KDSETMODE, KD_TEXT);

	reset_terminal_mode();

	printf("\n Done\n");
	return 0;
}
