#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>
#include "common.h"
#include "portaudio.h"

typedef struct AudioData
{
    int8_t phases[2];
    int8_t dirs[2];
} AudioData_t;

PaStream* init_audio(void);
void set_audio(PaStream *stream, bool active);
void deinit_audio(PaStream *stream);

#endif
