
#include "globals.h"
#include "graphics.h"

/*
Panels:
TopBar

Overview

Emotion Select

Double Emotion Select

Chest Display Controller

*/

enum Panels
{
	Overview,
	DoubleEmotion,
	ChestConfig,
	AudioPipe,
} PanelState;

void RenderPanels(FrameBuffer fb);

