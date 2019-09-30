
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "globals.h"
#include "graphics.h"
#include "camera.h"

/*
Panels:
TopBar

Overview

Emotion Select

Double Emotion Select

Chest Display Controller

*/

extern enum Panels
{
	Overview,
	DoubleEmotion,
	ChestConfig,
	AudioPipe,
	Terminal,
} PanelState;

void HandleEyeEvent(int eyeX, int eyeY);
void HandleKeyEvent(union Ksequ sequ);

void RenderPanels(FrameBuffer fb);

void Log(const char *format, ...);

#endif
