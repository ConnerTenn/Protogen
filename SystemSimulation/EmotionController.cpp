
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
			PRINT("%s ",(pixel?BWHITE:BBLACK));
		}	
	}
	PRINT("\n");
}

void UpdateDisplays()
{
	LOCKMUTEX(&TermLock);
	PrintDisplay(Frames[ExprState.LeftEye].Data, 16, 8, 80+16, 0);
	PrintDisplay(Frames[ExprState.RightEye].Data, 16, 8, 80+16*2+8+2, 0);
	PrintDisplay(Frames[ExprState.Nose].Data, 8, 8, 80+16+8, 8+1);
	PrintDisplay(Frames[ExprState.Nose].Data, 8, 8, 80+16+16+8+2, 8+1);
	PrintDisplay(Frames[ExprState.LeftMouth].Data, 32, 8, 80, (8+1)*2);
	PrintDisplay(Frames[ExprState.RightMouth].Data, 32, 8, 80+32+8+2, (8+1)*2);
	PrintDisplay(Frames[ExprState.CenterMouth].Data, 8, 8, 80+32+1, (8+1)*2+1);
	PrintDisplay(Frames[ExprState.Body].Data, 8, 8, 80, (8+1)*3+4);
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

	memset(&ExprState, 0, sizeof(ExprState));

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

