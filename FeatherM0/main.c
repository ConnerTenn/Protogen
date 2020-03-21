
#include "interfaces.h"


/*
Pin Definitions

         A         B              C          D          E           F           G           H        
PIN      EIC       ADC,AC,DAC     SERCOM     SERCOMALT  TC/TCC      TCC         COM         AC/GCLK  
-----------------------------------------------------------------------------------------------------
40:RST   _         _              _          _          _           _           _           _        
3.3V     _         _              _          _          _           _           _           _        
4 :PA03  EINT[3]   AIN[1]         _          _          _           _           _           _        
GND      _         _              _          _          _           _           _           _        
3 :PA02  EINT[2]   AIN[0],DAC     _          _          _           _           _           _        
7 :PB08  EINT[8]   AIN[2]         _          SERCOM4:0  TC4/WO[0]   _           _           _        
8 :PB09  EINT[9]   AIN[3]         _          SERCOM4:1  TC4/WO[1]   _           _           _        
9 :PA04  EINT[4]   AIN[4],AIN[0]  _          SERCOM0:0  TCC0/WO[0]  _           _           _        
10:PA05  EINT[5]   AIN[5],AIN[1]  _          SERCOM0:1  TCC0/WO[1]  _           _           _        
47:PB02  EINT[2]   AIN[10]        _          SERCOM5:0  TC6/WO[0]   _           _           _        
20:PB11  EINT[11]  _              _          SERCOM4:3  TC5/WO[1]   TCC0/WO[5]  I2S/SCK[1]  GCLK[5]  
19:PB10  EINT[10]  _              _          SERCOM4:2  TC5/WO[0]   TCC0/WO[4]  I2S/MCK[1]  GCLK[4]  
21:PA12  EINT[21]  _              SERCOM2:0  SERCOM4:0  TCC2/WO[0]  TCC0/WO[6]  _           AC/CMP[0]
16:PA11  EINT[11]  AIN[19]        SERCOM0:3  SERCOM2:3  TCC1/WO[1]  TCC0/WO[3]  I2S/FS[0]   GCLK[5]  
15:PA10  EINT[10]  AIN[18]        SERCOM0:2  SERCOM2:2  TCC1/WO[0]  TCC0/WO[2]  I2S/SCK[0]  GCLK[4]  
GND      _         _              _          _          _           _           _           _        
                                                                                                     
VBAT     _         _              _          _          _           _           _           _        
En       _         _              _          _          _           _           _           _        
VBUS     _         _              _          _          _           _           _           _        
26:PA17  EINT[1]   _              SERCOM1:1  SERCOM3:1  TCC2/WO[1]  TCC0/WO[7]  _           GCLK[3]  
28:PA19  EINT[3]   _              SERCOM1:3  SERCOM3:3  TCC3/WO[1]  TCC0/WO[3]  I2S/SD[0]   AC/CMP[1]
25:PA16  EINT[0]   _              SERCOM1:0  SERCOM3:0  TCC2/WO[0]  TCC0/WO[6]  _           GCLK[2]  
27:PA18  EINT[2]   _              SERCOM1:2  SERCOM3:2  TCC3/WO[0]  TCC0/WO[2]  _           AC/CMP[0]
12:PA07  EINT[7]   AIN[7],AIN[3]  _          SERCOM0:3  TCC1/WO[1]  _           I2S/SD[0]   _        
29:PA20  EINT[4]   _              SERCOM5:2  SERCOM3:2  TC7/WO[0]   TCC0/WO[6]  I2S/SCK[0]  GCLK[4]  
24:PA15  EINT[15]  _              SERCOM2:3  SERCOM4:3  TC3/WO[1]   TCC0/WO[5]  _           GCLK[1]  
32:PA23  EINT[7]   _              SERCOM3:1  SERCOM5:1  TC4/WO[1]   TCC0/WO[5]  USB/SOF     GCLK[7]  
31:PA22  EINT[6]   _              SERCOM3:0  SERCOM5:0  TC4/WO[0]   TCC0/WO[4]  _           GCLK[6]  
                                                                                                     
13:PA08
30:PA21
11:PA06


*/

/*
21: PIN Multiplexing Definitions

28: Address Space

34: Nested Vector Interrupt Controller (NVIC)

41: Periphial Access controller

48 Peripherals Address Space Summary

95: Generic Clock Controller (GCLK)
102: GCLK Register Summary

117: Power manager
127: Power manager Register Summary

148: System Controller (SYSCTRL)
165: SYSCTRL Register Summary

379: PORT section
387: PORT Register Summary

432: Serial Communication Interface (SERCOM)
440: SERCOM USART
454: SERCOM USART Register Summary
478: SERCOM SPI
489: SERCOM SPI Register Summary


610: Timer/Counter (TC)
624: TC Register Summary

651: Timer/Counter for Control (TCC)
684: TCC Register Summary

1104: Table Of Contents
*/


//MOSI: DDRA18 SERCOM2 PAD[2]
//MISO: DDRA16 SERCOM2 PAD[0]
//SCK:  DDRA19 SERCOM2 PAD[3]

//MOSI: DDRB10 SERCOM4 PAD[2]
//MISO: DDRA12 SERCOM4 PAD[0]
//SCK:  DDRB11 SERCOM4 PAD[3]

Max7219 DisplayListCOM1[] = 
	{//Must be listed in the order of connection
		{.NumSegments=2, .FrameIndex=11, .FrameDelay=-1, .QueuedIndex=-1}, //Eye
		{.NumSegments=1, .FrameIndex=9 , .FrameDelay=-1, .QueuedIndex=-1}, //Nose
		{.NumSegments=4, .FrameIndex=10, .FrameDelay=-1, .QueuedIndex=-1}, //Mouth
	};

