
#include "Global.h"

#include "Devices.h"
#include "Messaging.h"


void *EmotionControllerEntry(void *data)
{
	PRINTENTERFUNC
	Device *device = (Device *)data; (void)device;

	GPIO *gpio = GPIO::Open(device->Open(211));

	Serial *serial = Serial::Open(device->Open(221));

	MessageHandler messenger;
	InitMessageHandler(&messenger, serial);

	while (Run)
	{
		int logic = (int)GPIO::Read(gpio);

		pthread_mutex_lock(&TermLock);

		PRINT("Emote GPIO:%d\n", logic);

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

	GPIO::Close(gpio);
	device->Close(gpio);

	PRINTRETFUNC
	return 0;
}

void *HeadControllerEntry(void *data)
{
	PRINTENTERFUNC
	Device *device = (Device *)data; (void)device;

	GPIO *gpio = GPIO::Open(device->Open(111));
	GPIO::SetDirection(gpio, GPIO::Output);

	Serial *serial = Serial::Open(device->Open(121));

	MessageHandler messenger;
	InitMessageHandler(&messenger, serial);

	bool logic=0;
	while (Run)
	{
		GPIO::Set(gpio, logic?GPIO::High:GPIO::Low);


		pthread_mutex_lock(&TermLock);

		Message msg;
		strcpy(msg.Dest,"Emote");
		strcpy(msg.Label,"Test");
		static int cc=0;
		sprintf((char *)msg.Content,"Works %d", cc++);
		SendMessage(&messenger, &msg);

		PRINT("Status sent:: GPIO:%d\n", logic);
		PRINT("Msg: "); PrintMessage(&msg);

		PRINT("\n");

		pthread_mutex_unlock(&TermLock);

		logic=!logic;
		usleep(1000*1000);
	}

	DestroyMessageHandler(&messenger);

	Serial::Close(serial);
	device->Close(serial);

	GPIO::Close(gpio);
	device->Close(gpio);

	PRINTRETFUNC
	return 0;
}


