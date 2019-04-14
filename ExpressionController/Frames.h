
#ifndef _FRAMES_H_
#define _FRAMES_H_

#include <General.h>

static constexpr int EyeDim[2] = {16,8};
static constexpr int MouthDim[4] = {32,8,8,8};
static constexpr int NoseDim[2] = {8,8};

struct Expression
{
	//u8 LeftEye[EyeDim[1]][EyeDim[0]];
	//u8 RightEye[EyeDim[1]][EyeDim[0]];
	u8 *LeftMouth;
	u8 *RightMouth;
	u8 *CenterMouth;
	u8 *LeftNose;
	u8 *RightNose;

	void ( *RenderEyes)(int x, int y, u8 *left, u8 *right);
};

extern Expression Happy;

extern Expression ExpressionList[];

#endif

