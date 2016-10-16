#include <stdio.h>
#include <cstdio>
#include "portaudio.h"
#include <climits>

#define SAMPLE_RATE (44100)

/* This routine will be called by the PortAudio engine when audio is needed.
It may called at interrupt level on some machines so don't do anything
that could mess up the system like calling malloc() or free().
*/
static int patestCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{

	float *input = (float *)inputBuffer;
	float max = INT_MIN;
	for (int i = 0; i < framesPerBuffer; i++) {
		if (input[i] > max) {
			max = input[i];
		}
	}
	printf("%f\n", max);
	return 0;
}

int main() {
	PaError err = Pa_Initialize();
	if (err != paNoError)
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));

	PaStream *stream;
	/* Open an audio I/O stream. */
	err = Pa_OpenDefaultStream(&stream,
		1,          /* no input channels */
		0,          /* stereo output */
		paFloat32,  /* 32 bit floating point output */
		SAMPLE_RATE,
		256,        /* frames per buffer, i.e. the number
					of sample frames that PortAudio will
					request from the callback. Many apps
					may want to use
					paFramesPerBufferUnspecified, which
					tells PortAudio to pick the best,
					possibly changing, buffer size.*/
		patestCallback, /* this is your callback function */
		NULL); /*This is a pointer that will be passed to
				your callback*/

	if (err != paNoError)
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));

	err = Pa_StartStream(stream);
	if (err != paNoError)
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));

	/* Sleep for several seconds. */
	Pa_Sleep(5 * 1000);

	err = Pa_StopStream(stream);
	if (err != paNoError)
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));

	err = Pa_CloseStream(stream);
	if (err != paNoError)
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));

	err = Pa_Terminate();
	if (err != paNoError)
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));

	std::getchar();
}