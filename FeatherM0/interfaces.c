
#include "interfaces.h"


// ISR(SPI_STC_vect)
// {
// 	//PORTC = !PORTC;
// }

void IntiSPI()
{
	// == SERCOM4 ==

	//Enable SERCOM4 in Power Manager
	PM->APBCMASK.bit.SERCOM4_ = 1;

	//MOSI: DDRB10 SERCOM4 PAD[2]
	//MISO: DDRA12 SERCOM4 PAD[0]
	//SCK:  DDRB11 SERCOM4 PAD[3]

	//DDR
	//MOSI
	PORT->Group[1].DIRSET.reg = PORT_PB10D_SERCOM4_PAD2; //DDRB10:Out 
	//MISO
	PORT->Group[0].DIRCLR.reg = PORT_PA12D_SERCOM4_PAD0; //DDRA12:IN
	//SCK
	PORT->Group[1].DIRSET.reg = PORT_PB11D_SERCOM4_PAD3; //DDRB11:Out
	// //SS
	// PORT->Group[1].DIRSET.reg = 1<<9; //DDRB09:Out

	// PORT->Group[1].OUTSET.reg = (1<<10) | (1<<11);


	//PINMUX
	// SCK (ODD) Peripheral D (SERCOM4 P3), MOSI (EVEN) Peripheral D (SERCOM4 P2)
	PORT->Group[1].PMUX[10/2].reg = PORT_PMUX_PMUXO_D | PORT_PMUX_PMUXE_D;
	// MISO (EVEN) Peripheral D (SERCOM4 P0)
	PORT->Group[0].PMUX[12/2].reg = PORT_PMUX_PMUXE_D;
	// // SS (ODD) Peripheral D (SERCOM4 P1)
	// PORT->Group[1].PMUX[9/2].reg = PORT_PMUX_PMUXO_D;

	//PINCFG
	PORT->Group[1].PINCFG[10].bit.PMUXEN = 1; //MOSI
	PORT->Group[0].PINCFG[12].bit.PMUXEN = 1; //MISO
	PORT->Group[1].PINCFG[11].bit.PMUXEN = 1; //SCK
	// PORT->Group[1].PINCFG[ 9].bit.PMUXEN = 1; //SS
	
	//Put Generic Clock Generator 0 as source for SERCOM4
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_SERCOM4_CORE) | // Generic Clock Multiplexer SERCOM4
						GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is source
						GCLK_CLKCTRL_CLKEN;
	//Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) { }

	SERCOM4->SPI.CTRLA.bit.SWRST = 1;
	//Wait for synchronization
	while (SERCOM4->SPI.SYNCBUSY.bit.SWRST) {}

	SERCOM4->SPI.CTRLA.reg = 
		SERCOM_SPI_CTRLA_MODE_SPI_MASTER | //Set Master Mode
		SERCOM_SPI_CTRLA_CPOL | SERCOM_SPI_CTRLA_CPHA | //Mode 3: CLK Idle High, sample rising
		SERCOM_SPI_CTRLA_FORM(0) | //SPI Frame Only
		SERCOM_SPI_CTRLA_DIPO(0) | //DIn(MISO) is PAD[0]
		SERCOM_SPI_CTRLA_DOPO(1) | //DOut(MOSI) is PAD[2], SCK is PAD[3]: Mode 1
		SERCOM_SPI_CTRLA_IBON; //STATUS.BUFOVF asserted immediately upon overflow
	//MSB first

	SERCOM4->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_CHSIZE(0); //8 Bit charsize
	// SERCOM4->SPI.CTRLB.reg |= SERCOM_SPI_CTRLB_MSSEN; //Hardware controlled SS pin

	//Set Baud Rate
	SERCOM4->SPI.BAUD.reg = SERCOM_SPI_BAUD_BAUD(23); //Target 1MHz: 48MHz/(2*1MHz) - 1 = 23

	//Enable SPI
	SERCOM4->SPI.CTRLA.bit.ENABLE = 1;
	while (SERCOM4->SPI.SYNCBUSY.bit.ENABLE) {}



	// == SERCOM1 ==

	//Enable SERCOM1 in Power Manager
	PM->APBCMASK.bit.SERCOM1_ = 1;

	//MOSI: DDRA18 SERCOM2 PAD[2]
	//MISO: DDRA16 SERCOM2 PAD[0]
	//SCK:  DDRA19 SERCOM2 PAD[3]

	//DDR
	//MOSI
	PORT->Group[0].DIRSET.reg = PORT_PA18C_SERCOM1_PAD2; //DDRA18:Out 
	//MISO
	PORT->Group[0].DIRCLR.reg = PORT_PA16C_SERCOM1_PAD0; //DDRA16:IN
	//SCK
	PORT->Group[0].DIRSET.reg = PORT_PA19C_SERCOM1_PAD3; //DDRA19:Out


	//PINMUX
	// SCK (ODD) Peripheral D (SERCOM1 P3), MOSI (EVEN) Peripheral D (SERCOM1 P2)
	PORT->Group[0].PMUX[18/2].reg = PORT_PMUX_PMUXO_C | PORT_PMUX_PMUXE_C;
	// MISO (EVEN) Peripheral D (SERCOM1 P0)
	PORT->Group[0].PMUX[16/2].reg = PORT_PMUX_PMUXE_C;
	// // SS (ODD) Peripheral D (SERCOM1 P1)
	// PORT->Group[1].PMUX[9/2].reg = PORT_PMUX_PMUXO_D;

	//PINCFG
	PORT->Group[0].PINCFG[18].bit.PMUXEN = 1; //MOSI
	PORT->Group[0].PINCFG[16].bit.PMUXEN = 1; //MISO
	PORT->Group[0].PINCFG[19].bit.PMUXEN = 1; //SCK
	
	//Put Generic Clock Generator 0 as source for SERCOM4
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_SERCOM1_CORE) | // Generic Clock Multiplexer SERCOM4
						GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is source
						GCLK_CLKCTRL_CLKEN;
	//Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) { }

	SERCOM1->SPI.CTRLA.bit.SWRST = 1;
	//Wait for synchronization
	while (SERCOM1->SPI.SYNCBUSY.bit.SWRST) {}

	SERCOM1->SPI.CTRLA.reg = 
		SERCOM_SPI_CTRLA_MODE_SPI_MASTER | //Set Master Mode
		SERCOM_SPI_CTRLA_CPOL | SERCOM_SPI_CTRLA_CPHA | //Mode 3: CLK Idle High, sample rising
		SERCOM_SPI_CTRLA_FORM(0) | //SPI Frame Only
		SERCOM_SPI_CTRLA_DIPO(0) | //DIn(MISO) is PAD[0]
		SERCOM_SPI_CTRLA_DOPO(1) | //DOut(MOSI) is PAD[2], SCK is PAD[3]: Mode 1
		SERCOM_SPI_CTRLA_IBON; //STATUS.BUFOVF asserted immediately upon overflow
	//MSB first

	SERCOM1->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_CHSIZE(0); //8 Bit charsize
	// SERCOM4->SPI.CTRLB.reg |= SERCOM_SPI_CTRLB_MSSEN; //Hardware controlled SS pin

	//Set Baud Rate
	SERCOM1->SPI.BAUD.reg = SERCOM_SPI_BAUD_BAUD(23); //Target 1MHz: 48MHz/(2*1MHz) - 1 = 23

	//Enable SPI
	SERCOM1->SPI.CTRLA.bit.ENABLE = 1;
	while (SERCOM1->SPI.SYNCBUSY.bit.ENABLE) {}
}

