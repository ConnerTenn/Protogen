

#include "Frames.h"

int main()
{
	printf("Len:%lX\nData:\n", FrameDataEnd-FrameData);
	
	for (int i=0; i<FrameDataEnd-FrameData;)
	{
		printf("%02X ", FrameData[i]);
		i++;
		if (i%8==0) { printf("\n"); }
	}
	printf("\n\n");

	u64 offset = *((u64 *)FrameData);
	printf("Offset:%llu\n",offset);

	for (u64 i=0; i<8;)
	{
		printf("%02X ", FrameData[offset+i]);
		i++;
		if (i%8==0) { printf("\n"); }
	}

	return 0;
}

