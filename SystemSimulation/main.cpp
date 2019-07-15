
#include <execinfo.h>
#include "Global.h"
#include "Devices.h"

bool Run = true;
sem_t RunSem;



void InteruptHandler(int arg) { printf(YELLOW "Stopping\n" RESET); Run=false; sem_post(&RunSem); }
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

	set_conio_terminal_mode();

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

	//sem_wait(&RunSem);
	while(Run)
	{
		while(kbhit() && Run)
		{
			int ch = getch();
			PRINT("PRESS: %d(%X)\n", ch, ch);
		}
	}
	
	Close();

	PRINTRETFUNC
	return 0;
}

