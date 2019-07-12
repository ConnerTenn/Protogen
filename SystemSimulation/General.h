

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <pthread.h>
#include <semaphore.h>

#include <General.h>
#include <Colour.h>
#include "TextScreen.h"

extern bool Run;

extern pthread_mutex_t TermLock;


#define PRINTENTERFUNC PRINTF(INV "\n>> %s\n" RESET, __FUNCTION__);
#define PRINTRETFUNC PRINTF(INV GREEN "\n<< %s\n" RESET, __FUNCTION__);
#define PRINTRETFAILFUNC PRINTF(INV RED "\n<< %s\n" RESET, __FUNCTION__);