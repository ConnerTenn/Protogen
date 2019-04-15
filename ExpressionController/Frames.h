
#ifndef _FRAMES_H_
#define _FRAMES_H_

#include <General.h>

constexpr int EyeDim[2] = {16,8};
constexpr int MouthDim[4] = {32,8,8,8};
constexpr int NoseDim[2] = {8,8};
constexpr int LimbDim[2] = {8,8};


struct EyeFrame
{
	const u8 *Pupil;
	const u8 *Frame;
	const u8 *Blink;
	const int CenterX, CenterY;
};
struct Expression
{
	const u8 *SideMouth;
	const u8 *CenterMouth;
	const u8 *Nose;
	EyeFrame Eye;
	const u8 *Limb;
	const u8 LeftEarPos;
	const u8 RightEarPos;
};

extern Expression ExpressionList[];

#endif

