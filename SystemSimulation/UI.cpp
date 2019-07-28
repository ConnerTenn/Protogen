
#include "UI.h"


u32 FocusedPane = 0;

i32 EmoteCursor=0;
void DrawEmotionPane(int x, int y)
{
	// if (Selected==-2) { PRINT(INV); }
	// MOVETO(x, y+0*3);
	// PRINT("Individuals" RESET);
	
	// if (Selected==-1) { PRINT(INV); }
	// MOVETO(x, y+1*3);
	// PRINT("Groups" RESET);
	int ypos=y+2*3;
	for (i16 i=0; i<NumExpressions; i++)
	{
		//FillCharacers(x, y+i*3+2*3,3,3,{' ',BBLACK});
		
		MOVETO(x, ypos);
		if (i==EmoteCursor) { PRINT(CYAN); }
		if (i==SelectedEmote) { PRINT(INV); }
		PRINT("Emote %s", Expressions[i].Name);
		ypos+=1;

		PRINT(RESET);

		u8 linelen=0;
		for (i32 j=0; j<7; j++)
		{
			const char *name=ExprFrags[Expressions[i].Frags[j]].Name;
			u8 len =strlen(name)+1;
			if (linelen+len > 80/2-1) { linelen=0; ypos+=1; }

			MOVETO(x+linelen, ypos);
			PRINT("%s ",name);
			linelen+=len;
		}
		ypos+=1;
		ypos+=1;

		PRINT(RESET);
	}
}


extern u8 Battery;
extern float UpdateFreq;
void DrawInfoPane(int x, int y)
{
	MOVETO(x,y);
	PRINT("Status: %s  ", "OK");
	PRINT("UpdateRate: %.1fu/s  ", UpdateFreq);
	PRINT("Battery: %d%%", (100*Battery)/255);
}


const u16 TermLogLen=20;
char TerminalLog[TermLogLen][64];
u8 TermLogHead=0;
void DrawControlPane(int x, int y)
{
	MOVETO(x,y);
	FillCharacers(x,y,10,10,{' ',BBLUE});

	MOVETO(x,y+30);
	PRINT("Terminal Log");
	for (u16 i=0; i<TermLogLen; i++)
	{
		MOVETO(x,(int)(y+31+i));
		PRINT("%s", TerminalLog[(i+TermLogHead)%TermLogLen]);
	}
}


void InitUI()
{
	LOGENTERFUNC
	
	LOGRETFUNC
}

void DestroyUI()
{
	LOGENTERFUNC
	
	LOGRETFUNC
}

void UpdateUI()
{
	FillCharacers(0,0,80,1,{' ',BGREY});
	FillCharacers(0,60-1,80,1,{' ',BGREY});
	FillCharacers(0,0,1,60,{' ',BGREY});
	FillCharacers(80-1,0,1,60,{' ',BGREY});


	DrawInfoPane(1,1);

	DrawControlPane(1,2);

	DrawEmotionPane(80/2,2);
}

void UIHandleInput(Ksequ key, MessageHandler *messenger)
{
	if (key.val == K_UP && EmoteCursor>-2) { EmoteCursor--; }
	if (key.val == K_DOWN && EmoteCursor<NumExpressions-1) { EmoteCursor++; }
	if (key.val == K_ENTER) 
	{ 
		SelectedEmote=EmoteCursor; 
		SendExpressionState(messenger);
		TermLogWrite((char *)"SendExpressionState");
	}
}

void TermLogWrite(char *msg)
{
	if (TermLogHead==0) { TermLogHead=TermLogLen-1; }
	else { TermLogHead--; }
	strncpy(TerminalLog[TermLogHead], msg, 64);
}
