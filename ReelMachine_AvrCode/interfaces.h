
#include "globals.h"
#include <avr/interrupt.h>

//http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf

#define SETBITS(r,v,m) ((r)=((r) & ~(m)) | ((v) & (m)))
#define ENBITS(r,m) ((r)=((r) | (m)))
#define DABITS(r,m) ((r)=((r) & ~(m)))

//Page 135
void IntiSPI();
void SPITransmit(const u8 data);
void SPITransmit16(const u16 data);

#define DDRB_MOSI 3
#define DDRB_SCK 5
#define DDRB_CS 2

#define REFRESH_INTERVAL 300
u16 RefreshTimer;

//Page 143
void IntiUART();
void SerialTransmitByte(u8 data);
//void SerialTransmit(u8 *data, u8 len);
void SerialFlush();
void SerialTransmit(u8 *data, u8 len);
void SerialTransmitStr(char *data);

u8 SerialRead(u8 *data, u8 len);
u8 SerialAvail();

