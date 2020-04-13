
#ifndef _INTERFACES_H_
#define _INTERFACES_H_

#include "globals.h"
#include "frames.h"


//Page 135
void IntiSPI();
#ifdef CHARLIE
void SPITransmitCOM1(const u8 data);
void SPITransmit16COM1(const u16 data);
#endif
void SPITransmitCOM4(const u8 data);
void SPITransmit16COM4(const u16 data);

// #define SPI_TRANSMIT(data) 


#define REFRESH_INTERVAL 300
// u16 RefreshTimer;

//Page 143
void IntiUART();
void SerialTransmitByte(u8 data);
//void SerialTransmit(u8 *data, u8 len);
void SerialFlush();
void SerialTransmit(u8 *data, u8 len);
void SerialTransmitStr(char *data);

u8 SerialRead(u8 *data, u8 len);
u8 SerialGetCh();
u8 SerialAvail();


typedef struct
{
	u8 NumSegments;
	u16 FrameIndex;
	u16 FrameDelay;
	u16 QueuedIndex;
} Max7219;

#ifdef CHARLIE
void Max7219InitCOM1(u8 numSegments);
void Max7219RefreshCOM1(u8 numSegments);
void Max7219SendCmdCOM1(u16 cmd, u8 numSegments);
void Max7219SendFramesCOM1(Max7219 *displays, u8 numDisplays);
#endif

void Max7219InitCOM4(u8 numSegments);
void Max7219RefreshCOM4(u8 numSegments);
void Max7219SendCmdCOM4(u16 cmd, u8 numSegments);
void Max7219SendFramesCOM4(Max7219 *displays, u8 numDisplays);

// void Max7219SendFrames(Max7219 *displaysCom1, u8 numDisplaysCom1, Max7219 *displaysCom4, u8 numDisplaysCom4);

#endif
