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

float linearTween(float currentTime, float start, float change, float duration) {
    return change * currentTime / duration + start;
}


// typedef struct Sprite {
//     Texture2D texture;
//     Rectangle bounds;
//     Vector2 velocity;
//     // Vector2 acceleration;
// };

int main() {
    
    
    InitWindow(1024, 768, "Gaps");
    SetWindowMonitor(2);
    SetMousePosition(-1, -1);

    Vector2 position = { START_X, START_Y };
    Vector2 center = { GetScreenWidth() / 2, GetScreenHeight() / 2 };
    const int GAP = (GetScreenWidth() - (TOTAL_WIDTH)) / NO_OF_SPACES;

    SetTargetFPS(60);

    int frameCounter = 0;
    int state = 0;
    int duration = 240;     // 60 * 4 = 4 seconds

    float t = 0;  // 0 < t < 1
    bool shouldAnimate = false;
    bool animationComplete = false;
    bool isAnimating = false;

    while(!WindowShouldClose()) {

        // Handle Input
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            printf("yes\n");
        }

        // Update
        if (state == 0) {
            frameCounter++;
            position.x  = EaseLinearIn((float) frameCounter, START_X, center.x - RECT_WIDTH / 2 - START_X,  duration);
            position.y  = EaseLinearIn((float) frameCounter, START_Y, center.y - RECT_HEIGHT / 2 - START_Y,  duration);

            if (frameCounter > duration) {
                frameCounter = 0;
                state = 1;
            }
        }

        // Collision

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        // int y = center.y - RECT_HEIGHT / 2;
        // for (int i = 0; i < NO_OF_RECTS; ++i) {
        //     position.x = i * RECT_WIDTH + GAP * (i + 1);
        //     // DrawCircle(radius + i * radius * 2, center.y, radius, YELLOW);
        //     DrawRectangle(position.x, position.y, RECT_WIDTH, RECT_HEIGHT, BLUE);
        //     DrawRectangleLines(position.x, position.y, RECT_WIDTH, RECT_HEIGHT, WHITE);
        // }

        // Draw a single rect
        DrawRectangle(position.x, position.y, RECT_WIDTH, RECT_HEIGHT, BLUE);
        DrawRectangleLines(position.x, position.y, RECT_WIDTH, RECT_HEIGHT, WHITE);

        DrawText(TextFormat("x %.1f\ny %.1f", position.x, position.y), 20, 20, 50, WHITE);


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
