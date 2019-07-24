
#include "Global.h"

#include "Devices.h"
#include "Messaging.h"
#include "UI.h"


u8 Battery = 213;
float UpdateFreq;

void *HeadControllerEntry(void *data)
{
	LOGENTERFUNC
	Device *device = (Device *)data; (void)device;

	Serial *serial = Serial::Open(device->Open(111));

	MessageHandler messenger;
	InitMessageHandler(&messenger, serial);
	
	InitUI();
	
	i64 tp = 0, t1 = 0, td = 0;
	while (Run)
	{
		tp = t1;
		t1 = GetMilliseconds();
		td = t1 - tp;
		UpdateFreq = 1000.0/td;
		
		
		LOCKMUTEX(&LogLock);

		Message msg; bool print=false;
		while(GetNextMessage(&messenger, &msg))
		{
			LOG("%s:: ",__FUNCTION__); LogMessage(&msg); LOGF("\n");
		}
		
		while(kbhit() && Run)
		{
			u8 ch;// = getch();
			Ksequ sequ = {.val=0};
			u8 seqc=0;
			while(kbhit()) { sequ.seq[seqc++]=getch();}sequ.seq[seqc]=0;
			ch=sequ.seq[0];
			
			LOGF("PRESS: %d(%X) %llu(%016llX)\n", ch, ch, sequ.val, sequ.val);
			
			UIHandleInput(sequ, &messenger);
		}

		if (print) { LOGF("\n\n"); }
		ULOCKMUTEX(&LogLock);
		
		LOCKMUTEX(&TermLock);
		UpdateUI();
		ULOCKMUTEX(&TermLock);
		

		usleep(50*1000);
	}
	
	DestroyUI();

	DestroyMessageHandler(&messenger);

	Serial::Close(serial);
	device->Close(serial);

	LOGRETFUNC
	return 0;
}


