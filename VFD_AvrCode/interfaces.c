
#include "interfaces.h"


ISR(SPI_STC_vect)
{
    //PORTC = !PORTC;
}

//Page 135
//Page 140
void IntiSPI()
{
    //Setup GPIO Pins
    //Must set directions before enabling SPI
    ENBITS(DDRB,(1<<DDRB_MOSI) | (1<<DDRB_SCK) | (1<<DDRB_CS));

    //Setup SPI
    SPCR = (1<<MSTR) | (1<<SPE) | (1<<CPOL) | (1<<CPHA);//Master & SPI enable
    //SPSR = (1<<SPI2X);

    //sei();
}

void SPITransmit(const u8 data)
{
    SPDR = data;
    while(!(SPSR & (1<<SPIF))) {}
}

void SPITransmit16(const u16 data)
{
    SPDR = data >> 8;
    while(!(SPSR & (1<<SPIF))) {}

    SPDR = data;
    while(!(SPSR & (1<<SPIF))) {}
}



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


#define DISPSEL(cs) ENBITS(PORTC, 1<<(cs)) //(PORTB=(PORTB&(~(1<<2))) | (1<<2))
#define DISPDESEL(cs) DABITS(PORTC, 1<<(cs)) //(PORTB=(PORTB&(~(1<<2))) | (0<<2))





