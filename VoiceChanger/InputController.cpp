
#include "InputController.h"
#include "Spectrum.h"

void PAudioError(PaError error)
{
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", error );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( error ) );
}

PaStream *Stream;

#define SAMPLERATE 44100/2
#define BUFFERLEN 128

static int PACallback( const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData )
{
	ComplexD spectrum[BUFFERLEN];
	Fft((float *)inputBuffer, spectrum, BUFFERLEN);
	//Fourier((float *)inputBuffer, spectrum, BUFFERLEN);
	
	//Ifft((float *)outputBuffer, spectrum, BUFFERLEN);
	for (int f=0; f<CEILDIV(BUFFERLEN,2); f++)
	{
		for (int i=0; i<BUFFERLEN; i++)
		{
			if (f==0) { ((float *)outputBuffer)[i] = 0; }
			//((float *)inputBuffer)[i] += spectrum[f]*sin((double)f*i*TAU/BUFFERLEN);
			((float *)outputBuffer)[i] += abs(spectrum[f])*cos((double)f*i*TAU/BUFFERLEN - arg(spectrum[f]));
			//((float *)outputBuffer)[i] += abs(spectrum[f])*(2.0*((f*i+int(arg(spectrum[f])))%BUFFERLEN)/BUFFERLEN-1);
		}
	}
	/*for (int i = 0; i < BUFFERLEN; i++)
	{
		((float *)outputBuffer)[i] = ((float *)inputBuffer)[i];
	}*/
	
	return 0;
}

void PASetup()
{
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

	error = Pa_OpenStream(&Stream, &inputParameters, &outputParameters, (SAMPLERATE), BUFFERLEN, paNoFlag, &PACallback, 0);
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


