
#include "Global.h"


bool Run = true;
sem_t RunSem;

struct Thread
{
	pthread_t ID;
	void *(* Function)(void *);
};

void *HeadController(void *data);
void *EmotionController(void *data);



#define NUMTHREADS 2
Thread ThreadList[NUMTHREADS] = 
{
	{ 0, HeadController },
	{ 0, EmotionController },
};


void InteruptHandler(int arg) { printf("\n"); Run=false; sem_post(&RunSem); }

int main()
{
	PRINTENTERFUNC
	signal(SIGINT, InteruptHandler); signal(SIGKILL, InteruptHandler);

	pthread_mutex_init(&TermLock, 0);
	Serial::InitSerial();

	//pthread_t headCtlThread;
	//pthread_create(&headCtlThread, 0, HeadController, &Run);
	for (int i = 0; i < NUMTHREADS; i++)
	{
		pthread_create(&(ThreadList[i].ID), 0, ThreadList[i].Function, &Run);
	}


	sem_wait(&RunSem);
	
	for (int i = 0; i < NUMTHREADS; i++)
	{
		pthread_join(ThreadList[i].ID, 0);
	}

	pthread_mutex_destroy(&TermLock);
	Serial::CloseSerial();

	PRINTRETFUNC
	return 0;
}

