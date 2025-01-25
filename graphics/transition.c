#include "raylib.h"
#include "reasings.h"
#include <stdio.h>
#include <assert.h>

#define LOGO_DELAY_SECS 3
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

char* screenName(GameScreen screen) {
    if (screen == 0) return "Logo";
    if (screen == 1) return "Menu";
    if (screen == 2) return "Level";
    if (screen == 3) return "Game";
    if (screen == 4) return "GameOver";
    if (screen == 5) return "Transition Out";
    if (screen == 6) return "Transition In";
    return "Error";
}

void switchScreens(GameScreen *current, GameScreen next) {
    assert(next >= 0 && next <= 6);
    printf("Switch screen to %s\n", screenName(next));

    *current = next;
}

void handleInput(GameScreen *currentScreen) {
    // printf("currentScreen: %d\n", currentScreen);

    switch (*currentScreen) {
        case LOGO: {
            // Do nothing or click to skip?
        }
        break;
        case MENU: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                // printf("Menu\n");
                // *currentScreen = LEVEL;

                switchScreens(currentScreen, LEVEL);
            }
        }
        break;
        case LEVEL: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                switchScreens(currentScreen, GAME);
            }
        }
        break;
        case GAME: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                switchScreens(currentScreen, GAMEOVER);
            }
        }
        break;
        case GAMEOVER: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                switchScreens(currentScreen, MENU);
            }
        }
        break;
        default: break;
    }
}

void update(GameScreen *currentScreen, int *framesCounter) {
    switch (*currentScreen) {
        case LOGO: {
            double currentTime = GetTime();

            if (currentTime >= LOGO_DELAY_SECS) {
                switchScreens(currentScreen, MENU);
            }

            // // Previous way
            // (*framesCounter)++;    // Count frames

            // // Wait for 3 seconds (180 frames) before jumping to TITLE screen
            // if (*framesCounter > 180) {
            //     switchScreens(currentScreen, MENU);
            // }
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
            int countdown = (int)(LOGO_DELAY_SECS - GetTime()) + 1;
            int fontSize = 240;
            // char *num = 48 + countdown;
            int textW = MeasureText(TextFormat("%d", countdown), fontSize);
            int fontX = (GetScreenWidth() - textW) / 2;
            int fontY = GetScreenHeight() / 2 - 100;

            // Draw screen name
            DrawTextPro(GetFontDefault(), "Logo", textPosition, textOrigin, 0, 40, 20, BLACK);
            // Draw countdown
            DrawText(TextFormat("%d", countdown), fontX, fontY, fontSize, BLACK);
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
