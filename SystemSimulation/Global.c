
#include "Global.h"

i64 GetMilliseconds()
{
	struct timeval now;
	gettimeofday(&now, 0);
	return now.tv_sec*1000 + now.tv_usec/1000;
}

