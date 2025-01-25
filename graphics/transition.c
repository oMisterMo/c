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
    GAMEOVER,
    TRANSITION_OUT,
    TRANSITION_IN
} GameScreen;

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


void handleInput(GameScreen *currentScreen) {
    // printf("currentScreen: %d\n", currentScreen);

    switch (*currentScreen) {
        case LOGO: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                printf("Logo\n");
                *currentScreen = MENU;
            }
        }
        break;
        case MENU: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                printf("Menu\n");
                *currentScreen = LEVEL;
            }
        }
        break;
        case LEVEL: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                printf("Level\n");
                *currentScreen = GAME;
            }
        }
        break;
        case GAME: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                printf("Game\n");
                *currentScreen = GAMEOVER;
            }
        }
        break;
        case GAMEOVER: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                printf("Gameover\n");
                *currentScreen = MENU;
            }
        }
        break;
        default: break;
    }
}

void update(GameScreen *currentScreen, int *framesCounter) {
    switch (*currentScreen) {
        case LOGO: {
            (*framesCounter)++;    // Count frames

            // Wait for 3 seconds (180 frames) before jumping to TITLE screen
            if (*framesCounter > 180) {
                *currentScreen = MENU;
            }
        }
        break;
        case MENU: {

        }
        break;
        case LEVEL: {

        }
        break;
        case GAME: {

        }
        break;
        case GAMEOVER: {

        }
        break;
        default: break;
    }

    if (TRANSITION_IN) {
        // if (state == TWEENING) {
        //     scale = EaseSineOut((float)frameCounter, 0, 70, duration);
        //     destRect.width  += scale;
        //     destRect.height += scale;
        //     origin.x = (float) destRect.width / 2;
        //     origin.y = (float) destRect.height / 2;
        //     // printf("%.1f,%.1f\n", destRect.width, destRect.height);
        //     printf("scale: %f\n", scale);
        //     if (frameCounter > duration) {
        //         frameCounter = 0;
        //         scale = 70.0;
        //         state = IDLE;
        //         printf("scale: %f\n", scale);
        //         printf("Done.\n");
        //     }
        // }
        // if (state == IDLE) {

        // }
    }
    if (TRANSITION_OUT) {

    }
}

void draw(int currentScreen, Vector2 textPosition, Vector2 textOrigin) {
    BeginDrawing();
    ClearBackground(WHITE);


    switch (currentScreen) {
        case LOGO: {
            ClearBackground(WHITE);
            DrawTextPro(GetFontDefault(), "Logo", textPosition, textOrigin, 0, 40, 20, BLACK);
        }
        break;
        case MENU: {
            ClearBackground(BLUE);
            DrawTextPro(GetFontDefault(), "Menu", textPosition, textOrigin, 0, 40, 20, BLACK);
        }
        break;
        case LEVEL: {
            ClearBackground(RED);
            DrawTextPro(GetFontDefault(), "Level", textPosition, textOrigin, 0, 40, 20, BLACK);
        }
        break;
        case GAME: {
            ClearBackground(GREEN);
            DrawTextPro(GetFontDefault(), "Game", textPosition, textOrigin, 0, 40, 20, BLACK);
        }
        break;
        case GAMEOVER: {
            ClearBackground(YELLOW);
            DrawTextPro(GetFontDefault(), "Gameover", textPosition, textOrigin, 0, 40, 20, BLACK);
        }
        break;
        default: break;
    }

    // Transition
    if (TRANSITION_IN) {
        // DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ORANGE);
        // DrawRectanglePro((Rectangle) {destRect.x, destRect.y, destRect.width, destRect.height}, origin, 0, WHITE);
        // DrawTexturePro(transitionTexture, sourceRec, destRect, origin, rotation, ORANGE);
    }
    if (TRANSITION_OUT) {

    }

    EndDrawing();
}


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

    Vector2 textPosition = { 40, 40 };
    Vector2 textOrigin = { 0 };
    GameScreen currentScreen = LOGO;



    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        // Handle input
        handleInput(&currentScreen);

        // Update
        update(&currentScreen, &frameCounter);

        // Draw
        draw(currentScreen, textPosition, textOrigin);
    }


    UnloadTexture(transitionTexture);
    CloseWindow();
    return 0;
}
