
#include <General.h>
#include "VoiceController.h"
#include <signal.h>
#include <unistd.h>

bool Run = true;
void InteruptHandler(int arg) { Run = false; }



#include <complex>
#include <math.h>

typedef std::complex<double> ComplexD;

int main(int argc, char **argv)
{	
	signal(SIGINT, InteruptHandler); signal(SIGKILL, InteruptHandler);
	
	PASetup();
	
	std::cout << "Running...\n";
	
	while(Run) { usleep(100000); }
	
	std::cout << "Stopping...\n";
	
	PAClose();
	
	std::cout << "Done\n";
	
	return 0;
}

