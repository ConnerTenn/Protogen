
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <General.h>
#include <Colour.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>

#define CSI(a,c) "\033[" a c

#define RESET    CSI("0","m")
#define BLACK    CSI("1;38;5;232","m")
#define RED      CSI("1;31","m")
#define GREEN    CSI("1;32","m")
#define BLUE     CSI("1;34","m")
#define MAGENTA  CSI("1;35","m")
#define YELLOW   CSI("1;33","m")
#define CYAN     CSI("1;36","m")
#define WHITE    CSI("1;37","m")
#define INV      CSI("7","m")

#define BBLACK   CSI("1;48;5;232","m")
#define BRED     CSI("1;41","m")
#define BGREEN   CSI("1;42","m")
#define BBLUE    CSI("1;44","m")
#define BMAGENTA CSI("1;45","m")
#define BYELLOW  CSI("1;43","m")
#define BCYAN    CSI("1;46","m")
#define BWHITE   CSI("1;47","m")

#define CLRLINE CSI("0","K")

#define MOVETO(x,y) printf(CSI("%d;%d", "H"),(y)+1,(x)+1)
//#define MOVEREL(x,y) CSI(<<((y)<0?-(y)+1:0)<<,"A") << CSI(<<((y)>0?(y)+1:0)<<,"B") << CSI(<<((x)>0?(x)+1:0)<<,"C") << CSI(<<((x)<0?-(x)+1:0)<<,"D")

#define PRINT(...) printf(__VA_ARGS__) //std::cout<<s<<std::flush
#define PRINTF(...) PRINT(__VA_ARGS__); fflush(stdout)
#define ERROR(...) PRINTF(RED "ERROR: " RESET __VA_ARGS__)
//#define FCOLOUR(r,g,b) PRINT(CSI("38;2;%d;%d;%d","m"),r,g,b)
//#define BCOLOUR(r,g,b) PRINT(CSI("48;2;%d;%d;%d","m"),r,g,b)

inline void MoveRel(int x, int y)
{
	if (y<0) { PRINT(CSI("%d","A"), -y); }
	else if (y>0) { PRINT(CSI("%d","B"), y); }
	if (x<0) { PRINT(CSI("%d","D"), -x); }
	else if (x>0) { PRINT(CSI("%d","C"), x); }
}

extern struct termios orig_termios;
void reset_terminal_mode();
void set_conio_terminal_mode();
int kbhit();
int getch();

struct ColourChar
{
	char Character = 0;
	const char *codes;
};

Array<int, 2> GetDimensions();
void FillCharacers(int x, int y, int width, int height, ColourChar character);
void ClearTerm();
void SavePosition();
void RecoverPosition();

/*
class TextScreen
{
	int Width = 0;
	int Height = 0;
	ColourChar **ScreenBuffer;
public:
	bool ClearOnClose = true;
	
	TextScreen();
	TextScreen(int height);
	TextScreen(int width, int height);
	~TextScreen();

	void Clear();
	Array<int,2> GetDimensions();

	void Draw(int x, int y, RGB foreground, RGB background, char character);
	void DrawToBuffer(int x, int y, RGB foreground, RGB background, char character);
	void ClearBuffer();
	void DisplayBuffer();
	int GetWidth();
	int GetHeight();

private:
	void SavePosition();
	void RecoverPosition();
};

RGB RGBFade(double val);

class TerminalBuffer
{
private:	
	RoundBuffer<std::string> Buffer;
	int X; 
	int Y;
public:
	TerminalBuffer(int x, int y, int length);
	void Write(std::string text);
	void Display();
};

extern TerminalBuffer TermBuffer;

*/

#endif
