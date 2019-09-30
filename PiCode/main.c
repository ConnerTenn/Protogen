

#include <signal.h>

#include "globals.h"
#include "camera.h"
#include "graphics.h"
#include "controller.h"

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
	
	//127;

	while(Run)
	{
		QueueBuffer(camfd);

		WaitForFrame(camfd);

		DeQueueBuffer(camfd);

		while(kbhit())
		{
			//u8 ch;// = getch();
			union Ksequ sequ = {.val=0};
			u8 seqc=0;
			while(kbhit()) { sequ.seq[seqc++]=getch();} sequ.seq[seqc]=0;
			//ch=sequ.seq[0];
			
			//printf("PRESS: %d(%X)\n%u(%08X)\n", ch, ch, sequ.val, sequ.val);

			HandleKeyEvent(sequ);
		}

		memset(fb1.Buff, 0, fb1.Width*fb1.Height*4);

		u32 eyeTrackBuff[CAMHEIGHT*CAMWIDTH];
		int eyeX=0, eyeY=0;
		EyeTracking(cambuff, &eyeX, &eyeY, eyeTrackBuff);

		BitBlit((FrameBuffer){.Buff=eyeTrackBuff,.Width=CAMWIDTH}, fb1, 0, 0, 0, 0, CAMWIDTH, CAMHEIGHT);


		RenderPanels(fb1);

		//Draw Cursor
		for (int y=MAX(eyeY-5,0); y<=MIN(eyeY+5,FBHEIGHT-1); y++)
		{
			for (int x=MAX(eyeX-5,0); x<=MIN(eyeX+5,FBWIDTH-1); x++)
			{
				SetPixel(fb1.Buff, fb1.Width, x, y, PIXEL(0xFF,0x00,0x00,0xFF));
			}
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
