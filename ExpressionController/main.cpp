
#include "EmotionController.h"
#include <signal.h>
#include <iostream>

bool Run = true;

void InteruptHandler(int arg) { Run = false; }


std::string MenuItems[] = {
	"Happy"
};

void DrawMenu()
{
	for (int i = 0; i < 1; i++)
	{
		DrawText(50, 15*i+400, std::to_string(i+1) + ": " +MenuItems[i], {255,255,255});
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
	
	int eyeX=0, eyeY=0;

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
				if (key==65429) { eyeX = -2; eyeY=-2; }
				if (key==65431) { eyeX = 0; eyeY=-2; }
				if (key==65434) { eyeX = 2; eyeY=-2; }
				if (key==65430) { eyeX = -2; eyeY=0; }
				if (key==65432) { eyeX = 2; eyeY=0; }
				if (key==65436) { eyeX = -2; eyeY=2; }
				if (key==65433) { eyeX = 0; eyeY=2; }
				if (key==65435) { eyeX = 2; eyeY=2; }
				if (key == 65307) { Run = false; }
			}
			else if (event.type == KeyRelease) 
			{ 
				key = GetKeyReleased(&event); 
				if (key==65429 || key==65431 || key==65434 || key==65430 || key==65432 || key==65436 || key==65433 || key==65435) { eyeX=0; eyeY=0; }
			}
		}

		
		EmotionController em;
		em.XPos=50; em.YPos=50;
		em.Update(0, eyeX, eyeY);
		
		ForceClear();

		em.Display();

		DrawMenu();

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
