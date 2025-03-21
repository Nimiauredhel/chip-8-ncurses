#include "audio.h"

static AudioData_t audio_data = {0};

static int patestCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData)
{
    int8_t *out = (int8_t*)outputBuffer;
    unsigned int i;
    (void) inputBuffer; /* Prevent unused variable warning. */
    (void) userData;

    for( i=0; i<framesPerBuffer; i++ )
    {
        for (int j = 0; j < 2; j++) // left and right
        {
            *out++ = audio_data.phases[j];
            audio_data.phases[j] += audio_data.dirs[j];

            if (audio_data.phases[j] >= 127)
            {
                audio_data.phases[j] = 127;
                audio_data.dirs[j] *= -1;
            }
            else if (audio_data.phases[j] <= -128)
            {
                audio_data.phases[j] = -128;
                audio_data.dirs[j] *= -1;
            }
        }
    }
    return 0;
}

PaStream* init_audio(void)
{
    PaStream *new_stream;
    PaError err;

    audio_data.phases[0] = -128;
    audio_data.phases[1] = 127;
    audio_data.dirs[0] = 4;
    audio_data.dirs[1] = -4;

    err = Pa_Initialize();

    if(err != paNoError)
    {
        // TODO: handle error
    }

    err = Pa_OpenDefaultStream(&new_stream,
                               0,
                               2,
                               paInt8,
                               44100,
                               256,        
                               patestCallback,
                               NULL);
    if(err != paNoError)
    {
        // TODO: handle error
    }

    return new_stream;
}

void set_audio(PaStream *stream, bool active)
{
    PaError err = paNoError;
    bool was_active = Pa_IsStreamActive(stream);

    if (active != was_active)
    {
        if (active) err = Pa_StartStream(stream);
        else err = Pa_StopStream(stream);
    }

    if(err != paNoError)
    {
        // TODO: handle error
    }
}

void deinit_audio(PaStream *stream)
{
    /*
    PaError err;

    err = Pa_CloseStream(stream);
    err = Pa_Terminate();

    // TODO: handle PA errors ..
    */
}
