

#include <signal.h>

#include "globals.h"
#include "camera.h"
#include "graphics.h"
#include "controller.h"
#include "matrix.h"

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

		double sx1=300, sy1=250, sx2=500, sy2=250, sx3=500, sy3=400, sx4=300, sy4=400;
		double dx1=0, dy1=0, dx2=FBWIDTH, dy2=0, dx3=FBWIDTH, dy3=FBHEIGHT, dx4=0, dy4=FBHEIGHT;
		double sx=eyeX,sy=eyeY,dx,dy;

		ProjectiveTransformation(sx1,sy1,sx2,sy2,sx3,sy3,sx4,sy4,dx1,dy1,dx2,dy2,dx3,dy3,dx4,dy4,sx,sy,&dx,&dy);

		FillRectSize(fb1, sx1-5, sy1-5, 10, 10, PIXEL(0xFF, 0xFF, 0x00, 0xFF));
		FillRectSize(fb1, sx2-5, sy2-5, 10, 10, PIXEL(0xFF, 0xFF, 0x00, 0xFF));
		FillRectSize(fb1, sx3-5, sy3-5, 10, 10, PIXEL(0xFF, 0xFF, 0x00, 0xFF));
		FillRectSize(fb1, sx4-5, sy4-5, 10, 10, PIXEL(0xFF, 0xFF, 0x00, 0xFF));

		FillRectSize(fb1, dx1-5, dy1-5, 10, 10, PIXEL(0xFF, 0xFF, 0x00, 0xFF));
		FillRectSize(fb1, dx2-5, dy2-5, 10, 10, PIXEL(0xFF, 0xFF, 0x00, 0xFF));
		FillRectSize(fb1, dx3-5, dy3-5, 10, 10, PIXEL(0xFF, 0xFF, 0x00, 0xFF));
		FillRectSize(fb1, dx4-5, dy4-5, 10, 10, PIXEL(0xFF, 0xFF, 0x00, 0xFF));

		FillRectSize(fb1, sx-5, sy-5, 10, 10, PIXEL(0xFF, 0x00, 0x00, 0xFF));
		FillRectSize(fb1, dx-5, dy-5, 10, 10, PIXEL(0x00, 0x00, 0xFF, 0xFF));


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
