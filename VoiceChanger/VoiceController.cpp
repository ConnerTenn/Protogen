
#include "VoiceController.h"
#include "Spectrum.h"

void PAudioError(PaError error)
{
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", error );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( error ) );
}

PaStream *Stream;

#define SAMPLERATE 44100

const int WindowSize = 1024;
const int HopSize = 256; //WindowSize/4
double Window[WindowSize]; //Generate Hanning window
RoundBuffer<double> SampleBuffer(WindowSize);
RoundBuffer<double> ProcessedBuffer(WindowSize*4);
Queue<double> OutputBuffer(WindowSize*2);
u64 Sample = 0;
double PreviousPhase[WindowSize], PhaseCumulative[WindowSize];

void Init()
{
	for (int i=0, n=(2*WindowSize+1); i<WindowSize; i++) 
	{ 
		Window[i]=0.5-cos(TAU*(i*2.0+1.0)/n)/2.0; 
	}
	
	for (int i=0; i<WindowSize; i++)
	{
		SampleBuffer[i] = 0;
		ProcessedBuffer[i] = 0;
		OutputBuffer[i] = 0;
		
		PreviousPhase[i]=0;
		PhaseCumulative[i]=0;
	}
}

void ProcessSamples(float *input, float *output, int numSamples)
{
	double shift = 1.5;
	int hopOut = round(shift*HopSize);
	
	for (int s=0; s<numSamples; s++)
	{
		SampleBuffer.InsertEnd(input[s]);
		Sample++;
		
		//static int maxf = 0; double maxv = 0;
		if (Sample % HopSize == 0) //New full frame generated
		{
			ComplexD frame[WindowSize];
			
			for (int i=0; i<WindowSize; i++)
			{
				frame[i] = SampleBuffer[i] * Window[i] / sqrt(WindowSize/(HopSize*2.0));
			}
			
			FftInplace(frame, WindowSize);
			
			
			/*for (int i=1; i<WindowSize/6; i++)
			{
				if (abs(frame[i]) > maxv) { maxf = i; maxv=abs(frame[i]); }
			}
			for (int i = 0; i < WindowSize/6; i++)
			{
				double v = pow(abs(frame[i]),1.0/3.5);
				if (i==maxf) { std::cout << "\033[48;5;" << (int)(5*(v<=0.5?2*v:1)+(int)(5*(v>0.5?2*v-1:0))*(6)+16) << "m "; }
				else { std::cout << "\033[48;5;" << (int)(23*v+232) << "m "; }
			}
			std::cout << maxf << "\033[0m\n";*/
			/*// Simple Envelope Calculation
			for (int i=1; i<WindowSize; i++)
			{
				double w = 20;
				double c = 40;
				frame[i] = 10.0*frame[i] * exp(-1.0/(w*w) * (i-c)*(i-c));
			}*/
			/*//Simple Pitch Shift
			ComplexD frame2[WindowSize];
			for (int i=1; i<WindowSize; i++) { frame2[i]=0; }
			for (int i=WindowSize-1; i>=1; i--)
			{
				frame2[i]+=frame[(int)(i/1.5)];
			}
			for (int i=1; i<WindowSize; i++) { frame[i]=frame2[i]; }*/
			
			//if (maxf*2<WindowSize) { frame[maxf*2] *= 10; }
			//if (maxf*3<WindowSize) { frame[maxf*2] *= 8; }
			//if (maxf*3<WindowSize) { frame[maxf*3] *= 6; }
			//frame[14] = 0.05;
			
			
			for (int i=0; i<WindowSize; i++)
			{
				//Get phase difference
				double deltaPhase = arg(frame[i]) - PreviousPhase[i];
				PreviousPhase[i] = arg(frame[i]);
				
				//Remove the expected phase difference
				deltaPhase = deltaPhase - HopSize*TAU*i/WindowSize;
				
				//Map to -pi/pi range
				deltaPhase = mmod(deltaPhase+PI, TAU) - PI;
				
				//Get the new frequency
				double freq = TAU * i / WindowSize + deltaPhase / HopSize;
				//std::cout << (TAU * i / WindowSize) << ":" << freq << "    ";
				
				//Calculate final phase
				PhaseCumulative[i] = PhaseCumulative[i] + HopSize * freq; //Changed to HopSize from hopOut
			}
			//std::cout << "\n";
			
			//Apply phases
			for (int i=0; i<WindowSize; i++)
			{
				frame[i] = abs(frame[i]) * exp(PhaseCumulative[i]*ComplexD(0,1));
			}
			
			IfftInplace(frame, WindowSize);
			
			//Queue up new elemnts for next frame hop segment
			for (int i=0; i<hopOut; i++) { ProcessedBuffer.InsertEnd(0); }
			//Add frame (with overlap)
			for (int i=0; i<WindowSize; i++)
			{
				ProcessedBuffer[-WindowSize+i]+=(real(frame[i]) * Window[i] / sqrt(WindowSize/(hopOut*2.0)));
			}
			
			for (int i=0; i<HopSize; i++) 
			{
				double s = ((double)i*hopOut)/HopSize;
				//double a = fmod(s,1.0);
				//std::cout << a << "    \t" << (int)s << "-" << (int)(s)+1 << "   \t" << hopOut << "\n";
				//double v = (1.0-a)*ProcessedBuffer[(int)s] + a*ProcessedBuffer[(int)s+1];
				
				int i1=(int)s, i2=i1+1, i3=i1+2, i0=i1-1;
				double v0=ProcessedBuffer[i0], v1=ProcessedBuffer[i1], v2=ProcessedBuffer[i2], v3=ProcessedBuffer[i3]; 
				double v;
				v=-(1.0/6.0)*v0*(s-i1)*(s-i2)*(s-i3);
				v=v+(1.0/2.0)*v1*(s-i0)*(s-i2)*(s-i3);
				v=v-(1.0/2.0)*v2*(s-i0)*(s-i1)*(s-i3);
				v=v+(1.0/6.0)*v3*(s-i0)*(s-i1)*(s-i2);
				OutputBuffer.Push(v);
			}
		}
		
		/*if (Sample % WindowSize == 0)
		{
			for (int i=0; i<WindowSize; i++) 
			{
				double s = ((double)i*hopOut)/HopSize;
				double a = fmod(s,1.0);
				double v = (1.0-a)*ProcessedBuffer[(int)s] + a*ProcessedBuffer[(int)ceil(s)];
				OutputBuffer.Push(v);
			}
		}*/
		//std::cout << maxf << "\n";
		
		output[s] = 3*OutputBuffer.Pull();
	}
}

