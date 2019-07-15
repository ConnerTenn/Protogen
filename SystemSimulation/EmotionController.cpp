
#include "EmotionController.h"


Display Display1;

void UpdateDisplays()
{
	pthread_mutex_lock(&TermLock);
	PrintDisplay(&Display1, 0, 1);
	fflush(stdout);
	pthread_mutex_unlock(&TermLock);
}

u8 DispData[] =
{
	0, 1, 1, 1,
	0, 0, 1, 0,
	1, 1, 1, 0,
	0, 0, 1, 0,
};

void *EmotionControllerEntry(void *data)
{
	LOGENTERFUNC
	Device *device = (Device *)data; (void)device;

	Serial *serial = Serial::Open(device->Open(211));

	MessageHandler messenger;
	InitMessageHandler(&messenger, serial);

	InitDisplay(&Display1, 4, 4);
	memcpy(Display1.Array, DispData, ARRAYLEN(DispData));

	while (Run)
	{
		pthread_mutex_lock(&LogLock);
		LOG("%s Update\n", __FUNCTION__);

		Message msg;
		while(GetNextMessage(&messenger, &msg))
		{
			LogMessage(&msg);
		}

		LOGF("\n\n");
		pthread_mutex_unlock(&LogLock);


		UpdateDisplays();

		usleep(1000*1000);
	}

	DestroyDisplay(&Display1);

	DestroyMessageHandler(&messenger);

	Serial::Close(serial);
	device->Close(serial);

	LOGRETFUNC
	return 0;
}

