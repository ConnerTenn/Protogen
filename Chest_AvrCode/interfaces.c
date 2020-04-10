
#include "interfaces.h"



u8 TX_Ring[256];
u8 TX_Head=0, TX_Tail=0, TX_Ongoing=0; 


u8 RX_Ring[256];
u8 RX_Head=0, RX_Tail=0;

ISR(USART_RX_vect)
{
    RX_Ring[RX_Head] = UDR0;
    if (RX_Head+(u8)1 == RX_Tail) { RX_Tail++; }
    RX_Head++;
}
ISR(USART_TX_vect)
{
    cli();
    TX_Ongoing=0;
    DABITS(UCSR0B,(1<<UDRIE0));
    sei();
}
ISR(USART_UDRE_vect)
{
    if (TX_Tail != TX_Head) { UDR0 = TX_Ring[TX_Tail++]; }
    else { DABITS(UCSR0B,(1<<UDRIE0)); }
}

#define BAUD 9600
#define UBRR (F_CPU/16/BAUD-1)

void IntiUART()
{
    cli();
    UBRR0H = (u8)(UBRR>>8);
    UBRR0L = (u8)(UBRR);


    //Enable Transmit and Recieve
    UCSR0B = (1<<RXEN0)|(1<<TXEN0) | (1<<RXCIE0);//|(1<<TXCIE0);//|(1<<UDRIE0);
    //Set frame format: 8data, 2stop bit
    UCSR0C = (1<<USBS0)|(3<<UCSZ00); 

    sei();
}

void SerialFlush()
{
    while (TX_Ongoing==1) {}
}

void SerialTransmitByte(u8 data)
{
    //Wait for empty transmit buffer
    while ( !( UCSR0A & (1<<UDRE0)) );
    //Put data into buffer, sends the data
    UDR0 = data;
}

void SerialTransmit(u8 *data, u8 len)
{
    for (u8 i=0; i<len; i++)
    {
        TX_Ring[TX_Head] = data[i];
        if (TX_Head+(u8)1 == TX_Tail) { return; }
        TX_Head++;
    }
    ENBITS(UCSR0B,(1<<UDRIE0));
}

void SerialTransmitStr(char *data)
{
	while(*data)
	{
		TX_Ring[TX_Head] = *data;
        if (TX_Head+(u8)1 == TX_Tail) { return; }
        TX_Head++; data++;
	}
    ENBITS(UCSR0B,(1<<UDRIE0));
}

void SerialTransmitHexVal(u16 val)
{
	if (val)
	{
		while(!(val & 0xF000)) { val=(val<<4)&0xFFFF; }
		while (val)
		{
			u8 hx = ((val&0xF000)>>12);
			if (hx < 10) { hx += '0'; }
			else { hx += 'A'-10; }
			TX_Ring[TX_Head] = hx;
			if (TX_Head+(u8)1 == TX_Tail) { return; }
			TX_Head++; val=(val<<4)&0xFFFF;
		}
	}
	else
	{
		TX_Ring[TX_Head] = '0';
		if (TX_Head+(u8)1 == TX_Tail) { return; }
		TX_Head++;
	}
    ENBITS(UCSR0B,(1<<UDRIE0));
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


#define DISPSEL(cs) ENBITS(PORTC, 1<<(cs)) //(PORTB=(PORTB&(~(1<<2))) | (1<<2))
#define DISPDESEL(cs) DABITS(PORTC, 1<<(cs)) //(PORTB=(PORTB&(~(1<<2))) | (0<<2))





