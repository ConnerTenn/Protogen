
#include "Frames.h"

u8 SmileLeft[MouthDim[0]][MouthDim[1]] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,
	0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1};
u8 SmileRight[MouthDim[0]][MouthDim[1]] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,
	0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,
	0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
	0,1,1,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,
	1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
	1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0};
u8 SmileCenter[MouthDim[3]][MouthDim[2]] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,1,1,0,0,0,
	0,1,1,1,1,1,1,0,
	1,1,1,0,0,1,1,1,
	1,0,0,0,0,0,0,1,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0};

u8 LeftEyeHappy[EyeDim[1]][EyeDim[0]] = {
	0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
	1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,
	0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
u8 RightEyeHappy[EyeDim[1]][EyeDim[0]] = {
	0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,
	0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,
	0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,
	0,0,0,1,1,1,0,0,0,0,0,0,1,1,1,1,
	0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

u8 LeftEyeAngryMask[EyeDim[1]][EyeDim[0]] = {
	0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,
	0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,0,
	0,0,0,0,0,1,1,2,2,2,2,2,2,0,0,0,
	0,0,0,0,0,1,1,1,1,1,2,2,2,2,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	0,0,0,0,0,1,1,1,1,2,2,2,2,2,0,0,
	0,0,0,0,2,2,2,2,2,2,2,2,2,2,0,0};

u8 NoseLeft[NoseDim[1]][NoseDim[0]] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,0,
	0,0,1,1,1,1,1,1,
	0,0,0,0,0,0,1,1,
	0,0,0,0,0,0,1,1,
	0,0,0,0,0,0,0,1,
	0,0,0,0,0,0,0,0};
u8 NoseRight[NoseDim[1]][NoseDim[0]] = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,0,
	1,1,1,1,1,1,0,0,
	1,1,0,0,0,0,0,0,
	1,1,0,0,0,0,0,0,
	1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0};

void RenderEyesHappy(int x, int y, u8 *left, u8 *right);
void RenderEyesAngry(int x, int y, u8 *left, u8 *right);
Expression Happy
{
	(u8 *)SmileLeft,
	(u8 *)SmileRight,
	(u8 *)SmileCenter,
	(u8 *)NoseLeft,
	(u8 *)NoseRight,
	RenderEyesHappy
};
Expression Angry
{
	(u8 *)SmileLeft,
	(u8 *)SmileRight,
	(u8 *)SmileCenter,
	(u8 *)NoseLeft,
	(u8 *)NoseRight,
	RenderEyesAngry
};


Expression ExpressionList[] = {Happy, Angry};


void SubArrCpy(u8 *src, u8 *dest, int srcX, int srcY, int destX, int destY, int clipWidth, int clipHeight, int width, int height)
{
	for (int y = 0; y<clipHeight; y++)
	{
		for (int x = 0; x<clipWidth; x++)
		{
			if (x+destX>=0 && x+destX<width && x+srcX>=0 && x+srcX<width && y+destY>=0 && y+destY<height && y+srcY>=0 && y+srcY<height)
			{
				dest[(x+destX)+(y+destY)*width] = src[(x+srcX)+(y+srcY)*width];
			}
		}
	}
}

void RenderEyesHappy(int eyeX, int eyeY, u8 *left, u8 *right)
{
	eyeX+=EyeDim[0]/2-1; eyeY+=EyeDim[1]/2;
	/*for (int y = 0; y<EyeDim[1]; y++)
	{
		for (int x = 0; x<EyeDim[0]; x++)
		{
			if (pow(x-eyeX,2)+pow(y-eyeY,2)<6)
			{
				left[x+1+y*EyeDim[0]] = 1;
				right[x-1+y*EyeDim[0]] = 1;
			}
		}
	}*/
	SubArrCpy((u8 *)LeftEyeHappy, left, 0, 0, eyeX-13/2, eyeY-6/2, 13, 6, EyeDim[0], EyeDim[1]);
	SubArrCpy((u8 *)RightEyeHappy, right, 3, 0, eyeX-13/2+1, eyeY-6/2, 13, 6, EyeDim[0], EyeDim[1]);
}

void RenderEyesAngry(int eyeX, int eyeY, u8 *left, u8 *right)
{
	eyeX+=EyeDim[0]/2; eyeY+=EyeDim[1]/2;
	for (int y = 0; y<EyeDim[1]; y++)
	{
		for (int x = 0; x<EyeDim[0]; x++)
		{
			u8 pupil = pow(x-eyeX-1,2)+pow(y-eyeY-1,2)<6;
			left[x+y*EyeDim[0]] = (LeftEyeAngryMask[y][x]&0x2) | (LeftEyeAngryMask[y][x]&pupil);
			pupil = pow(x-eyeX+2,2)+pow(y-eyeY-1,2)<6;
			right[x+y*EyeDim[0]] = (LeftEyeAngryMask[y][EyeDim[0]-x-1]&0x2) | (LeftEyeAngryMask[y][EyeDim[0]-x-1]&pupil);
		}
	}
}

