
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

#define PRINTENTERFUNC PRINTF(INV "\n>> %s\n" RESET, __FUNCTION__);
#define PRINTRETFUNC PRINTF(INV GREEN "\n<< %s\n" RESET, __FUNCTION__);
#define PRINTRETFAILFUNC PRINTF(INV RED "\n<< %s\n" RESET, __FUNCTION__);

#define ARRAYLEN(arr) (sizeof(arr)/sizeof(*(arr)))

extern bool Run;

extern pthread_mutex_t TermLock;

//#endif
