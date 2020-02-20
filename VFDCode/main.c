
#include "globals.h"

#include <stdio.h>

#include "interfaces.h"
#include "vfd.h"


//main callback
ISR(TIMER1_COMPA_vect)
{
    sei(); //Enable Nested Interrupts

    ENBITS(PORTC, 0x2);

    while (SerialAvail()>=4)
    {
        /*
        New serial format:
        u8 DeviceAddr
        u8 DestDisplay (implies Display type)
        u16 FrameNumber
        u16 MaskNumber
        u16 BorderNumber
        */
        u8 in[4];
        SerialRead(in, 4);

        
    }



    DABITS(PORTC, 0x2);
}

//Frame delay update
ISR(TIMER0_COMPA_vect)
{
    sei(); //Enable Nested Interrupts

    ENBITS(PORTC, 0x4);


    DABITS(PORTC, 0x4);
}

void InitTimers()
{
    //Counter 0 (8 bit)
    //Page 84
    TCCR0A = (1<<WGM01);
    OCR0A = 250 - 1; //(16MHz/64)/1000Hz  1KHz
    TCNT0 = 0;
    TIMSK0 = (1<<OCIE0A); //Enable Interrupt
    //CTC mode & Prescaler divide by 64 & start the timer
    TCCR0B = (0<<CS02)|(1<<CS01)|(1<<CS00); 

    //Offset Counter 1 by 1/16 of 1ms (of the overflow value of Counter 0)
    while (TCNT0<OCR0A/16) {} //Wait till offset is reached

    //Counter 1 (16 bit)
    //Page 89
    //Page 108
    OCR1A = 2000 - 1; //(16MHz/8)/1000Hz  1KHz
    TCNT1 = 0;
    TIMSK1 = (1<<OCIE1A); //Enable Interrupt
    //CTC mode & Prescale divide by 8 & start the timer
    TCCR1B = (1<<WGM12) | (0<<CS12)|(1<<CS11)|(0<<CS10); 
    
}

int main()
{
    PRR = 0;//PRR & ~(1<<PRSPI); //Power Reduction Register

    DDRC = 0xF;

	PORTC = 0xFF;

    IntiSPI();

    IntiUART();

	asm volatile (
    "    ldi  r18, 42"	"\n"
    "    ldi  r19, 142"	"\n"
    "1:  dec  r19"	"\n"
    "    brne 1b"	"\n"
    "    dec  r18"	"\n"
    "    brne 1b"	"\n"
    "    nop"	"\n"
	);

	PORTC = 0x00;
	InitDisplay();
	
	//GRAM_READ(0,0);

	PORTC = 0xFF;
    InitTimers();
	PORTC = 0x00;

    SerialTransmit("\nSetup Complete\n", 17);

    DISP_ON_OFF_CMD(1, 1, 1, 0, 0, 0);
	//SET_WRITE_POS_X_ADDR_CMD(0);
	//SET_WRITE_POS_Y_ADDR_CMD(0);
	SET_ADDR_MODE_CMD(1, 1);
	GRAM_WRITE(0xEA);
	GRAM_WRITE(0x4C);
	GRAM_WRITE(0x35);
	GRAM_WRITE(0x93);


    while (1)
    {
    }

    return 0;
}
