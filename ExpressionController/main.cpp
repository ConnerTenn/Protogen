
#include "EmotionController.h"
#include <signal.h>

bool Run = true;

void InteruptHandler(int arg) { Run = false; }

int main()
{
	signal(SIGINT, InteruptHandler); signal(SIGKILL, InteruptHandler);

	InitWindow();
	srand(StartTime);
	
	//u64 startTime = GetMilliseconds();
	i64 lastTime = StartTime;
	i64 maxTime = 0;
	
	while (Run)
	{		
		u64 key;
		XEvent event;
		if (PendingEvent(&event))
		{
			if (event.type == KeyPress) 
			{ 
				key = GetKeyPressed(&event); 
				
				
				if (key == 65307) { Run = false; }
			}
			else if (event.type == KeyRelease) 
			{ 
				key = GetKeyReleased(&event); 
				
			}
		}

		
		
		ForceClear();

		EmotionController em;
		em.XPos=50; em.YPos=50;
		em.Display();

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
