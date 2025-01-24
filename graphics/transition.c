#include "raylib.h"
#include "reasings.h"
#include <stdio.h>

#define MIN_SCALE 0
#define MAX_SCALE 2

typedef enum {
    LOGO = 0,
    MENU,
    LEVEL,
    GAME,
    GAMEOVER
} State;

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

int main() {

    int screenWidth = 960;
    int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Transition");
    SetWindowMonitor(2);

    // Texture
    Texture2D transitionTexture = LoadTexture("resources/ui/transition.png");
    int rotation = 0;
    float scale = 0.01;
    transitionTexture.width *= scale;
    transitionTexture.height *= scale;
    Rectangle sourceRec = { 0, 0, transitionTexture.width, transitionTexture.height };  // multiply width by -1 to flip
    Rectangle destRect = { screenWidth / 2, screenHeight / 2, transitionTexture.width, transitionTexture.height };
    Vector2 origin = { (float) transitionTexture.width / 2, (float) transitionTexture.height / 2 };

    // Tween
    Tween tween = { (Vector2) {}, (Vector2) {}, TWEENING, 0, 60 * 3 };
    int frameCounter = 0;
    float duration = 60 * 1.5;
    int state = TWEENING;



    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        frameCounter++;
        // Handle input


        // Update
        if (state == TWEENING) {
            scale = EaseSineOut((float)frameCounter, 0, 70, duration);
            destRect.width  += scale;
            destRect.height += scale;
            origin.x = (float) destRect.width / 2;
            origin.y = (float) destRect.height / 2;
            // printf("%.1f,%.1f\n", destRect.width, destRect.height);
            printf("scale: %f\n", scale);
            if (frameCounter > duration) {
                frameCounter = 0;
                scale = 70.0;
                state = IDLE;
                printf("scale: %f\n", scale);
                printf("Done.\n");
            }
        }
        if (state == IDLE) {

        }




        // Draw
        BeginDrawing();
        ClearBackground(ORANGE);

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ORANGE);
        DrawRectanglePro((Rectangle) {destRect.x, destRect.y, destRect.width, destRect.height}, origin, 0, WHITE);
        DrawTexturePro(transitionTexture, sourceRec, destRect, origin, rotation, ORANGE);


        EndDrawing();
    }


    UnloadTexture(transitionTexture);
    CloseWindow();
    return 0;
}
