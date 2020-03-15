

//#include "SERCOM.h"
#include "SPI.h"
#include "wiring_private.h" // pinPeripheral() function

#define REG(t, r) (*(t *)(r))

typedef unsigned char u8;
typedef unsigned long u32;

SPIClass mySPI (&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);

int main()
{
	REG(volatile u32, 0x41004400+0x00) = (1<<15);

	REG(volatile u32, 0x41004400+0x10) = (1<<15); 
	REG(volatile u32, 0x41004400+0x10) = (0<<15); 

	mySPI.begin();

	pinPeripheral(11, PIO_SERCOM);
	pinPeripheral(12, PIO_SERCOM);
	pinPeripheral(13, PIO_SERCOM);


	u8 i=0; (void)i;
	while (1)
	{
		REG(volatile u32, 0x41004400+0x10) = (1<<15); 
		REG(volatile u32, 0x41004400+0x10) = (0<<15); 

		mySPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
		mySPI.transfer(i++);
		mySPI.endTransaction();
	}

	return 0;
}

