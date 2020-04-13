
#include "sam.h"
#include <string.h>

#define CEILDIV(n,d) ( ((int)(n)) / ((int)(d)) + ( ((int)(n))%((int)(d)) ? 1 : 0 ) )

#define SETBITS(r,v,m) ( (r)=((r) & ~(m)) | ((m) & (v)) )
#define ENBITS(r,m) ( (r)=((r) | (m)) )
#define DABITS(r,m) ( (r)=((r) & ~(m)) )
#define REG(t, r) ( *(t *)(r) )
#define SETREG(t, r, v, m)  ( SETBITS(REG(t, r), v, m) )

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
//typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
//typedef int64_t i64;


// #define CHARLIE
#define JESS


