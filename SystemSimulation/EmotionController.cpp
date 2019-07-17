
#include "EmotionController.h"


//Display Display1;

#define _ "\x0"
#define $ "\x1"
const char *DispData[] =
{
$ $ $ _ \
$ _ _ _ \
$ _ _ _ \
_ _ _ _
,
$ $ $ $ \
_ _ _ _ \
_ _ _ _ \
_ _ _ _
,
_ $ $ $ \
_ _ _ $ \
_ _ _ $ \
_ _ _ _
,
_ _ _ $ \
_ _ _ $ \
_ _ _ $ \
_ _ _ $
,
_ _ _ _ \
_ _ _ $ \
_ _ _ $ \
_ $ $ $
,
_ _ _ _ \
_ _ _ _ \
_ _ _ _ \
$ $ $ $
,
_ _ _ _ \
$ _ _ _ \
$ _ _ _ \
$ $ $ _
,
$ _ _ _ \
$ _ _ _ \
$ _ _ _ \
$ _ _ _
};
#undef _
#undef $

void UpdateDisplays()
{
	LOCKMUTEX(&TermLock);
	//PrintDisplay(&Display1, 0, 1);
	static u64 i=0; i++;
	PrintDisplay((u8 *)DispData[i%8], 4, 4, 100, 0);
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

