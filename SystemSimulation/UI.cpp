
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
	
	for (i32 i=0; i<(i32)ExpressionList.size(); i++)
	{
		//FillCharacers(x, y+i*3+2*3,3,3,{' ',BBLACK});
		
		MOVETO(x, y+i*3+2*3);
		if (i==EmoteCursor) { PRINT(CYAN); }
		if (i==SelectedEmote) { PRINT(INV); }
		PRINT("Emote %s", ExpressionList[i].Name.c_str());

		PRINT(RESET);

		MOVETO(x, y+i*3+2*3+1);
		for (i32 j=0; j<7; j++)
		{
			//FillCharacers(x+4+j*3, y+i*3+2*3+1,2,2,{' ',BBLACK});
			PRINT("%s ",ExpressionList[i].Frags[j]->Name.c_str());
		}

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


std::vector<std::string> TerminalLog;
void DrawControlPane(int x, int y)
{
	MOVETO(x,y);
	FillCharacers(x,y,10,10,{' ',BBLUE});

	MOVETO(x,y+30);
	PRINT("Terminal Log");
	for (size_t i=0; i<TerminalLog.size(); i++)
	{
		MOVETO(x,(int)(y+31+i));
		PRINT("%s", TerminalLog[i].c_str());
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
	if (key.val == K_DOWN && EmoteCursor<(i32)ExpressionList.size()-1) { EmoteCursor++; }
	if (key.val == K_ENTER) 
	{ 
		SelectedEmote=EmoteCursor; 
		SendExpressionState(messenger);
		TermLogWrite("SendExpressionState");
	}
}

void TermLogWrite(std::string msg)
{
	TerminalLog.push_back(msg);
	if(TerminalLog.size()>20) { TerminalLog.erase(TerminalLog.begin()); }
}