// Max7219 DisplayListCOM4[] = 
// 	{//Must be listed in the order of connection
// 		// {.NumSegments=2, .FrameIndex=11, .FrameDelay=-1}, //Eye
// 		// {.NumSegments=1, .FrameIndex=9, .FrameDelay=-1}, //Nose
// 		// {.NumSegments=4, .FrameIndex=10, .FrameDelay=-1}, //Mouth
// 	};

// Max7219 DisplayList[sizeof(DisplayListCOM1)/sizeof(DisplayListCOM1[0]) + sizeof(DisplayListCOM4)/sizeof(DisplayListCOM4[0])];
	

const u8 NumDisplaysCOM1 = sizeof(DisplayListCOM1)/sizeof(DisplayListCOM1[0]);
// const u8 NumDisplaysCOM4 = sizeof(DisplayListCOM4)/sizeof(DisplayListCOM4[0]);


u8 TotalSegmentsCOM1;//, TotalSegmentsCOM4;

u16 RefreshTimer;

void TC3_Handler()
{
	if (TC3->COUNT16.INTFLAG.bit.MC0) //1kHz Timer
	{
		PORT->Group[0].OUTTGL.reg = PORT_PA02;

		if (RefreshTimer == 0)
		{
			Max7219RefreshCOM1(TotalSegmentsCOM1);
			RefreshTimer = REFRESH_INTERVAL;
		}
		else { RefreshTimer--; }

		Max7219SendFramesCOM1(DisplayListCOM1, NumDisplaysCOM1);
		// Max7219SendFramesCOM1(DisplayListCOM1, NumDisplaysCOM1);
		// Max7219SendFrames(DisplayListCOM1, NumDisplaysCOM1, DisplayListCOM1, NumDisplaysCOM1);


		TC3->COUNT16.INTFLAG.reg = TC_INTFLAG_MC0;
	}
	if (TC3->COUNT16.INTFLAG.bit.MC1) //1kHz Timer @50% Phase offset
	{
		for (u8 i=0; i<NumDisplaysCOM1; i++)
		{
			if (DisplayListCOM1[i].FrameDelay!=-1)
			{
				DisplayListCOM1[i].FrameDelay--;
			}
			if (DisplayListCOM1[i].FrameDelay==0)
			{
				DisplayListCOM1[i].FrameDelay=FRAME_HEADER_ACC(DisplayListCOM1[i].FrameIndex)->FrameDelay;
				DisplayListCOM1[i].FrameIndex=FRAME_HEADER_ACC(DisplayListCOM1[i].FrameIndex)->FrameNext;
			}
		}

		TC3->COUNT16.INTFLAG.reg = TC_INTFLAG_MC1;
	}
}

void InitTimers()
{
	//Enable TC3 in Power Manager
	PM->APBCMASK.bit.TC3_ = 1;

	//Put Generic Clock Generator 0 as source for SERCOM4
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_TCC2_TC3) | // Generic Clock Multiplexer SERCOM4
						GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is source
						GCLK_CLKCTRL_CLKEN;
	//Wait for synchronization
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) { }

	TC3->COUNT16.CTRLA.bit.SWRST = 1;
	//Wait for synchronization
	while (TC3->COUNT16.STATUS.bit.SYNCBUSY) {}

	TC3->COUNT16.CTRLA.reg = 
		TC_CTRLA_PRESCSYNC_RESYNC | //Reset on the next generic clock, rset prescaler counter
		TC_CTRLA_PRESCALER_DIV1 | //Divide by 1
		TC_CTRLA_WAVEGEN_MFRQ | //Match Frequency mode; Top is CC0
		TC_CTRLA_MODE_COUNT16; //16Bit Counter

	TC3->COUNT16.CC[0].reg = 48000; //Target 1kHz: 48MHz/1kHz = 48000
	TC3->COUNT16.CC[1].reg = 24000; //48000/2 = 24000

	//Enable TC3
	TC3->COUNT16.CTRLA.bit.ENABLE = 1;
	while (TC3->COUNT16.STATUS.bit.SYNCBUSY) {}


	//Enable TC3 interrupts
	NVIC_EnableIRQ(TC3_IRQn);

	//Enable Intterupts
	TC3->COUNT16.INTENSET.bit.MC0 = 1;
	TC3->COUNT16.INTENSET.bit.MC1 = 1;
}

int main()
{
	PORT->Group[0].DIRSET.reg = PORT_PA02; //Out


	TotalSegmentsCOM1=0;
	for (u8 i=0; i<NumDisplaysCOM1; i++)
	{
		TotalSegmentsCOM1 += DisplayListCOM1[i].NumSegments;
		DisplayListCOM1[i].FrameDelay=FRAME_HEADER_ACC(DisplayListCOM1[i].FrameIndex)->FrameDelay;
	}
	// TotalSegmentsCOM4=0;
	// for (u8 i=0; i<NumDisplaysCOM4; i++)
	// {
	// 	TotalSegmentsCOM4 += DisplayListCOM4[i].NumSegments;
	// 	DisplayListCOM4[i].FrameDelay=FRAME_HEADER_ACC(DisplayListCOM4[i].FrameIndex)->FrameDelay;
	// }

	RefreshTimer = 0;

	IntiSPI();

	IntiUART();

	InitTimers();


	// SerialTransmit((u8 *)"Hello!", 6);
	// SerialTransmit(FRAME_DATA, FRAME_LEN);
	
	SerialFlush();



	u8 i=0; (void)i;
	while (1)
	{
		PORT->Group[0].OUTTGL.reg = PORT_PA02;
	}

	return 0;
}

