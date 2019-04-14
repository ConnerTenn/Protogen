
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <General.h>
#include <Colour.h>
#include <WindowController.h>



struct EmotionController
{
	static constexpr int EyeDim[2] = {16,8};
	static constexpr int MouthDim[4] = {32,8,8,8};
	static constexpr int NoseDim[2] = {8,8};

	u8 LeftEyeLEDs[EyeDim[1]][EyeDim[0]];
	u8 RightEyeLEDs[EyeDim[1]][EyeDim[0]];
	u8 LeftMouthLEDs[MouthDim[1]][MouthDim[0]];
	u8 RightMouthLEDs[MouthDim[1]][MouthDim[0]];
	u8 CenterMouthLEDs[MouthDim[2]][MouthDim[3]];
	u8 LeftNoseLEDs[NoseDim[1]][NoseDim[0]];
	u8 RightNoseLEDs[NoseDim[1]][NoseDim[0]];

	int XPos = 0, YPos = 0;

	EmotionController();

	void Display();

};

#endif
