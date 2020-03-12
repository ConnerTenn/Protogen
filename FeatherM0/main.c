

#define REG(t, r) (*(t *)(r))

typedef unsigned char u8;


int main()
{
	REG(volatile u8, 0x41004400+0x00) = (1<<6);

	while (1)
	{
		REG(volatile u8, 0x41004400+0x10) = (1<<6); 
		REG(volatile u8, 0x41004400+0x10) = (0<<6); 
	}

	return 0;
}

