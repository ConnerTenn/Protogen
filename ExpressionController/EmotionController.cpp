
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

void RenderMatrix(int xPos, int yPos, u8 *mat, int width, int height)
{
	for (int y = 0; y<height; y++)
	{
		for (int x = 0; x<width; x++)
		{
			DrawCircle(x*15+xPos, y*15+yPos, 15, 15, {255,255,255});
			DrawCircle(x*15+xPos, y*15+yPos, 13, 13, {0,0,mat[y*height+x]});
			//OutlineCircle(x*15+xPos, y*15+yPos, 10, 10, {255,255,255});
		}
	}
}

void EmotionController::Display()
{
	int x = 0, y = 0;
	RenderMatrix(XPos+0, YPos+0, (u8 *)LeftEyeLEDs, EyeDim[0], EyeDim[1]);

	x = 15*EyeDim[0]/2; y = 15*EyeDim[1];
	RenderMatrix(XPos+x, YPos+y, (u8 *)LeftMouthLEDs, MouthDim[0], MouthDim[1]);

	x = 15*EyeDim[0]/2+15*MouthDim[0]; y = 15*EyeDim[1]+15*MouthDim[1]*2/3;
	RenderMatrix(XPos+x, YPos+y, (u8 *)LeftMouthLEDs, MouthDim[2], MouthDim[3]);
	
	x = 15*EyeDim[0]/2+15*MouthDim[0]+15*MouthDim[2]; y = 15*EyeDim[1];
	RenderMatrix(XPos+x, YPos+y, (u8 *)LeftMouthLEDs, MouthDim[0], MouthDim[1]);

	x = 2*15*MouthDim[0]+15*MouthDim[2]; y = 0;
	RenderMatrix(XPos+x, YPos+y, (u8 *)LeftMouthLEDs, EyeDim[0], EyeDim[1]);


	x = 15*EyeDim[0]/2 + 15*MouthDim[0] - 15*NoseDim[0]*2/3; y = 0;
	RenderMatrix(XPos+x, YPos+y, (u8 *)CenterMouthLEDs, MouthDim[2], MouthDim[3]);

	x = 15*EyeDim[0]/2 + 15*MouthDim[0] + 15*MouthDim[2] - 15*NoseDim[0]*1/3; y = 0;
	RenderMatrix(XPos+x, YPos+y, (u8 *)CenterMouthLEDs, MouthDim[2], MouthDim[3]);

}