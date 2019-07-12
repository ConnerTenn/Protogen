
#include "General.h"


bool Run = true;
pthread_mutex_t TermLock;
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

	PRINTRETFUNC

	return 0;
}

