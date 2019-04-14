
#include "EmotionController.h"

EmotionController::EmotionController()
{
	for (int i = 0; i < EyeDim[0]*EyeDim[1]; i++)
	{
		((u8 *)LeftEyeLEDs)[i] = 0;
		((u8 *)RightEyeLEDs)[i] = 0;
	}
	for (int i = 0; i < MouthDim[0]*MouthDim[1]; i++)
	{
		((u8 *)LeftMouthLEDs)[i] = 0;
		((u8 *)RightMouthLEDs)[i] = 0;
	}
	for (int i = 0; i < MouthDim[2]*MouthDim[3]; i++)
	{
		((u8 *)CenterMouthLEDs)[i] = 0;
	}
	for (int i = 0; i < NoseDim[0]*NoseDim[1]; i++)
	{
		((u8 *)LeftNoseLEDs)[i] = 0;
		((u8 *)RightNoseLEDs)[i] = 0;
	}
}


void CopyMatrix(u8 *src, u8 *dest, int width, int height)
{
	for (int i = 0; i<width*height; i++)
	{
		dest[i] = src[i];
	}
}

void EmotionController::Update(int expr, int eyeX, int eyeY)
{
	CopyMatrix(ExpressionList[expr].LeftMouth, (u8 *)LeftMouthLEDs, MouthDim[0], MouthDim[1]);
	CopyMatrix(ExpressionList[expr].RightMouth, (u8 *)RightMouthLEDs, MouthDim[0], MouthDim[1]);
	CopyMatrix(ExpressionList[expr].CenterMouth, (u8 *)CenterMouthLEDs, MouthDim[2], MouthDim[3]);
	CopyMatrix(ExpressionList[expr].LeftNose, (u8 *)LeftNoseLEDs, NoseDim[0], NoseDim[1]);
	CopyMatrix(ExpressionList[expr].RightNose, (u8 *)RightNoseLEDs, NoseDim[0], NoseDim[1]);
	ExpressionList[expr].RenderEyes(eyeX, eyeY, (u8 *)LeftEyeLEDs, (u8 *)RightEyeLEDs);
}

void RenderMatrix(int xPos, int yPos, u8 *mat, int width, int height)
{
	OutlineRectangle(xPos-15/2,yPos-15/2, width*15, height*15, {0x3F,0x3F,0x3F});
	for (int y = 0; y<height; y++)
	{ for (int x = 0; x<width; x++) {
		//DrawCircle(x*15+xPos, y*15+yPos, 15, 15, {0x3F,0x3F,0x3F});
		DrawCircle(x*15+xPos, y*15+yPos, 13, 13, mat[y*width+x]?RGB{0x30,0x70,0xFF}:RGB{0,0,0});
		//OutlineCircle(x*15+xPos, y*15+yPos, 10, 10, {255,255,255});
	} }
}

void EmotionController::Display()
{
	int x = 0, y = 0;
	RenderMatrix(XPos+0, YPos+0, (u8 *)LeftEyeLEDs, EyeDim[0], EyeDim[1]);

	x = 15*2; y = 15*EyeDim[1]+15*8;
	RenderMatrix(XPos+x, YPos+y, (u8 *)LeftMouthLEDs, MouthDim[0], MouthDim[1]);

	x = 15*2+15*MouthDim[0]; y = 15*EyeDim[1]+15*MouthDim[1]*1/4+15*8;
	RenderMatrix(XPos+x, YPos+y, (u8 *)CenterMouthLEDs, MouthDim[2], MouthDim[3]);
	
	x = 15*2+15*MouthDim[0]+15*MouthDim[2]; y = 15*EyeDim[1]+15*8;
	RenderMatrix(XPos+x, YPos+y, (u8 *)RightMouthLEDs, MouthDim[0], MouthDim[1]);

	x = 2*15*MouthDim[0]+15*MouthDim[2]-(15*EyeDim[0]-2*15*2); y = 0;
	RenderMatrix(XPos+x, YPos+y, (u8 *)RightEyeLEDs, EyeDim[0], EyeDim[1]);


	x = 15*2 + 15*MouthDim[0] - 15*NoseDim[0]*2/3; y = 15*7;
	RenderMatrix(XPos+x, YPos+y, (u8 *)LeftNoseLEDs, NoseDim[0], NoseDim[1]);

	x = 15*2 + 15*MouthDim[0] + 15*MouthDim[2] - 15*NoseDim[0]*1/3; y = 15*7;
	RenderMatrix(XPos+x, YPos+y, (u8 *)RightNoseLEDs, NoseDim[0], NoseDim[1]);

}

