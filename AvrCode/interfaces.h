
#include "globals.h"
#include <avr/interrupt.h>

#define SETBITS(r,m,v) ((r)=((r) & ~(m)) | ((m) & (v)))
#define ENBITS(r,m) ((r)=((r) | (m)))
#define DABITS(r,m) ((r)=((r) & ~(m)))

void IntiSPI();
void SPITransmit(u8 data);
void SPITransmit16(u16 data);

#define DDR_MOSI 3
#define DDR_SCK 5
#define DRB_CS 2

