
#include "controller.h"

#include <stdarg.h>

enum Panels PanelState = Overview;

u8 BatteryLevel;

#define LOGLEN 24
char LogBuffer[LOGLEN][32];
u8 LogBufferBegin=0;

void HandleEyeEvent(int eyeX, int eyeY)
{
}

void HandleKeyEvent(union Ksequ sequ)
{
	if (sequ.val==0x00415B1B) { Threshold-=2; }
	if (sequ.val==0x00425B1B) { Threshold+=2; }
	if (Threshold<0) { Threshold=0; }
	if (Threshold>255) { Threshold=255; }


	if (sequ.val==0x00000031) { PanelState=Overview; }
	if (sequ.val==0x00000032) { PanelState=DoubleEmotion; }
	if (sequ.val==0x00000033) { PanelState=ChestConfig; }
	if (sequ.val==0x00000034) { PanelState=AudioPipe; }
	if (sequ.val==0x00000035) { PanelState=Terminal; }

	Log("PRESS: %u(%08X)", sequ.val, sequ.val);
}

void RenderPanels(FrameBuffer fb)
{
	
	
	// Render Top Panel
	FillRectSize(fb, 0, 0, FBWIDTH, 20, PIXEL(0x1F,0x1F,0x1F,0xAF));

	// Uptime Battery
	{
		char buffer[100];
		sprintf(buffer, "Uptime: %d:%d  Battery: %%%d", 0, 24, 94);
		DrawText(fb, buffer, 2, 2, PIXEL(0xFF,0xFF,0xFF,0xFF), PIXEL(0x00,0x00,0x00,0x00), 2);
	}

	//Panel Select
	Pixel active = PIXEL(0x3F,0x7F,0xBF,0xAF), inactive = PIXEL(0x1F,0x1F,0x1F,0xAF);
	FillRectSize(fb, 0, 25, 95, 20, (PanelState==Overview ? active : inactive));
	FillRectSize(fb, 95, 25, 70, 20, (PanelState==DoubleEmotion ? active : inactive));
	FillRectSize(fb, 95+70, 25, 85, 20, (PanelState==ChestConfig ? active : inactive));
	FillRectSize(fb, 95+70+85, 25, 85, 20, (PanelState==AudioPipe ? active : inactive));
	FillRectSize(fb, 95+70+85+85, 25, 65, 20, (PanelState==Terminal ? active : inactive));
	DrawText(fb, " Status  Full  Chest  Audio  Log", 2, 25+2, PIXEL(0xFF,0xFF,0xFF,0xFF), PIXEL(0x00,0x00,0x00,0x00), 2);
	

	//Render Left Panel
	if (PanelState==Terminal)
	{
		FillRect(fb, 0, 45, FBWIDTH/2, FBHEIGHT, PIXEL(0x1F,0x1F,0x1F,0xAF));
		int c=0;
		for (u8 i=LogBufferBegin; c<LOGLEN; i=(i+1)%LOGLEN,c=c+1)
		{
			DrawText(fb, LogBuffer[i], 4, 45+4+18*(LOGLEN-1-c), PIXEL(0xFF,0xFF,0xFF,0xFF), PIXEL(0x00,0x00,0x00,0x00), 2);
		}
	}

	//Render Right Panel
	for (u16 i=0; i<5; i++)
	{
		FillRectSize(fb, FBWIDTH/2, 30+65*i, FBWIDTH/2, 60, PIXEL(0x1F,0x1F,0x1F,0xAF));
	}

}

#define LOGDEC LogBufferBegin=MOD((int)LogBufferBegin-1,LOGLEN)

void Log(const char *format, ...)
{
	char buffer[256];
	va_list valist;
	va_start(valist, format);

	vsnprintf(buffer, 256, format, valist);

	va_end(valist);

	buffer[255]=0;
	//printf(buffer);


	LOGDEC;
	//LogBufferBegin=LogBufferBegin%LOGLEN;
	u8 len=0;
	for(u8 i=0; buffer[i]; i++)
	{
		LogBuffer[LogBufferBegin][len]=buffer[i];
		len++;
		if (len==30) { LogBuffer[LogBufferBegin][31]=0; len=0; LOGDEC; }
		if (buffer[i]=='\n') { LogBuffer[LogBufferBegin][len]=0; len=0; LOGDEC; }
		
	}
}

