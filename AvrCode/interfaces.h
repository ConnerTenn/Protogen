
#include "globals.h"
#include <avr/interrupt.h>

//http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf

#define SETBITS(r,m,v) ((r)=((r) & ~(m)) | ((m) & (v)))
#define ENBITS(r,m) ((r)=((r) | (m)))
#define DABITS(r,m) ((r)=((r) & ~(m)))

//Page 135
void IntiSPI();
void SPITransmit(u8 data);
void SPITransmit16(u16 data);

#define DDRB_MOSI 3
#define DDRB_SCK 5
#define DDRB_CS 2


//Page 143
void IntiUART();
void SerialTransmitByte(u8 data);
//void SerialTransmit(u8 *data, u8 len);
void SerialFlush();
void SerialTransmit(u8 *data, u8 len);
void SerialTransmitStr(char *data);

u8 SerialRead(u8 *data, u8 len);
u8 SerialAvail();


void Max7219Init(u8 numDisplays);
void Max7219SendCmd(u16 cmd, u8 numDisplays);
void Max7219SendFrame(u8 *data, u8 numDisplays);


