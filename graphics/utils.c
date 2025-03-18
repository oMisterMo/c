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