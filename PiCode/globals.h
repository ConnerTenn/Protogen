
#ifndef _GLOBALS_H_
#define _GLOBALS_H_


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <sys/types.h>
#include <errno.h>

#include <math.h>


#define MIN(a,b) ((a)<=(b)?(a):(b))
#define MAX(a,b) ((a)>=(b)?(a):(b))
#define ABS(a) ((a)<0?-(a):(a))
#define PI M_PIl
#define TAU (2.0*PI)
#define MOD(a,b) ((a)%(b)+((a)<0?(b):0))
#define CEILDIV(a,b) ( (((long int)(a))/((long int)(b))) + (((long int)(a))%((long int)(b))?1:0))

#define ARRACC(buff,x,y,width,size,type) *(type)((buff)+(y)*(width)*(size)+(x)*(size))


typedef u_int8_t u8;
typedef u_int16_t u16;
typedef u_int32_t u32;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

union Ksequ
{
	u8 seq[8];
	u32 val;
};

int kbhit();
u8 getch();

void set_conio_terminal_mode();
void reset_terminal_mode();

#endif 
