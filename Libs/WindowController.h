
//http://blog.robertelder.org/detect-keyup-event-linux-terminal/

#ifndef _WINDOW_CONTROLLER_H_
#define _WINDOW_CONTROLLER_H_

//#include <stdio.h>
//#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <string>
#include <Colour.h>

extern Window Win;
extern Display *Disp;

u_int64_t XColour(RGB rgb);

void InitWindow();
void DestroyWindow();


bool PendingEvent(XEvent *event);
unsigned long long GetKeyPressed(XEvent *event);
unsigned long long GetKeyReleased(XEvent *event);

void Sync();
void ForceClear();
void DrawPixel(int x, int y, RGB colour);
void DrawRectangle(int x, int y, int w, int h, RGB colour);
void OutlineRectangle(int x, int y, int w, int h, RGB colour);
void DrawLine(int x1, int y1, int x2, int y2, RGB colour);
void DrawText(int x, int y, std::string text, RGB colour);

void Graph(int xOff, int yOff, double xB, double xE, double xS, double yS, double (*f)(double), RGB colour);

#endif


