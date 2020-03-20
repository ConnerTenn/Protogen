
#include "globals.h"


//Page 135
void IntiSPI();
void SPITransmit(const u8 data);
void SPITransmit16(const u16 data);

// #define SPI_TRANSMIT(data) 


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




void Max7219Init(u8 numSegments, u8 cs);
void Max7219Refresh(u8 numSegments, u8 cs);
void Max7219SendCmd(u16 cmd, u8 numSegments, u8 cs);
// void Max7219Send4Frame(u8 *data, u8 cs);
// void Max7219Send2Frame(u8 *data, u8 cs);
// void Max7219Send1Frame(u8 *data, u8 cs);

void Max7219SendData(u8 **data, u8 *numSegments, u8 numDisplays, u8 cs);
void Max7219SendFrames(u16 *indexes, u8 *numSegments, u8 numDisplays, u8 cs);
