

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <sys/types.h>
#include <errno.h>


#define MIN(a,b) ((a)<=(b)?(a):(b))
#define MAX(a,b) ((a)>=(b)?(a):(b))

#define ARRACC(buff,x,y,width,size,type) *(type)((buff)+(y)*(width)*(size)+(x)*(size))


typedef u_int8_t u8;
typedef u_int16_t u16;
typedef u_int32_t u32;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;


int kbhit();
u8 getch();

void set_conio_terminal_mode();
void reset_terminal_mode();