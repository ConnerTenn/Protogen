
#include "UI.h"

#include "Expressions.h"

u32 FocusedPane = 0;

i32 Selected=0;
void EmotionPaneDraw(int x, int y)
{
	if (Selected==-2) { PRINT(INV); }
	MOVETO(x, y+0*2);
	PRINT("Individuals");
	PRINT(RESET);
	
	if (Selected==-1) { PRINT(INV); }
	MOVETO(x, y+1*2);
	PRINT("Groups");
	PRINT(RESET);
	
	for (i32 i=0; i<(i32)ExpressionList.size(); i++)
	{
		MOVETO(x, y+i*2+4);
		if (i==Selected) { PRINT(INV); }
		PRINT("Emote %s", ExpressionList[i].Name.c_str());
		PRINT(RESET);
	}
}
void EmotionPaneEvent(Event event)
{
	if (event.Key.val == K_UP && Selected>-2) { Selected--; }
	if (event.Key.val == K_DOWN && Selected<(i32)ExpressionList.size()-1) { Selected++; }
}


extern u8 Battery;
extern float UpdateFreq;
void InfoPaneDraw(int x, int y)
{
	MOVETO(x,y);
	PRINT("Status: %s  ", "OK");
	PRINT("UpdateRate: %.1fu/s  ", UpdateFreq);
	PRINT("Battery: %d%%", (100*Battery)/255);
}
void InfoPaneEvent(Event event)
{
}

void ControlPaneDraw(int x, int y)
{
	MOVETO(x,y);
	FillCharacers(x,y,10,10,{' ',BBLUE});
}
void ControlPaneEvent(Event event)
{
}



std::vector<UIElem> UIElementList = 
{
	UIElem{ 
		.X=50, .Y=2,
		.Draw=EmotionPaneDraw,
		.Event=EmotionPaneEvent,
	},
	UIElem{ 
		.X=1, .Y=2,
		.Draw=ControlPaneDraw,
		.Event=ControlPaneEvent,
	},
	UIElem{ 
		.X=1, .Y=1,
		.Draw=InfoPaneDraw,
		.Event=InfoPaneEvent,
	},
};

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
	FillCharacers(0,0,100,1,{' ',BGREY});
	FillCharacers(0,59,100,1,{' ',BGREY});
	FillCharacers(0,0,1,60,{' ',BGREY});
	FillCharacers(99,0,1,60,{' ',BGREY});
	for (size_t i=0; i<UIElementList.size(); i++)
	{
		UIElem *elem = &UIElementList[i];
		elem->Draw(elem->X, elem->Y);
	}
}

void UIHandleInput(Ksequ key)
{
	UIElem *elem = &UIElementList[FocusedPane];
	elem->Event(Event{.Key=key});
}

