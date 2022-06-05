#include "output.c"

int frameCount = 0;
int frameCountMax = 60;

void start() {
    CreateScreen();
    CreateLayers();
}

void loop() {

    DrawLayers();

    frameCount++;
    if (frameCount == frameCountMax) {
        frameCount = 0;
    }
}