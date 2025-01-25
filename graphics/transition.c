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
    TRANSITION_START,
    TRANSITION_END
} GameScreen;

typedef enum {
    UI_BACK = 0,
    UI_RETRY,
    UI_RANDOM,
    UI_SKIP,
    UI_LEFT,
    UI_RIGHT
} UIButton;

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

typedef struct UIButtons {
    Texture2D texture;
    Rectangle src;
    Rectangle dest;
} UIButtons;

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
    printf("------------------------------\n");
    printf("Switch screen: %s\n", screenName(next));
    printf("------------------------------\n");

    *current = next;
}

void handleInput(GameScreen *currentScreen, Rectangle buttonLeftDest, Rectangle buttonRightDest) {
    // printf("currentScreen: %d\n", currentScreen);

    if (*currentScreen == TRANSITION_START || *currentScreen == TRANSITION_END) {
        // printf("No Touching...!\n");
        return;
    }

    switch (*currentScreen) {
        case LOGO: {
            // Do nothing or click to skip?
        }
        break;
        case MENU: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(GetMousePosition(), buttonLeftDest)) {
                    // switchScreens(currentScreen, LEVEL);
                }
                if (CheckCollisionPointRec(GetMousePosition(), buttonRightDest)) {
                    switchScreens(currentScreen, TRANSITION_START);
                    // switchScreens(currentScreen, LEVEL);
                }
            }
        }
        break;
        case LEVEL: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(GetMousePosition(), buttonLeftDest)) {
                    switchScreens(currentScreen, MENU);
                }
                if (CheckCollisionPointRec(GetMousePosition(), buttonRightDest)) {
                    switchScreens(currentScreen, GAME);
                }
            }
        }
        break;
        case GAME: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(GetMousePosition(), buttonLeftDest)) {
                    switchScreens(currentScreen, LEVEL);
                }
                if (CheckCollisionPointRec(GetMousePosition(), buttonRightDest)) {
                    switchScreens(currentScreen, GAMEOVER);
                }
            }
        }
        break;
        case GAMEOVER: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(GetMousePosition(), buttonLeftDest)) {
                    switchScreens(currentScreen, GAME);
                }
                if (CheckCollisionPointRec(GetMousePosition(), buttonRightDest)) {
                    // switchScreens(currentScreen, MENU);
                }
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

    if (*currentScreen == TRANSITION_START) {

    }
    if (*currentScreen == TRANSITION_END) {
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
}

void drawButtonButton(UIButtons button) {
    DrawTexturePro(button.texture, button.src, button.dest, (Vector2) {0}, 0, WHITE);
}
void drawButtons(UIButtons buttonLeft, UIButtons buttonRight) {
    drawButtonButton(buttonLeft);
    drawButtonButton(buttonRight);
}

void draw(int currentScreen, Vector2 textPosition, Vector2 textOrigin, UIButtons buttonLeft, UIButtons buttonRight) {
    BeginDrawing();
    // ClearBackground(WHITE);

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
            drawButtonButton(buttonRight);
        }
        break;
        case LEVEL: {
            ClearBackground(RED);
            DrawTextPro(GetFontDefault(), "Level", textPosition, textOrigin, 0, 40, 20, BLACK);
            drawButtons(buttonLeft, buttonRight);
        }
        break;
        case GAME: {
            ClearBackground(GREEN);
            DrawTextPro(GetFontDefault(), "Game", textPosition, textOrigin, 0, 40, 20, BLACK);
            drawButtons(buttonLeft, buttonRight);
        }
        break;
        case GAMEOVER: {
            ClearBackground(YELLOW);
            DrawTextPro(GetFontDefault(), "Gameover", textPosition, textOrigin, 0, 40, 20, BLACK);
            drawButtonButton(buttonLeft);
        }
        break;
        default: break;
    }

    // Transition
    if (currentScreen == TRANSITION_START) {
        DrawRectangle(GetScreenWidth() / 2, 0, GetScreenWidth(), GetScreenHeight(), PINK);
    }
    if (currentScreen == TRANSITION_END) {
        DrawRectangle(GetScreenWidth() / 2, 0, GetScreenWidth(), GetScreenHeight(), ORANGE);
        // DrawRectanglePro((Rectangle) {destRect.x, destRect.y, destRect.width, destRect.height}, origin, 0, WHITE);
        // DrawTexturePro(transitionTexture, sourceRec, destRect, origin, rotation, ORANGE);
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

    // Buttons
    Texture2D buttonsTexture = LoadTexture("resources/ui/buttons_navigation.png");
    buttonsTexture.width *= 0.3;
    buttonsTexture.height *= 0.3;
    int BUTTON_W = buttonsTexture.width / 6;
    int BUTTON_H = buttonsTexture.height / 2;
    Rectangle buttons[12];
    // Regular buttons
    for (int i = 0; i < 6; i++) {
        buttons[i] = (Rectangle) {
            (float) i * buttonsTexture.width / 6,
            0,
            (float) buttonsTexture.width / 6,
            (float) buttonsTexture.height / 2
        };
    }
    // Hover button (i + 6)
    for (int i = 6; i < 12; i++) {
        buttons[i] = (Rectangle) {
            (float) i * buttonsTexture.width / 6,
            buttonsTexture.height / 2,
            (float) buttonsTexture.width / 6,
            (float) buttonsTexture.height / 2
        };
    }
    Rectangle buttonLeftDest = { 0, (GetScreenHeight() - BUTTON_H ) / 2, BUTTON_W, BUTTON_H };
    Rectangle buttonRightDest = { GetScreenWidth() - BUTTON_W, (GetScreenHeight() - BUTTON_H ) / 2, BUTTON_W, BUTTON_H };
    UIButtons buttonLeft = { buttonsTexture, buttons[UI_LEFT], buttonLeftDest };
    UIButtons buttonRight = { buttonsTexture, buttons[UI_RIGHT], buttonRightDest };

    // Tween
    Tween tween = { (Vector2) {}, (Vector2) {}, TWEENING, 0, 60 * 3 };
    int frameCounter = 0;
    float duration = 60 * 1.5;
    int state = TWEENING;

    Vector2 textPosition = { 40, 40 };
    Vector2 textOrigin = { 0 };
    GameScreen currentScreen = MENU;



    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        // Handle input
        handleInput(&currentScreen, buttonLeftDest, buttonRightDest);

        // Update
        update(&currentScreen, &frameCounter);

        // Draw
        draw(currentScreen, textPosition, textOrigin, buttonLeft, buttonRight);
    }


    UnloadTexture(transitionTexture);
    UnloadTexture(buttonsTexture);
    CloseWindow();
    return 0;
}
