
#include "Global.h"

#include "Devices.h"
#include "Messaging.h"


void *HeadControllerEntry(void *data)
{
	LOGENTERFUNC
	Device *device = (Device *)data; (void)device;

	Serial *serial = Serial::Open(device->Open(111));

	MessageHandler messenger;
	InitMessageHandler(&messenger, serial);

	while (Run)
	{
		pthread_mutex_lock(&LogLock);
		Message msg; bool print=false;
		while(GetNextMessage(&messenger, &msg))
		{
			LOG("%s:: ",__FUNCTION__); LogMessage(&msg); LOGF("\n");
		}

		if (print) { LOGF("\n\n"); }
		pthread_mutex_unlock(&LogLock);



		strcpy(msg.Dest,"Emote");
		strcpy(msg.Label,"Test");
		static int cc=0;
		sprintf((char *)msg.Content,"Works %d", cc++);
		SendMessage(&messenger, &msg);

		//LOGF("\n");

		usleep(1000*1000);
	}

	DestroyMessageHandler(&messenger);

	Serial::Close(serial);
	device->Close(serial);

	LOGRETFUNC
	return 0;
}


