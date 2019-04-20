
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

#define SAMPLERATE 44100/4
#define BUFFERLEN 128

RoundBuffer<float> Buffer(BUFFERLEN);
u64 Count=0;
static int PACallback( const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData )
{
	for(u64 s = 0; s < framesPerBuffer; s++)
	{
		Buffer.InsertBegin(((float *)inputBuffer)[s]);
		ComplexD spectrum[BUFFERLEN]; float buff[BUFFERLEN];
		for(int j = 0; j < BUFFERLEN; j++) { buff[j] = Buffer[j]; }
		Fft(buff, spectrum, BUFFERLEN);
		
		
		for (int f=0; f<CEILDIV(BUFFERLEN,2); f++)
		{
			if (f==0) { ((float *)outputBuffer)[s] = 0; }
			double r = CEILDIV(BUFFERLEN,2);
			int mod = floor(r/2*(1-cos(f*TAU/2.0)*(1.0-f/r)));
			((float *)outputBuffer)[s] += abs(spectrum[f])*cos((double)f*(s+Count*framesPerBuffer)*TAU/BUFFERLEN + TAU*mod/(BUFFERLEN/2.0));
			//((float *)outputBuffer)[s] = ((float *)inputBuffer)[s];
		}
		//((float *)outputBuffer)[s] += Buffer[0];
		//((float *)outputBuffer)[s] += 0.05*cos((double)f*s*TAU/BUFFERLEN - (double)s/BUFFERLEN*TAU) * cos(Count/10.0);
		
		for (int i = -100; i <= 100; i++)
		{
			if (round(((float *)outputBuffer)[s]*800)==i)
			{
				std::cout << "#";
			}
			else if (i==0) { std::cout << "|"; }
			else { std::cout << " "; }
		}
		for (int i = 0; i < BUFFERLEN/2; i++)
		{
			
			std::cout << "\033[48;5;" << (int)(23*abs(spectrum[i])/0.05+232) << "m ";
		}
		std::cout << "\033[0m\n";
	}
	
	/*
	ComplexD spectrum[BUFFERLEN];
	Fft((float *)inputBuffer, spectrum, BUFFERLEN);
	//Fourier((float *)inputBuffer, spectrum, BUFFERLEN);
	
	//Ifft((float *)outputBuffer, spectrum, BUFFERLEN);
	for (int i=0; i<BUFFERLEN; i++) { ((float *)outputBuffer)[i] = 0; }
	for (int f=0; f<CEILDIV(BUFFERLEN,2); f++)
	{
		for (int i=0; i<BUFFERLEN; i++)
		{
			//((float *)inputBuffer)[i] += spectrum[f]*sin((double)f*i*TAU/BUFFERLEN);
			((float *)outputBuffer)[i] += abs(spectrum[f])*cos((double)f*i*TAU/BUFFERLEN - arg(spectrum[f]));
			//((float *)outputBuffer)[i] += abs(spectrum[f])*cos((double)f*i*TAU/BUFFERLEN + f*TAU*Count);
			//((float *)outputBuffer)[i] += abs(spectrum[f])*(2.0*((f*i+int(arg(spectrum[f])))%BUFFERLEN)/BUFFERLEN-1);
		}
		//Offset[f] = abs(spectrum[f])*cos((double)f*(BUFFERLEN-1)*TAU/BUFFERLEN - Offset[f]);
	}
	*/
	
	Count++;
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

	error = Pa_OpenStream(&Stream, &inputParameters, &outputParameters, (SAMPLERATE), BUFFERLEN/2, paNoFlag, &PACallback, 0);
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


