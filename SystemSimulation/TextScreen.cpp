

#include "TextScreen.h"

pthread_mutex_t TermLock;


struct termios orig_termios;

void reset_terminal_mode()
{
	tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
	struct termios new_termios;

	// take two copies - one for now, one for later
	tcgetattr(0, &orig_termios);
	memcpy(&new_termios, &orig_termios, sizeof(new_termios));

	// register cleanup handler, and set the new terminal mode
	atexit(reset_terminal_mode);
	//cfmakeraw(&new_termios);
	new_termios.c_lflag &= ~(ECHO | ECHONL| ICANON | IEXTEN);
	
	//new_termios.c_oflag |= ONLCR;
	//new_termios.c_lflag |= ISIG;
	tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

int getch()
{
	int r;
	unsigned char c;
	if ((r = read(0, &c, sizeof(c))) < 0) { return r; }
	else { return c; }
}


Array<int,2> GetDimensions()
{
	//Array<int,2> dimensions;
	
	/*{
		fd_set readset;
		struct timeval time;
		struct termios term, initial_term;

		/ *We store the actual properties of the input console and set it as:
		no buffered (~ICANON): avoid blocking 
		no echoing (~ECHO): do not display the result on the console* /
		tcgetattr(0, &initial_term);
		term = initial_term;
		term.c_lflag &=~ICANON; term.c_lflag &=~ECHO;
		tcsetattr(0, TCSANOW, &term);

		
		PRINT(MOVETO(10000,10000) CSI(,"6n"));

		//We wait 100ms for a terminal answer
		FD_ZERO(&readset); FD_SET(0, &readset);
		time.tv_sec = 0; time.tv_usec = 100000;

		//If it success we try to read the cursor value
		if (select(STDIN_FILENO + 1, &readset, NULL, NULL, &time) == 1)
		{
			scanf("\033[%d;%dR", &(dimensions.Values[0]), &(dimensions.Values[1]));
		}

		//We set back the properties of the terminal
		tcsetattr(STDIN_FILENO, TCSADRAIN, &initial_term);
	}*/

	struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	return {(int)w.ws_col, (int)w.ws_row};
}

void FillCharacers(int x, int y, int width, int height, char ColourChar)
{
	// if (x>=0 && y>=0 && x < Width && y < Height)
	// {
	// 	int FR = (int)foreground.R, FG = (int)foreground.G, FB = (int)foreground.B;
	// 	int BR = (int)background.R, BG = (int)background.G, BB = (int)background.B;
	// 	PRINTF(MOVETO((x+1), (y+1)) FCOLOUR(FR, FG, FB) BCOLOUR(BR, BG, BB)<<character);
	// }
}

void ClearTerm()
{
	PRINT(MOVETO(Width-1, Height-1) CSI("1","J"));
}

void SavePosition()
{
	PRINT(CSI(,"s"));
}
void RecoverPosition()
{
	PRINT(CSI(,"u") RESET);
}


// TextScreen::TextScreen()
// {
// 	Array<int, 2> dimensions = GetDimensions();
// 	TextScreen(dimensions[0], dimensions[1]);
// }
// TextScreen::TextScreen(int width, int height)
// {
// 	Array<int, 2> dimensions = GetDimensions();
// 	Width = MIN(width,dimensions[0]); Height = MIN(height,dimensions[1]);

// 	PRINT(CSI("2","J")MOVETO(Width, Height) "\n");
// 	//for (int i = 0; i < Height; i++) { putchar('\n'); }
	
// 	ScreenBuffer = new ColourChar *[Height];
// 	for (int y = 0; y < Height; y++)
// 	{
// 		ScreenBuffer[y] = new ColourChar[Width];
// 		for (int x = 0; x < Width; x++)
// 		{
// 			ScreenBuffer[y][x] = {{0,0,0},{0,0,0},0};
// 		}
// 	}
// }
// TextScreen::~TextScreen()
// {
// 	if (ClearOnClose) { PRINT(RESET CSI("2","J") MOVETO(0,0)); }
// 	for (int y = 0; y < Height; y++)
// 	{
// 		delete[] ScreenBuffer[y];
// 	}
// 	delete[] ScreenBuffer;
// }

// void TextScreen::Clear()
// {
// 	PRINT(MOVETO(Width-1, Height-1) CSI("1","J"));
// }

// Array<int,2> TextScreen::GetDimensions()
// {
// 	//Array<int,2> dimensions;
	
// 	/*{
// 		fd_set readset;
// 		struct timeval time;
// 		struct termios term, initial_term;

// 		/ *We store the actual properties of the input console and set it as:
// 		no buffered (~ICANON): avoid blocking 
// 		no echoing (~ECHO): do not display the result on the console* /
// 		tcgetattr(0, &initial_term);
// 		term = initial_term;
// 		term.c_lflag &=~ICANON; term.c_lflag &=~ECHO;
// 		tcsetattr(0, TCSANOW, &term);

		
// 		PRINT(MOVETO(10000,10000) CSI(,"6n"));

// 		//We wait 100ms for a terminal answer
// 		FD_ZERO(&readset); FD_SET(0, &readset);
// 		time.tv_sec = 0; time.tv_usec = 100000;

// 		//If it success we try to read the cursor value
// 		if (select(STDIN_FILENO + 1, &readset, NULL, NULL, &time) == 1)
// 		{
// 			scanf("\033[%d;%dR", &(dimensions.Values[0]), &(dimensions.Values[1]));
// 		}

// 		//We set back the properties of the terminal
// 		tcsetattr(STDIN_FILENO, TCSADRAIN, &initial_term);
// 	}*/

// 	struct winsize w;
//     ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);


// 	return {(int)w.ws_col, (int)w.ws_row};
// }

// void TextScreen::Draw(int x, int y, RGB foreground, RGB background, char character)
// {
// 	if (x>=0 && y>=0 && x < Width && y < Height)
// 	{
// 		SavePosition();
// 		int FR = (int)foreground.R, FG = (int)foreground.G, FB = (int)foreground.B;
// 		int BR = (int)background.R, BG = (int)background.G, BB = (int)background.B;
// 		PRINTF(MOVETO((x+1), (y+1)) FCOLOUR(FR, FG, FB) BCOLOUR(BR, BG, BB)<<character);
// 		RecoverPosition();
// 	}
// }
// #include <sstream>
// void TextScreen::DrawToBuffer(int x, int y, RGB foreground, RGB background, char character)
// {
// 	if (x>=0 && y>=0 && x < Width && y < Height)
// 	{
// 		ScreenBuffer[y][x] = {foreground, background, character};
// 	}
// }
// void TextScreen::ClearBuffer()
// {
// 	for (int y = 0; y < Height; y++)
// 	{
// 		for (int x = 0; x < Width; x++)
// 		{
// 			ScreenBuffer[y][x] = {{0,0,0}, {0,0,0}, 0};
// 		}
// 	}
// }
// void TextScreen::DisplayBuffer()
// {
// 	std::stringstream out;
// 	for (int y = 0; y < Height; y++)
// 	{
// 		for (int x = 0; x < Width; x++)
// 		{
// 			int FR = (int)ScreenBuffer[y][x].Foreground.R, FG = (int)ScreenBuffer[y][x].Foreground.G, FB = (int)ScreenBuffer[y][x].Foreground.B;
// 			int BR = (int)ScreenBuffer[y][x].Background.R, BG = (int)ScreenBuffer[y][x].Background.G, BB = (int)ScreenBuffer[y][x].Background.B;
// 			char ch = ScreenBuffer[y][x].Character;
// 			if (ch) { out << MOVETO(x+1,y+1) << FCOLOUR(FR,FG,FB) << BCOLOUR(BR,BG,BB) << ch; }
// 		}
// 		//out << "\n";
// 	}
// 	out<<RESET;
// 	PRINT(out.str());
	
// }

// int TextScreen::GetWidth() { return Width; }
// int TextScreen::GetHeight() { return Height; }

// void TextScreen::SavePosition()
// {
// 	PRINT(CSI(,"s"));
// }
// void TextScreen::RecoverPosition()
// {
// 	PRINT(CSI(,"u") RESET);
// }

// RGB RGBFade(double val)
// {
// 	u8 R = (u8)(255.0*MAX(0, val<1.0/3.0? 3.0*(val-0.0/3.0) : 1.0-(val-1.0/3.0)*3.0));
// 	u8 G = (u8)(255.0*MAX(0, val<2.0/3.0? 3.0*(val-1.0/3.0) : 1.0-(val-2.0/3.0)*3.0));
// 	u8 B = (u8)(255.0*MAX(0, val<3.0/3.0? 3.0*(val-2.0/3.0) : 1.0-(val-3.0/3.0)*3.0));
// 	return {R,G,B};
// }


// TerminalBuffer::TerminalBuffer(int x, int y, int length) :
// 	Buffer(length)
// {
// 	X = x;
// 	Y = y;
// }

// void TerminalBuffer::Write(std::string text)
// {
// 	static int count = 0;
// 	Buffer.InsertBegin(std::to_string(count++) + ": " + text);
// }

// void TerminalBuffer::Display()
// {
// 	for (int i = 0; i < Buffer.Size(); i++)
// 	{
// 		PixelTerm::DrawText(X, i*15+Y, Buffer[i], {0xff,0xff,0xff});
// 		//std::cout << Buffer[i] << "\n";
// 	}
// }

