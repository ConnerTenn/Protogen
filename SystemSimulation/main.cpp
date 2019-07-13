
#include "Global.h"
#include "Devices.h"


bool Run = true;
sem_t RunSem;



void InteruptHandler(int arg) { printf("\n"); Run=false; sem_post(&RunSem); }

void Init()
{
	signal(SIGINT, InteruptHandler); signal(SIGKILL, InteruptHandler);

	pthread_mutex_init(&TermLock, 0);

	InitDevices();
}

void Close()
{
	DestroyDevices();

	pthread_mutex_destroy(&TermLock);
}

int main()
{
	PRINTENTERFUNC

	Init();

	sem_wait(&RunSem);
	
	Close();

	PRINTRETFUNC
	return 0;
}

