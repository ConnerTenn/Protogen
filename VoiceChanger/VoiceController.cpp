
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
RoundBuffer<double> ProcessedBuffer(WindowSize);
RoundBuffer<double> OutputBuffer(WindowSize);
u64 Sample = 0;
double PreviousPhase[WindowSize], PhaseCumulative[WindowSize];

void Init()
{
	for (int i=0, n=(2*WindowSize+1); i<WindowSize; i++) 
	{ 
		Window[i]=0.5-cos(TAU*(i*2+1)/n)/2.0; 
	}
	
	for (int i=0; i<WindowSize; i++)
	{
		SampleBuffer[i] = 0;
		ProcessedBuffer[i] = 0;
		OutputBuffer[i] = 0;
	}
}

void ProcessSamples(float *input, float *output, int numSamples)
{
	double shift = 1.0;
	int hopOut = round(shift*HopSize);
	
	for (int s=0; s<numSamples; s++)
	{
		SampleBuffer.InsertEnd(input[s]);
		Sample++;
		
		if (Sample % HopSize == 0) //New full frame generated
		{
			ComplexD frame[WindowSize];
			for (int i=0; i<WindowSize; i++)
			{
				frame[i] = SampleBuffer[i] * Window[i] / sqrt(WindowSize/(HopSize*2.0));
			}
			
			FftInplace(frame, WindowSize);
			//double phase[WindowSize];
			for (int i=0; i<WindowSize; i++)
			{
				double deltaPhase = arg(frame[i]) - PreviousPhase[i];
				PreviousPhase[i] = arg(frame[i]);
				
				deltaPhase = deltaPhase - HopSize*TAU*i/WindowSize;
				
				deltaPhase = mmod(deltaPhase+PI, TAU) - PI;
				
				double freq = TAU * i / WindowSize + deltaPhase / HopSize;
				
				PhaseCumulative[i] = PhaseCumulative[i] + hopOut * freq;
			}
			
			for (int i=0; i<WindowSize; i++)
			{
				frame[i] = abs(frame[i]) * exp(PhaseCumulative[i]*ComplexD(0,1));
			}
			
			IfftInplace(frame, WindowSize);
			
			for (int i=0; i<hopOut; i++) { ProcessedBuffer.InsertEnd(0); }
			for (int i=0; i<WindowSize; i++)
			{
				ProcessedBuffer[i]+=(real(frame[i]) * Window[i] / sqrt(WindowSize/(hopOut*2.0)));
			}
			for (int i=0; i<hopOut; i++) { OutputBuffer.InsertEnd(ProcessedBuffer[i]); Sample--; } // Fix Output buffering
		}
		
		output[s] = OutputBuffer[Sample-1];
	}
}

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


