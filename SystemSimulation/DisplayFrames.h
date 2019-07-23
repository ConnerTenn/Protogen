
// Eyes: 16 x 8
// Mouth: 32 x 8
// Mouth Center: 8 x 8
// Nose: 8 x 8

// Encoding

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
		.Data=(u8 *)"\x10"
	},
};

