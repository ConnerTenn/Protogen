
#include "Global.h"

#include "Devices.h"


void *EmotionControllerEntry(void *data)
{
	PRINTENTERFUNC
	Device *device = (Device *)data; (void)device;

	GPIO *gpio = GPIO::Open(device->Open(211));

	Serial *serial = Serial::Open(device->Open(221));

	char buff[32];
	while (Run)
	{
		int logic = (int)GPIO::Read(gpio);
		if (Serial::Available(serial))
		{
			Serial::Read(serial, buff, 32);
		}

		pthread_mutex_lock(&TermLock);
		PRINT("Emote GPIO:%d  Serial:\"%s\"\n\n", logic, buff);
		pthread_mutex_unlock(&TermLock);
		strcpy(buff, "");
		usleep(1000*1000);
	}

	GPIO::Close(gpio);
	device->Close(gpio);

	Serial::Close(serial);
	device->Close(serial);

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

	bool logic=0;
	char buff[32] = "Hello!";
	while (Run)
	{
		GPIO::Set(gpio, logic?GPIO::High:GPIO::Low);
		Serial::Write(serial, buff, strlen(buff));

		pthread_mutex_lock(&TermLock);
		PRINT("Status sent:: GPIO:%d  Serial:\"%s\"\n\n", logic, buff);
		pthread_mutex_unlock(&TermLock);
		logic=!logic;
		usleep(1000*1000);
	}

	GPIO::Close(gpio);
	device->Close(gpio);

	Serial::Close(serial);
	device->Close(serial);

	PRINTRETFUNC
	return 0;
}


