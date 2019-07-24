
#include "EmotionController.h"

ExpressionState ExprState;

void PrintDisplay(u8 *display, u8 width, u8 height, int dx, int dy)
{
	for (u8 y=0; y<height; y++)
	{
		MOVETO(dx, dy+y);
		for (u8 x=0; x<width; x++)
		{
			u16 idx=y*(width/8)+(x/8);
			u8 bit = x%8;
			u8 mask = 0x80>>bit;
			u8 pixel = (display[idx] & mask)>>(7-bit);
			PRINT("%s  ",(pixel?BWHITE:BBLACK));
		}	
	}
	PRINT("\n");
}

void UpdateDisplays()
{
	LOCKMUTEX(&TermLock);
	PrintDisplay(Frames[ExprState.LeftEye].Data, 8, 8, 100, 0);
	PRINT(RESET);
	fflush(stdout);
	ULOCKMUTEX(&TermLock);
}

void *EmotionControllerEntry(void *data)
{
	LOGENTERFUNC
	Device *device = (Device *)data; (void)device;

	Serial *serial = Serial::Open(device->Open(211));

	MessageHandler messenger;
	InitMessageHandler(&messenger, serial);

	//InitDisplay(&Display1, 4, 4);
	//memcpy(Display1.Array, DispData, ARRAYLEN(DispData));

	while (Run)
	{
		LOCKMUTEX(&LogLock);
		Message msg; bool print=false;
		while(GetNextMessage(&messenger, &msg))
		{
			LOG("%s:: ",__FUNCTION__); LogMessage(&msg);

			if (strcmp(msg.Dest,"Emote")==0 && strcmp(msg.Label,"Update")==0)
			{
				ExprState=*((ExpressionState *)msg.Content);
			}
			print=true;
		}

		if (print) { LOGF("\n\n"); }
		ULOCKMUTEX(&LogLock);


		UpdateDisplays();

		usleep(50*1000);
	}

	//DestroyDisplay(&Display1);

	DestroyMessageHandler(&messenger);

	Serial::Close(serial);
	device->Close(serial);

	LOGRETFUNC
	return 0;
}

