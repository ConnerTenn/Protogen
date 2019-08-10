

#include <stdlib.h>
#include <stdio.h>

int main()
{
	FILE *f = fopen("/dev/fb0", "wb");

	unsigned int i = 0;
	while(1)
	{

		for (unsigned int y=0; y<480; y++)
		{
			for (unsigned int x=0; x<800; x++)
			{
				char *col;//[4]="\xff\x00\x00\x00";
				if (((y+i)/60)%3 == 0) { col="\x00\x00\xff\x00"; }
				else if (((y+i)/60)%3 == 1) { col="\x00\xff\x00\x00"; }
				else if (((y+i)/60)%3 == 2) { col="\xff\x00\x00\x00"; }
				fwrite(col, 4, 1, f);

			}
		}

		//usleep(100*1000);
		i++;
		//fwrite("\x00", 1, 1, f);
		fseek(f, 0, SEEK_SET);
		fflush(f);
	}

	fclose(f);

	return 0;
}
