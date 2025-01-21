#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>

#define NO_OF_RECTS 1
#define NO_OF_SPACES (NO_OF_RECTS + 1)
#define RECT_WIDTH 100
#define RECT_HEIGHT 100

#define TOTAL_WIDTH RECT_WIDTH * NO_OF_RECTS

#define START_X 20
#define START_Y 1024 / 2 - RECT_HEIGHT / 2
#define END_X 300
#define END_Y 300


#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

int normalize(int value, int min, int max);
float lerp(float norm, int min, int max);
void logger(int frameCounter);


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

    float t = 0;  // 0 < t < 1
    bool shouldAnimate = false;
    bool animationComplete = false;
    bool isAnimating = false;

    while(!WindowShouldClose()) {

        // Handle Input
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            printf("yes\n");
            isAnimating = true;
        }

        // Update
        frameCounter++;
        if (isAnimating) {
            t += .005;

            float result = lerp(t, 0, 500);
            position.x = result;
            position.y = result;

            // printf("time: %f\n", t);
            printf("%.2f\n", result);
        }


        // Collision
        for (int i = 0; i < NO_OF_RECTS; ++i) {

            int x = i * RECT_WIDTH + GAP * (i + 1);
            if (CheckCollisionPointRec((Vector2) {GetMouseX(), GetMouseY()}, (Rectangle) {position.x, position.y, RECT_WIDTH, RECT_HEIGHT})) {
                printf("yes %d\n", frameCounter);
            }

        }


        if (t > 1) {
            t = 0;
            isAnimating = false;
            shouldAnimate = false;
            animationComplete = true;

            position.x = START_X;
            position.y = START_Y;
        }

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
