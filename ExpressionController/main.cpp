
#include "EmotionController.h"
#include <signal.h>
#include <iostream>

bool Run = true;

void InteruptHandler(int arg) { Run = false; }


std::string MenuItems[] = {
	"Happy", "Angry"
};

void DrawMenu(int expr)
{
	DrawText(40, 15*expr+500, ">", {255,255,255});
	for (int i = 0; i < 2; i++)
	{
		DrawText(50, 15*i+500, std::to_string(i+1) + ": " +MenuItems[i], {255,255,255});
	}
}

int main()
{
	signal(SIGINT, InteruptHandler); signal(SIGKILL, InteruptHandler);

	InitWindow();
	srand(StartTime);
	
	//u64 startTime = GetMilliseconds();
	i64 lastTime = StartTime;
	i64 maxTime = 0;
	
	int eyeX=0, eyeY=0, expr=0;

	u8 KeyDirs[] = {0,0,0,0};

	while (Run)
	{		
		u64 key;
		XEvent event;
		if (PendingEvent(&event))
		{
			if (event.type == KeyPress) 
			{ 
				key = GetKeyPressed(&event); 
				std::cout << "Key [" << key << "]\n";
				if (key==65431) { KeyDirs[0]=1; }
				if (key==65437) { KeyDirs[1]=1; }
				if (key==65430) { KeyDirs[2]=1; }
				if (key==65432) { KeyDirs[3]=1; }
				if (key>=49 && key <=57) { expr=key-49; }
				if (key == 65307) { Run = false; }
			}
			else if (event.type == KeyRelease) 
			{ 
				key = GetKeyReleased(&event); 
				if (key==65431) { KeyDirs[0]=0; }
				if (key==65437) { KeyDirs[1]=0; }
				if (key==65430) { KeyDirs[2]=0; }
				if (key==65432) { KeyDirs[3]=0; }
			}
		}

		eyeX=0; eyeY=0;
		if (KeyDirs[0]) { eyeY-=2; }
		if (KeyDirs[1]) { eyeY+=2; }
		if (KeyDirs[2]) { eyeX-=2; }
		if (KeyDirs[3]) { eyeX+=2; }

		
		EmotionController em;
		em.XPos=50; em.YPos=50;
		em.Update(expr, eyeX, eyeY);
		
		ForceClear();

		em.Display();

		DrawMenu(expr);

		for (int i = 0; i < 100; i++) { DrawRectangle(400+10*i, 0, 20, 20, RGBVal(i/100.0)); }
		
		i64 delta = GetMicroseconds() - lastTime;
		lastTime = GetMicroseconds();
		maxTime = MAX(delta, maxTime);
		DrawText(10, 10, "Runtime:" + std::to_string((lastTime-StartTime)/1'000'000.0) + "  Frametime:" + std::to_string(delta/1'000'000.0) + "  Maxtime:" + std::to_string(maxTime/1'000'000.0), {255,255,255});
		if ((lastTime-StartTime) % 1'00'000 < 1000) { maxTime = 0; }
		
		
	
		Sync();
		
		//u64 nextTime = GetMilliseconds();
		//usleep((100-(GetMilliseconds()-lastTime))*1000);
		//usleep(500);
	}

	DestroyWindow();
}