// void SPITransmit(const u8 data)
// {
// 	SERCOM4->SPI.DATA.reg = data; //Data Out
// 	while (!SERCOM4->SPI.INTFLAG.bit.TXC) {} //Wait for transmit complete
// }

// void SPITransmit16(const u16 data)
// {
// 	SERCOM4->SPI.DATA.reg = (data>>8); //Data Out
// 	while (!SERCOM4->SPI.INTFLAG.bit.TXC) {} //Wait for transmit complete

// 	SERCOM4->SPI.DATA.reg = (data&0xFF); //Data Out
// 	while (!SERCOM4->SPI.INTFLAG.bit.TXC) {} //Wait for transmit complete
// }



volatile u8 TX_Ring[256];
volatile u8 TX_Head=0, TX_Tail=0, TX_Ongoing=0; 

volatile u8 RX_Ring[256];
volatile u8 RX_Head=0, RX_Tail=0;


void SERCOM0_Handler()
{
	// PORT->Group[0].OUTTGL.reg = PORT_PA02;
	if (SERCOM0->USART.INTFLAG.bit.RXC) //Receive Complete
	{
		RX_Ring[RX_Head] = SERCOM0->USART.DATA.reg;
		if (RX_Head+(u8)1 == RX_Tail) { RX_Tail++; }
		RX_Head++;
	}
	if (SERCOM0->USART.INTFLAG.bit.TXC) //Transmit Complete
	{
		TX_Ongoing = 0;
		SERCOM0->USART.INTFLAG.reg |= SERCOM_USART_INTFLAG_TXC; //Clear Transmit complete flag
		SERCOM0->USART.INTENCLR.bit.TXC = 1;
	}
	if (SERCOM0->USART.INTFLAG.bit.DRE) //Data Register Empty
	{
		if (TX_Tail != TX_Head) { SERCOM0->USART.DATA.reg = TX_Ring[TX_Tail++]; }
		else { SERCOM0->USART.INTENCLR.bit.DRE = 1; SERCOM0->USART.INTENSET.bit.TXC = 1;  }
	}
}