/*
Sampled
|-----0----|
   |-----1----|
      |-----2----|
         |-----3----|
            |-----4----|

Extension with constant pitch
|-----0----|
      |-----1----|
            |-----2----|
                  |-----3----|
                        |-----4----|
Re-Compression to increase pitch.
|---0---|
    |---1---|
        |---2---|
            |---3---|
                |---4---|


|------|
   |------|
      |------|

|------|
       |------|
              |------|

|---|
    |---|
        |---|

*/

static int PACallback( const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData )
{
	//float *input = (float *)inputBuffer, *output = (float *)outputBuffer;
	
	
	ProcessSamples((float *)inputBuffer, (float *)outputBuffer, framesPerBuffer);
	
	for(u64 s = 0; s < framesPerBuffer; s++)
	{
		//((float *)outputBuffer)[s] = ((float *)inputBuffer)[s];
		std::cout << "||";
		for (int i = -50; i <= 50; i++)
		{
			if (round(((float *)inputBuffer)[s]*800)==i) { std::cout << "#"; }
			else if (i==0) { std::cout << "|"; }
			else { std::cout << " "; }
		}
		std::cout << "||";
		for (int i = -50; i <= 50; i++)
		{
			if (round(((float *)outputBuffer)[s]*800)==i) { std::cout << "#"; }
			else if (i==0) { std::cout << "|"; }
			else { std::cout << " "; }
		}
		std::cout<<"||\n";
	}
	
	return 0;
}

void PASetup()
{
	Init();
	
	PaError error;
	
	std::cout << "Setting up PortAudio\n";

	error = Pa_Initialize();
	if( error != paNoError ) { return PAudioError(error); }
	
	PaStreamParameters inputParameters;
	inputParameters.device = Pa_GetDefaultInputDevice();
	inputParameters.channelCount = 1;
	inputParameters.sampleFormat = paFloat32;// | paNonInterleaved;
	inputParameters.suggestedLatency = 1;
	inputParameters.hostApiSpecificStreamInfo = 0;
	
	PaStreamParameters outputParameters;
	outputParameters.device = Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = 1;
	outputParameters.sampleFormat = paFloat32;// | paNonInterleaved;
	outputParameters.suggestedLatency = 1;
	outputParameters.hostApiSpecificStreamInfo = 0;

	//error = Pa_OpenStream(&OutStream, 0, &outputParameters, (SAMPLERATE), 1152, paNoFlag, &PAudioOutputCallback, 0);

	error = Pa_OpenStream(&Stream, &inputParameters, &outputParameters, (SAMPLERATE), WindowSize, paNoFlag, &PACallback, 0);
	if( error != paNoError ) { return PAudioError(error); }
	
	error = Pa_StartStream( Stream );
	if( error != paNoError ) { return PAudioError(error); }
}

void PAClose()
{
	PaError error;
	
	std::cout << "Stopping PortAudio\n";
	
	error = Pa_StopStream( Stream );
	if( error != paNoError ) { return PAudioError(error); }
	
	error = Pa_CloseStream( Stream );
	if( error != paNoError ) { return PAudioError(error); }
	
	Pa_Terminate();
}


