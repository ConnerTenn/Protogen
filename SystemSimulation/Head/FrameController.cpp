
#include "Devices.h"
#include "Messaging.h"
#include "Frames.h"

//ExpressionState ExprState;

u8 Temp32x8[32*8];

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

void ApplyMask(u8 *frame, u8 *mask, u8 *dest, u8 width, u8 height)
{

}

void TransformFrame(u8 *frame, u8 *dest, u8 width, u8 height, int dx, int dy)
{
	for (u8 y=0; y<height; y++)
	{
		for (u8 x=0; x<width/8; x++)
		{
			// u16 srcIdx=(y+dy)*(width/8)+(x+dx)/8;
			// u8 mask = 0xFF;
		}
	}
}

void UpdateDisplay(u16 *frameIdx, u8 width, u8 height, int dx, int dy)
{
	LOGENTERFUNC
	Frame *frame = &((Frame *)FrameData)[*frameIdx];
	PrintDisplay((u8 *)FrameData + frame->DataOffset, width, height, dx, dy);
	frame->DelayCounter++;
	if (frame->DelayCounter >= frame->Delay) 
	{
		*frameIdx = frame->Next;
		frame->DelayCounter=0;
	}
	//PrintDisplay(
	LOGRETFUNC
}

void UpdateDisplays()
{
	LOCKMUTEX(&TermLock);

	UpdateDisplay(&ExprState.LeftEye, 16, 8, 100+16, 0);
	UpdateDisplay(&ExprState.RightEye, 16, 8, 100+16*2+8+2, 0);
	UpdateDisplay(&ExprState.Nose, 8, 8, 100+16+8, 8+1);
	UpdateDisplay(&ExprState.Nose, 8, 8, 100+16+16+8+2, 8+1);
	UpdateDisplay(&ExprState.LeftMouth, 32, 8, 100, (8+1)*2);
	UpdateDisplay(&ExprState.RightMouth, 32, 8, 100+32+8+2, (8+1)*2);
	UpdateDisplay(&ExprState.CenterMouth, 8, 8, 100+32+1, (8+1)*2+1);
	UpdateDisplay(&ExprState.Body, 8, 8, 100, (8+1)*3+4);
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

