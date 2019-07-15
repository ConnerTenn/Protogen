
//#ifndef _GLOBAL_H_
//#define _GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <pthread.h>
#include <semaphore.h>

#include <vector>
#include <string>

#include <General.h>
#include <Colour.h>
#include "TextScreen.h"
//#include "PseudoInterface.h"
//#include "Devices.h"

#define ARRAYLEN(arr) (sizeof(arr)/sizeof(*(arr)))

#define LOG(...) fprintf(LogFile, __VA_ARGS__)
#define LOGF(...) fprintf(LogFile, __VA_ARGS__); fflush(LogFile);
#define LOGERROR(...)  LOGF(RED "ERROR: " RESET __VA_ARGS__)

#define PRINTENTERFUNC PRINTF(INV "\n>> %s\n" RESET, __FUNCTION__);
#define PRINTRETFUNC PRINTF(INV GREEN "\n<< %s\n" RESET, __FUNCTION__);
#define PRINTRETFAILFUNC PRINTF(INV RED "\n<< %s\n" RESET, __FUNCTION__);

#define LOGENTERFUNC LOGF(INV "\n>> %s\n" RESET, __FUNCTION__);
#define LOGRETFUNC LOGF(INV GREEN "\n<< %s\n" RESET, __FUNCTION__);
#define LOGRETFAILFUNC LOGF(INV RED "\n<< %s\n" RESET, __FUNCTION__);


extern bool Run;

extern pthread_mutex_t TermLock;

extern FILE *LogFile;
extern pthread_mutex_t LogLock;


//#endif
