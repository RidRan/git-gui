#include <stdlib.h>

#define MINIAUDIO_IMPLEMENTATION
#include "..\include\miniaudio.h"

#define NOISE_PATH "..\\src\\RAT_SCREAM_1.wav"

void Noise() {
    ma_result result;
    ma_engine engine;

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.");
        return;
    }

    ma_engine_play_sound(&engine, NOISE_PATH, NULL);

    ma_engine_uninit(&engine);
}