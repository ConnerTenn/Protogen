
#include "Global.h"


void *EmotionControllerEntry(void *data)
{
	PRINTENTERFUNC

	//Serial_t serial = Serial::Open(5);

	while (Run)
	{
		pthread_mutex_lock(&TermLock);
		PRINT("Emote\n");
		pthread_mutex_unlock(&TermLock);
		usleep(1000*1000);
	}

	//Serial::Close(serial);

	PRINTRETFUNC
	return 0;
}

void *HeadControllerEntry(void *data)
{
	PRINTENTERFUNC

	//Serial_t serial = Serial::Open(5);
	
	while (Run)
	{
		pthread_mutex_lock(&TermLock);
		PRINT("Status\n");
		pthread_mutex_unlock(&TermLock);
		usleep(1000*1000);
	}

	//Serial::Close(serial);

	PRINTRETFUNC
	return 0;
}


