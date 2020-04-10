
#include "globals.h"

//http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf

#define SETBITS(r,v,m) ( (r)=((r) & ~(m)) | ((m) & (v)) )
#define ENBITS(r,m) ( (r)=((r) | (m)) )
#define DABITS(r,m) ( (r)=((r) & ~(m)) )


//Page 143
void IntiUART();
void SerialTransmitByte(u8 data);
//void SerialTransmit(u8 *data, u8 len);
void SerialFlush();
void SerialTransmit(u8 *data, u8 len);
void SerialTransmitStr(char *data);
void SerialTransmitHexVal(u16 val);

u8 SerialRead(u8 *data, u8 len);
u8 SerialAvail();

