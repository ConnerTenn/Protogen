
#ifndef _INTERFACES_H_
#define _INTERFACES_H_

#include "globals.h"
#include "frames.h"


//Page 135
void IntiSPI();
// void SPITransmit(const u8 data);
// void SPITransmit16(const u16 data);

#define SPI_TRANSMIT8_SERCOM4(data) ( \
	SERCOM4->SPI.DATA.reg = (data), /*Data Out*/ \
	({ while (!SERCOM4->SPI.INTFLAG.bit.TXC) {} /*Wait for transmit complete*/ }) \
	)

#define SPI_TRANSMIT16_SERCOM4(data) ( \
	SPI_TRANSMIT8_SERCOM4(((data)>>8)&0xFF), \
	SPI_TRANSMIT8_SERCOM4((data)&0xFF) \
	)

#define SPI_TRANSMIT8_SERCOM1(data) ( \
	SERCOM1->SPI.DATA.reg = (data), /*Data Out*/ \
	({ while (!SERCOM1->SPI.INTFLAG.bit.TXC) {} /*Wait for transmit complete*/ }) \
	)

#define SPI_TRANSMIT16_SERCOM1(data) ( \
	SPI_TRANSMIT8_SERCOM1(((data)>>8)&0xFF), \
	SPI_TRANSMIT8_SERCOM1((data)&0xFF) \
	)

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
u8 SerialAvail();


typedef struct
{
	u8 NumSegments;
	u16 FrameIndex;
	u16 FrameDelay;
} Max7219;

void Max7219InitCOM1(u8 numSegments);
void Max7219InitCOM4(u8 numSegments);

void Max7219RefreshCOM1(u8 numSegments);
void Max7219RefreshCOM4(u8 numSegments);

void Max7219SendCmdCOM1(u16 cmd, u8 numSegments);
void Max7219SendCmdCOM4(u16 cmd, u8 numSegments);

void Max7219SendFramesCOM1(Max7219 *displays, u8 numDisplays);
void Max7219SendFramesCOM4(Max7219 *displays, u8 numDisplays);

#endif
