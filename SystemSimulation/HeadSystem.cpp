
#include "Global.h"

#include "Devices.h"
#include "Messaging.h"


void *EmotionControllerEntry(void *data)
{
	PRINTENTERFUNC
	Device *device = (Device *)data; (void)device;

	Serial *serial = Serial::Open(device->Open(211));

	MessageHandler messenger;
	InitMessageHandler(&messenger, serial);

	while (Run)
	{
		pthread_mutex_lock(&TermLock);
		PRINT("%s Update\n", __FUNCTION__);

		Message msg;
		while(GetNextMessage(&messenger, &msg))
		{
			PrintMessage(&msg);
		}

		PRINT("\n\n");
		pthread_mutex_unlock(&TermLock);

		usleep(1000*1000);
	}

	DestroyMessageHandler(&messenger);

	Serial::Close(serial);
	device->Close(serial);

	PRINTRETFUNC
	return 0;
}

void *HeadControllerEntry(void *data)
{
	PRINTENTERFUNC
	Device *device = (Device *)data; (void)device;

	Serial *serial = Serial::Open(device->Open(111));

	MessageHandler messenger;
	InitMessageHandler(&messenger, serial);

	while (Run)
	{
		pthread_mutex_lock(&TermLock);
		PRINT("%s Update\n", __FUNCTION__);

		Message msg;
		while(GetNextMessage(&messenger, &msg))
		{
			PrintMessage(&msg);
		}


		strcpy(msg.Dest,"Emote");
		strcpy(msg.Label,"Test");
		static int cc=0;
		sprintf((char *)msg.Content,"Works %d", cc++);
		SendMessage(&messenger, &msg);

		PRINT("\n");
		pthread_mutex_unlock(&TermLock);

		usleep(1000*1000);
	}

	DestroyMessageHandler(&messenger);

	Serial::Close(serial);
	device->Close(serial);

	PRINTRETFUNC
	return 0;
}


