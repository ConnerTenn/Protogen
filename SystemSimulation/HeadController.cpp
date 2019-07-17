
#include "Global.h"

#include "Devices.h"
#include "Messaging.h"

u8 Battery = 64;

void HandleUI()
{
	while(kbhit() && Run)
	{
		int ch = getch();
		LOGF("PRESS: %d(%X)\n", ch, ch);
	}

	FillCharacers(0,0,100,1,{' ',BGREY});
	FillCharacers(0,59,100,1,{' ',BGREY});
	FillCharacers(0,0,1,60,{' ',BGREY});
	FillCharacers(99,0,1,60,{' ',BGREY});

	MOVETO(0,1);
}

void *HeadControllerEntry(void *data)
{
	LOGENTERFUNC
	Device *device = (Device *)data; (void)device;

	Serial *serial = Serial::Open(device->Open(111));

	MessageHandler messenger;
	InitMessageHandler(&messenger, serial);

	while (Run)
	{
		LOCKMUTEX(&LogLock);
		HandleUI();

		Message msg; bool print=false;
		while(GetNextMessage(&messenger, &msg))
		{
			LOG("%s:: ",__FUNCTION__); LogMessage(&msg); LOGF("\n");
		}

		if (print) { LOGF("\n\n"); }
		ULOCKMUTEX(&LogLock);



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


