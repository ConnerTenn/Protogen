
extern "C"
{
#include <execinfo.h>
#include "Global.h"
}
#include "Devices.h"

bool Run = true;
sem_t RunSem;
pthread_mutex_t LogLock;
FILE *LogFile;


void InteruptHandler(int arg) { LOG(YELLOW "Stopping\n" RESET); Run=false; sem_post(&RunSem); }
void CrashHandler(int arg) 
{
	ERROR("Segfault");
	void *array[10];
	size_t size;
	char **strings;
	size_t i;

	size = backtrace (array, 10);
	strings = backtrace_symbols (array, size);

	printf ("Obtained %zd stack frames.\n", size);

	for (i = 0; i < size; i++) { printf ("%s\n", strings[i]); }

	free(strings);
	exit(1);
}

void Init()
{
	signal(SIGINT, InteruptHandler); signal(SIGKILL, InteruptHandler);
	signal(SIGSEGV, CrashHandler); signal(SIGKILL, InteruptHandler);

	LogFile = fopen("log", "w");
	LOG("\n\n\n");
	LOGENTERFUNC

	set_conio_terminal_mode();

	pthread_mutex_init(&TermLock, 0);
	pthread_mutex_init(&LogLock, 0);

	u16 dimx, dimy;
	GetDimensions(&dimx, &dimy);
	for (int y=0; y<dimy; y++)
	{
		for (int x=0; x<dimx; x++) { PRINT(" "); }
		PRINT("\n");
	}
	
	LOG("Dimensions: %d, %d\n", (int)dimx, (int)dimy);

	InitDevices();
	LOGRETFUNC
}

void Close()
{
	LOGENTERFUNC
	DestroyDevices();

	pthread_mutex_destroy(&TermLock);
	pthread_mutex_destroy(&LogLock);
	
	u16 dimx, dimy;
	GetDimensions(&dimx, &dimy);
	MOVETO(0, (int)dimy);
	PRINT("\n");

	LOGRETFUNC
	fclose(LogFile);
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

