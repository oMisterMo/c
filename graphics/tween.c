#include "raylib.h"
#include "reasings.h"

#include <stdio.h>
#include <stdbool.h>


#define NO_OF_RECTS 1
#define NO_OF_SPACES (NO_OF_RECTS + 1)
#define RECT_WIDTH 100
#define RECT_HEIGHT 100
#define TOTAL_WIDTH RECT_WIDTH * NO_OF_RECTS
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

typedef struct GameObject {
    Texture2D texture;
    Rectangle bounds;
    Tween tween;
} GameObject;

int normalize(int value, int min, int max);
float lerp(float norm, int min, int max);
float linearTween(float currentTime, float start, float change, float duration);
void logger(int frameCounter);

void handleInput(Rectangle *rect, int *state, Vector2 center, GameObject *mo);
void updateMo(GameObject *mo);
void drawMo(GameObject mo);
void updateRect(Rectangle *rect, Vector2 start, Vector2 end, int *frameCounter, int *state, Vector2 center, int duration);
void drawRect(Rectangle rect);

int main() {

    // ---------------------------------------------
    // Setup
    // ---------------------------------------------
    InitWindow(1024, 768, "Tween");
    SetMousePosition(-1, -1);
    const int GAP = (GetScreenWidth() - (TOTAL_WIDTH)) / NO_OF_SPACES;
    Vector2 center = { GetScreenWidth() / 2, GetScreenHeight() / 2 };


    // ---------------------------------------------
    // Initialize
    // ---------------------------------------------
    Texture2D bunny = LoadTexture("resources/sprites/piece.png");

    // Mo stuff
    Rectangle pos = { 0, center.y  - bunny.height / 2, bunny.width, bunny.height };
    Vector2 startMo = { pos.x, pos.y };
    Vector2 endMo = { GetScreenWidth() - bunny.width , center.y };

    Tween tween = { 0 };
    tween.startPosition = startMo;
    tween.targetPosition = endMo;
    tween.state = IDLE;
    tween.frameCounter = 0;
    tween.duration = 60 * 2;

    GameObject mo = { 0 };
    mo.texture = bunny;
    mo.bounds = pos;
    mo.tween = tween;

    // Rectangle stuff
    Vector2 startRect = { 20, GetScreenHeight() - RECT_HEIGHT - 20 };
    Vector2 endRect = { GetScreenWidth() - RECT_WIDTH - 20, startRect.y };
    Rectangle rect = { startRect.x, startRect.y, RECT_WIDTH, RECT_HEIGHT };
    int frameCounter = 0;
    int state = IDLE;
    int duration = 60 * 4;          // 60 * 4 = 4 seconds
    float t = 0;                    // 0 < t < 1
    printf("original rect => %p\n", &rect);



    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        // ---------------------------------------------
        // Input
        // ---------------------------------------------
        handleInput(&rect, &state, center, &mo);

        // ---------------------------------------------
        // Update
        // ---------------------------------------------
        updateRect(&rect, startRect, endRect, &frameCounter, &state, center, duration);
        updateMo(&mo);

        // ---------------------------------------------
        // Draw
        // ---------------------------------------------
        BeginDrawing();
        ClearBackground(SKYBLUE);

        drawRect(rect);
        drawMo(mo);

        EndDrawing();
    }

    // ---------------------------------------------
    // Destroy
    // ---------------------------------------------
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

void handleInput(Rectangle *rect, int *state, Vector2 center, GameObject *mo) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        printf("left mouse pressed...\n");
        if (CheckCollisionPointRec(GetMousePosition(), mo->bounds)) {
            // reset mo
            mo->tween.state = TWEENING;
            return;
        }
        if (CheckCollisionPointRec(GetMousePosition(), *rect)) {
            // reset rect
            *state = TWEENING;
            rect->x = 0;
            rect->y = center.y;
            return;
        }

        // Reset all
        // reset rect
        *state = TWEENING;
        rect->x = 0;
        rect->y = center.y;

        // reset mo
        mo->tween.state = TWEENING;
    }
}

void updateMo(GameObject *mo) {
        if (mo->tween.state == TWEENING) {
            mo->tween.frameCounter++;

            // Tween
            mo->bounds.x  = EaseElasticOut(
                (float) mo->tween.frameCounter,
                mo->tween.startPosition.x,
                mo->tween.targetPosition.x - mo->tween.startPosition.x,
                mo->tween.duration
            );

            // Tween complete
            if (mo->tween.frameCounter >= mo->tween.duration) {
                mo->tween.frameCounter = 0;
                mo->tween.state = IDLE;


                // Set final position
                mo->bounds.x = mo->tween.startPosition.x;
            }
        }
}

void drawMo(GameObject mo) {
    // Draw Mo
    // DrawRectangleRec(mo.bounds, WHITE);
    DrawTextureV(mo.texture, (Vector2) { mo.bounds.x, mo.bounds.y }, WHITE);
    DrawText(TextFormat("x %.1f\ny %.1f", mo.bounds.x, mo.bounds.y), mo.bounds.x + mo.texture.width / 2 - 50, mo.bounds.y - 50, 20, WHITE);
}

void updateRect(Rectangle *rect, Vector2 start, Vector2 end, int *frameCounter, int *state, Vector2 center, int duration) {
    // printf("rect => %p\n", rect);
    if (*state == TWEENING) {

        (*frameCounter)++;

        rect->x  = EaseCircIn((float) *frameCounter, start.x, end.x - start.x,  duration);
        rect->y  = EaseCircIn((float) *frameCounter, start.y, end.y - start.y,  duration);

        if (*frameCounter >= duration) {
            *frameCounter = 0;
            *state = IDLE;

            rect->x = start.x;
        }
    }
}

void drawRect(Rectangle rect) {
    // Draw a single rect
    DrawRectangleRec(rect, ORANGE);
    DrawRectangleLinesEx(rect, 2, WHITE);
    DrawText(TextFormat("x %.1f\ny %.1f", rect.x, rect.y), rect.x + 5, rect.y - 50, 20, WHITE);
}