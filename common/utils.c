#include "raylib.h"

// THESE SHOULD BE IN CONSTANTS FILE
const int INITIAL_SCREEN_WIDTH = 2880 / 3;
const int INITIAL_SCREEN_HEIGHT = 1920 / 3;
// Literal Window
int SCREEN_WIDTH = INITIAL_SCREEN_WIDTH;
int SCREEN_HEIGHT = INITIAL_SCREEN_HEIGHT;
// THE ACUTAL WORLD
int gameScreenWidth = INITIAL_SCREEN_WIDTH;
int gameScreenHeight = INITIAL_SCREEN_HEIGHT;

// Declarations...
// int normalize(int value, int min, int max);
// float lerp(float norm, int min, int max);
// float linearTween(float currentTime, float start, float change, float duration);
// void logger(int frameCounter);


// Methods here...
void HandleFullscreen() {
    if (IsKeyPressed(KEY_F)) {
        if (!IsWindowFullscreen()) {
            int monitor = GetCurrentMonitor();
            SCREEN_WIDTH  = GetMonitorWidth(monitor);
            SCREEN_HEIGHT = GetMonitorHeight(monitor);
            SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
            ToggleFullscreen();
        } else {
            ToggleFullscreen();
            SCREEN_WIDTH = INITIAL_SCREEN_WIDTH;
            SCREEN_HEIGHT = INITIAL_SCREEN_HEIGHT;
            SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
        }
    }
}

void Logger(int frameCounter) {
    printf("getFrameTime: %f\n", GetFrameTime());   // Time since last frame (dt)
    printf("getTime: %lf\n", GetTime());            // Elapsed time (seconds)
    printf("frameCounter: %d\n", frameCounter);     // Elapased frames
}

int Normalize(int value, int min, int max) {
    // value - min  = amount of value that lies within range
    // max - min    = range
    return (value - min) / (max - min);
}

float Lerp(float norm, int min, int max) {
    return (max - min) * norm + min;
}

float LinearTween(float currentTime, float start, float change, float duration) {
    return change * currentTime / duration + start;
}
