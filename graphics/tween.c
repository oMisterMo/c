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

typedef enum {
    IDLE = 0,
    TWEENING
} TweenState;

typedef struct Tween {
    Vector2 startPosition;      // Tween start
    Vector2 targetPosition;     // Tween end [could be consts]
    int state;                  // IDLE | TWEENING
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

typedef struct Item {
    Texture2D texture;
    Vector2 position;
    Tween tween;
} Item;

int normalize(int value, int min, int max);
float lerp(float norm, int min, int max);
void logger(int frameCounter);
float linearTween(float currentTime, float start, float change, float duration);

void handleInput(int *state, Rectangle rect, Vector2 center, Item *mo) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        printf("yes\n");
        // reset original
        *state = 0;
        rect.x = 0;
        rect.y = center.y;

        // reset mo
        printf("=> %p\n", mo);
        mo->tween.state = TWEENING;
    }
}

void updateMo(Item *mo) {
        if (mo->tween.state == TWEENING) {
            mo->tween.frameCounter++;

            // Tween
            mo->position.x  = EaseElasticInOut(
                (float) mo->tween.frameCounter,
                mo->tween.startPosition.x,
                mo->tween.targetPosition.x - mo->tween.startPosition.x,
                mo->tween.duration
            );

            // Tween complete
            if (mo->tween.frameCounter > mo->tween.duration) {
                mo->tween.frameCounter = 0;
                mo->tween.state = IDLE;


                printf("Final pos x: %f\n", mo->position.x);
                mo->position.x = mo->tween.targetPosition.x;
                printf("Final pos x: %f\n", mo->position.x);
            }
        }
}

void drawMo(Item mo) {
    // Draw Mo
    DrawTextureV(mo.texture, mo.position, WHITE);
}

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
    Texture2D bunny = LoadTexture("resources/sprites/piece.png");

    Vector2 center = { GetScreenWidth() / 2, GetScreenHeight() / 2 };
    Rectangle rect = { START_X, START_Y, RECT_WIDTH, RECT_HEIGHT };

    Vector2 pos = { 0, center.y  - bunny.height / 2 };
    Tween leftToRight = {
            pos,                      // start
            (Vector2) { (GetScreenWidth() - bunny.width) , center.y },      // end
            IDLE,
            0,
            60 * 3                                                              // 3 seconds
         };


    Item mo = { bunny, pos, leftToRight };
    printf("original => %p\n", &mo);
    


    int frameCounter = 0;
    int state = 0;
    int duration = 240;     // 60 * 4 = 4 seconds
    float t = 0;  // 0 < t < 1



    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        // ---------------------------------------------
        // Input
        // ---------------------------------------------
        handleInput(&state, rect, center, &mo);

        // ---------------------------------------------
        // Update
        // ---------------------------------------------
        if (state == 0) {
            frameCounter++;
            rect.x  = EaseLinearIn((float) frameCounter, START_X, center.x - RECT_WIDTH / 2 - START_X,  duration);
            rect.y  = EaseLinearIn((float) frameCounter, START_Y, center.y - RECT_HEIGHT / 2 - START_Y,  duration);

            if (frameCounter > duration) {
                frameCounter = 0;
                state = 1;
            }
        }

        updateMo(&mo);

        // ---------------------------------------------
        // Draw
        // ---------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);




        // Draw a single rect
        DrawRectangleRec(rect, BLUE);
        DrawRectangleLinesEx(rect, 2, WHITE);

        drawMo(mo);




        DrawText(TextFormat("x %.1f\ny %.1f", rect.x, rect.y), 20, 20, 50, WHITE);
        EndDrawing();
    }


    UnloadTexture(bunny);
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
