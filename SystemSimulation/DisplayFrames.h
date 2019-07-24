
// Eyes: 16 x 8
// Mouth: 32 x 8
// Mouth Center: 8 x 8
// Nose: 8 x 8

// Encoding
/*
Bytes record horizontal pixel data in rows. Each bit represents 1 pixel
*/

struct Frame
{
	u8 *Data;
	u8 DelayMin, DelayMax;
	u8 Next;
};


Frame Frames[]=
{
	// [0]={},
	// [1]=
	{
		.Data=(u8 *)"\x00\x00\x00\x00\x00\x00\x00\x00"
	},
	{
		.Data=(u8 *)"\xe1\x81\xe1\x81\xe1\x49\x92\x49"
	},
	{
		.Data=(u8 *)"\xe7\x81\xe7\x84\xe7\x49\x92\x49"
	},
};