void IntiUART()
{
	//Enable SERCOM0 in Power Manager
	PM->APBCMASK.bit.SERCOM0_ = 1;

	//RX: PA11 SERCOM0 PAD[3]
	//TX: PA10 SERCOM0 PAD[2]

	//DDR
	//RX, TX
	PORT->Group[0].DIRSET.reg = PORT_PA11C_SERCOM0_PAD3 | PORT_PA10C_SERCOM0_PAD2; //DDRA11:Out  DDRA10:Out 


	//PINMUX
	// RX (ODD) Peripheral C (SERCOM0 P3), TX (EVEN) Peripheral C (SERCOM0 P2)
	PORT->Group[0].PMUX[10/2].reg = PORT_PMUX_PMUXO_C | PORT_PMUX_PMUXE_C;

	//PINCFG
	PORT->Group[0].PINCFG[11].bit.PMUXEN = 1; //RX
	PORT->Group[0].PINCFG[10].bit.PMUXEN = 1; //TX
	
	//Put Generic Clock Generator 0 as source for SERCOM0
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_SERCOM0_CORE) | // Generic Clock Multiplexer SERCOM4
						GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is source
						GCLK_CLKCTRL_CLKEN;
	//Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) { }

	SERCOM0->USART.CTRLA.bit.SWRST = 1;
	//Wait for synchronization
	while (SERCOM0->USART.SYNCBUSY.bit.SWRST) {}

	SERCOM0->USART.CTRLA.reg = 
		SERCOM_USART_CTRLA_DORD | //Set LSB first
		SERCOM_USART_CTRLA_FORM(0) | //USART Frame Only (No parity)
		SERCOM_USART_CTRLA_SAMPA(0) | //Oversampling adjustment (16x) 7-8-9
		SERCOM_USART_CTRLA_RXPO(3) | //RX is PAD[3]
		SERCOM_USART_CTRLA_TXPO(1) | //TX PAD[2]
		SERCOM_USART_CTRLA_SAMPR(0) | //16x oversampling using arithmetic generation
		SERCOM_USART_CTRLA_IBON | //STATUS.BUFOVF asserted immediately upon overflow
		SERCOM_USART_CTRLA_MODE(1); //Internal clock
	//Asynchronous Mode

	SERCOM0->USART.CTRLB.reg = SERCOM_USART_CTRLB_CHSIZE(0); //8 Bit charsize
	//1 stop bit

	//Set Baud Rate
	SERCOM0->USART.BAUD.reg = SERCOM_USART_BAUD_BAUD(63019); //Target 115200bps: 65536*(1-16*(115200/48MHz)) = 63019

	SERCOM0->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN; //RX and TX enabled
	while (SERCOM0->USART.SYNCBUSY.bit.CTRLB) {}

	//Enable UART
	SERCOM0->USART.CTRLA.bit.ENABLE = 1;
	while (SERCOM0->USART.SYNCBUSY.bit.ENABLE) {}

	//Enable SERCOM0 interrupts
	NVIC_EnableIRQ(SERCOM0_IRQn);

	//Enable interrupts
	SERCOM0->USART.INTENSET.reg = SERCOM_USART_INTENSET_RXC; //Receive complete 
}

void SerialTransmitByte(u8 data)
{
	// while (!SERCOM0->USART.INTFLAG.bit.DRE) {} //Wait for data empty
	SERCOM0->USART.DATA.reg = data; //Data Out
	while (!SERCOM0->USART.INTFLAG.bit.TXC) {} //Wait for data empty
}


void SerialTransmit(u8 *data, u8 len)
{
	for (u8 i=0; i<len; i++)
	{
		TX_Ring[TX_Head] = data[i];
		if (TX_Head+(u8)1 == TX_Tail) { return; }
		TX_Head++;
	}
	TX_Ongoing = 1;
	SERCOM0->USART.INTENSET.bit.DRE = 1;
}
void SerialFlush()
{
	while (TX_Ongoing==1) {}
}


u8 SerialRead(u8 *data, u8 len)
{
	u8 i=0;
	while (i<len && RX_Tail != RX_Head)
	{
		data[i++] = RX_Ring[RX_Tail++];
	}
	return i;
}

u8 SerialAvail()
{
	return RX_Head-RX_Tail+(RX_Tail>RX_Head?1:0);
}


