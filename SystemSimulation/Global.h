
//#ifndef _GLOBAL_H_
//#define _GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <pthread.h>
#include <semaphore.h>

//#include <vector>
#include <string.h>

//#include <General.h>
//#include <Colour.h>
#include "TextScreen.h"
//#include "PseudoInterface.h"
//#include "Devices.h"

#define MIN(a,b) ((a)<=(b)?(a):(b))
#define MAX(a,b) ((a)>=(b)?(a):(b))
#define ABS(a) ((a)<0?-(a):(a))
#define PI M_PIl
#define TAU (2.0*PI)
#define MOD(a,b) ((a)%(b)+((a)<0?(b):0))
#define CEILDIV(a,b) ( (((long int)(a))/((long int)(b))) + (((long int)(a))%((long int)(b))?1:0))


#define ARRAYLEN(arr) (sizeof(arr)/sizeof(*(arr)))

#define LOCKMUTEX(m) pthread_mutex_lock(m)
#define ULOCKMUTEX(m) pthread_mutex_unlock(m)

#define LOG(...) fprintf(LogFile, __VA_ARGS__)
#define LOGF(...) fprintf(LogFile, __VA_ARGS__); fflush(LogFile);
#define LOGERROR(...)  LOGF(RED "ERROR: " RESET __VA_ARGS__)

#define PRINTENTERFUNC PRINTF(INV "\n>> %s\n" RESET, __FUNCTION__);
#define PRINTRETFUNC PRINTF(INV GREEN "\n<< %s\n" RESET, __FUNCTION__);
#define PRINTRETFAILFUNC PRINTF(INV RED "\n<< %s\n" RESET, __FUNCTION__);

#define LOGENTERFUNC LOGF(INV "\n>> %s\n" RESET, __FUNCTION__);
#define LOGRETFUNC LOGF(INV GREEN "\n<< %s\n" RESET, __FUNCTION__);
#define LOGRETFAILFUNC LOGF(INV RED "\n<< %s\n" RESET, __FUNCTION__);


typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;


extern bool Run;

extern pthread_mutex_t TermLock;

extern FILE *LogFile;
extern pthread_mutex_t LogLock;

i64 GetMilliseconds();

//#endif
