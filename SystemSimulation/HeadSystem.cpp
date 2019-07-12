
#include "General.h"


void *EmotionController(void *data)
{
	PRINTENTERFUNC

	while (Run)
	{
		pthread_mutex_lock(&TermLock);
		PRINT("Emote\n");
		pthread_mutex_unlock(&TermLock);
		usleep(1000*1000);
	}

	PRINTRETFUNC
	return 0;
}

void *HeadController(void *data)
{
	PRINTENTERFUNC
	
	while (Run)
	{
		pthread_mutex_lock(&TermLock);
		PRINT("Status\n");
		pthread_mutex_unlock(&TermLock);
		usleep(1000*1000);
	}

	PRINTRETFUNC
	return 0;
}