// #define DISPSEL(cs) ENBITS(PORTC, 1<<(cs)) //(PORTB=(PORTB&(~(1<<2))) | (1<<2))
// #define DISPDESEL(cs) DABITS(PORTC, 1<<(cs)) //(PORTB=(PORTB&(~(1<<2))) | (0<<2))

// void Max7219Init(u8 numSegments, u8 cs)
// {
// 	DISPSEL(cs);

// 	Max7219SendCmd(0x0C00, numSegments, cs); //Disable
// 	Max7219SendCmd(0x0F00, numSegments, cs); //Test off
// 	Max7219SendCmd(0x0A00, numSegments, cs); //Intensity 0 (minimum)
// 	Max7219SendCmd(0x0900, numSegments, cs); //Decode off
// 	Max7219SendCmd(0x0B07, numSegments, cs); //Scan 7

// 	//Blank Display
// 	for (u16 l=1; l<=8; l++)
// 	{
// 		Max7219SendCmd(l<<8, numSegments, cs); 
// 	}

// 	Max7219SendCmd(0x0C01, numSegments, cs);  //Enable
// }

// void Max7219Refresh(u8 numSegments, u8 cs)
// {
// 	DISPSEL(cs);

// 	Max7219SendCmd(0x0F00, numSegments, cs); //Test off
// 	Max7219SendCmd(0x0A00, numSegments, cs); //Intensity 0 (minimum)
// 	Max7219SendCmd(0x0900, numSegments, cs); //Decode off
// 	Max7219SendCmd(0x0B07, numSegments, cs); //Scan 7

// 	Max7219SendCmd(0x0C01, numSegments, cs);  //Enable
// }

// void Max7219SendCmd(u16 cmd, u8 numSegments, u8 cs)
// {
// 	DISPDESEL(cs); 
// 	for (u8 d=0; d<numSegments; d++) 
// 	{ 
// 		SPITransmit16(cmd);
// 	}
// 	DISPSEL(cs);
// }

// void Max7219Send4Frame(u8 *data, u8 cs)
// {
// 	u16 y, n, i=0, l;
// 	for (y=0; y<8; y++)
// 	{
// 		l=(y+1)<<8;
// 		DISPDESEL(cs);
// 		SPITransmit16(l | data[i++]);
// 		SPITransmit16(l | data[i++]);
// 		SPITransmit16(l | data[i++]);
// 		SPITransmit16(l | data[i++]);
// 		DISPSEL(cs);
// 	}
// }
// void Max7219Send2Frame(u8 *data, u8 cs)
// {
// 	u16 y, n, i=0, l;
// 	for (y=0; y<8; y++)
// 	{
// 		l=(y+1)<<8;
// 		DISPDESEL(cs);
// 		SPITransmit16(l | data[i++]);
// 		SPITransmit16(l | data[i++]);
// 		DISPSEL(cs);
// 	}
// }
// void Max7219Send1Frame(u8 *data, u8 cs)
// {
// 	u16 y, n, i=0;
// 	for (y=0; y<8; y++)
// 	{
// 		DISPDESEL(cs);
// 		SPITransmit16(((y+1)<<8) | data[i++]);
// 		DISPSEL(cs);
// 	}
// }

// void Max7219SendData(u8 **data, u8 *numSegments, u8 numDisplays, u8 cs)
// {
// 	u16 i[numDisplays];

// 	for (u16 y=0; y<8; y++)
// 	{
// 		u16 cmd = ((y+1)<<8); //select row

// 		DISPDESEL(cs);
// 		//From last display to the first; The data for the end must be sent first
// 		for (u8 d=numDisplays-1; d<numDisplays; d--)
// 		{
// 			if (y==0) { i[d] = 0; }

// 			for (u8 s=0; s<numSegments[d]; s++)
// 			{
// 				SPITransmit16(cmd | data[d][i[d]++]);
// 			}
// 		}
// 		DISPSEL(cs);
// 	}
// }

// #include "frames.h"

// void Max7219SendFrames(u16 *indexes, u8 *numSegments, u8 numDisplays, u8 cs)
// {
// 	u16 i[numDisplays];

// 	for (u16 y=0; y<8; y++)
// 	{
// 		u16 cmd = ((y+1)<<8); //select row

// 		DISPDESEL(cs);
// 		//From last display to the first; The data for the end must be sent first
// 		for (u8 d=numDisplays-1; d<numDisplays; d--)
// 		{
// 			if (y==0) { i[d] = 0; }

// 			for (u8 s=0; s<numSegments[d]; s++)
// 			{
				
// 				SPITransmit16(cmd | FRAME_DAT_ACC_8(indexes[d], i[d]++));
// 			}
// 		}
// 		DISPSEL(cs);
// 	}
// }




