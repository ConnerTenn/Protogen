
#include <avr/io.h>



typedef unsigned char u8;


int main()
{
	DDRC = 0xFF;

	while (1)
	{
		PORTC=0xFF;
		PORTC=0;
	}

	return 0;
}

