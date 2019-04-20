
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <General.h>
#include <Colour.h>
#include <WindowController.h>
#include "Frames.h"

/*
Emotions & Expressions:

Neutral
Happy
Sad
Suprised

*/


struct EmotionController
{
	u8 LeftEyeLEDs[EyeDim[1]][EyeDim[0]];
	u8 RightEyeLEDs[EyeDim[1]][EyeDim[0]];
	u8 LeftMouthLEDs[MouthDim[1]][MouthDim[0]];
	u8 RightMouthLEDs[MouthDim[1]][MouthDim[0]];
	u8 CenterMouthLEDs[MouthDim[2]][MouthDim[3]];
	u8 LeftNoseLEDs[NoseDim[1]][NoseDim[0]];
	u8 RightNoseLEDs[NoseDim[1]][NoseDim[0]];
	u8 LimbLEDs[LimbDim[1]][LimbDim[0]];

	int XPos = 0, YPos = 0;

	EmotionController();

	void Update(Expression expr, int eyeX, int eyeY, bool blink);
	//void Update(int expr, int eyeX, int eyeY);

	void Display(bool half);
};

#endif
