#include "raylib.h"
#include "reasings.h"

#include <stdio.h>
#include <stdbool.h>


#define NO_OF_RECTS 1
#define NO_OF_SPACES (NO_OF_RECTS + 1)
#define RECT_WIDTH 100
#define RECT_HEIGHT 100
#define TOTAL_WIDTH RECT_WIDTH * NO_OF_RECTS
#define START_X 20
#define START_Y 100
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

int normalize(int value, int min, int max);
float lerp(float norm, int min, int max);
void logger(int frameCounter);
float linearTween(float currentTime, float start, float change, float duration);

typedef enum {
    FINISHED = 0,
    TWEENING
} TweenState;

typedef struct Tween {
    int state;                  // FINISHED | TWEENING
    Vector2 targetPosition;     // Could be consts
    Vector2 currentPosition;    // Tween start position
    int frameCounter;           // Current time in tween
    int duration;               // How long to tween
} Tween;

typedef struct Sprite {
    Texture2D texture;
    Rectangle bounds;
    Vector2 velocity;
    // Vector2 acceleration;
    Tween tween;
} Sprite;

int main() {

    // ---------------------------------------------
    // Setup
    // ---------------------------------------------
    InitWindow(1024, 768, "Tween");
    SetWindowMonitor(2);
    SetMousePosition(-1, -1);
    const int GAP = (GetScreenWidth() - (TOTAL_WIDTH)) / NO_OF_SPACES;


    // ---------------------------------------------
    // Initialize
    // ---------------------------------------------
    Vector2 center = { GetScreenWidth() / 2, GetScreenHeight() / 2 };
    Rectangle item = { START_X, START_Y, RECT_WIDTH, RECT_HEIGHT };
    




    int frameCounter = 0;
    int state = 0;
    int duration = 240;     // 60 * 4 = 4 seconds
    float t = 0;  // 0 < t < 1



    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        // ---------------------------------------------
        // Input
        // ---------------------------------------------
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            printf("yes\n");
            state = 0;
            item.x = 0;
            item.y = center.y;
        }

        // ---------------------------------------------
        // Update
        // ---------------------------------------------
        if (state == 0) {
            frameCounter++;
            item.x  = EaseLinearIn((float) frameCounter, START_X, center.x - RECT_WIDTH / 2 - START_X,  duration);
            item.y  = EaseLinearIn((float) frameCounter, START_Y, center.y - RECT_HEIGHT / 2 - START_Y,  duration);

            if (frameCounter > duration) {
                frameCounter = 0;
                state = 1;
            }
        }

        // ---------------------------------------------
        // Draw
        // ---------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);




        // Draw a single rect
        DrawRectangleRec(item, BLUE);
        DrawRectangleLinesEx(item, 2, WHITE);




        DrawText(TextFormat("x %.1f\ny %.1f", item.x, item.y), 20, 20, 50, WHITE);
        EndDrawing();
    }


    CloseWindow();
}

void logger(int frameCounter) {
    printf("getFrameTime: %f\n", GetFrameTime());   // Time since last frame (dt)
    printf("getTime: %lf\n", GetTime());            // Elapsed time (seconds)
    printf("frameCounter: %d\n", frameCounter);     // Elapased frames
}

int normalize(int value, int min, int max) {
    // value - min  = amount of value that lies within range
    // max - min    = range
    return (value - min) / (max - min);
}

float lerp(float norm, int min, int max) {
    return (max - min) * norm + min;
}

float linearTween(float currentTime, float start, float change, float duration) {
    return change * currentTime / duration + start;
}
