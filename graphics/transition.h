#include <stdio.h>
#include <assert.h>

#define LOGO_DELAY_SECS 3

typedef enum {
    LOADING = 0,
    LOGO,
    MENU,
    LEVEL,
    GAME,
    GAMEOVER
} GameScreen;

typedef enum {
    TRANISITON_IDLE = 0,
    TRANSITION_START,
    TRANSITION_END
} TransitionState;

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
    Texture2D *texture;
    Rectangle src;
    Rectangle hoverSrc;
    Rectangle dest;
    bool hover;
} UIButtons;

typedef struct Game {
    GameScreen currentScreen;
    TransitionState currentTransition;
    int framesCounter;
} Game;

char* screenName(GameScreen screen) {
    if (screen == 0) return "Loading";
    if (screen == 1) return "Logo";
    if (screen == 2) return "Menu";
    if (screen == 3) return "Level";
    if (screen == 4) return "Game";
    if (screen == 5) return "GameOver";
    // if (screen == 5) return "Transition Out";
    // if (screen == 6) return "Transition In";
    return "Error";
}

void switchScreens(GameScreen *current, GameScreen next, int *framesCounter) {
    assert(next >= 0 && next <= 6);
    printf("------------------------------\n");
    printf("Switch screen: %s\n", screenName(next));
    printf("------------------------------\n");
    *framesCounter = 0;
    *current = next;
}

void handleInput(Game *game, UIButtons buttonLeft, UIButtons buttonRight, Rectangle *bg) {
    // printf("currentScreen: %d\n", currentScreen);

    if (game->currentTransition == TRANSITION_START || game->currentTransition == TRANSITION_END) {
        printf("No Touching...!\n");
        return;
    }

    switch (game->currentScreen) {
        case LOGO: {
            // Do nothing or click to skip?
        }
        break;
        case MENU: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(GetMousePosition(), buttonLeft.dest)) {
                    // Do nothing
                }
                if (CheckCollisionPointRec(GetMousePosition(), buttonRight.dest)) {
                    switchScreens(&game->currentScreen, LEVEL, &game->framesCounter);
                }
            }

            if (IsKeyPressed(KEY_Q)) {
                printf("Q\n");
            }
            if (IsKeyPressed(KEY_W)) {
                printf("W\n");

            }
        }
        break;
        case LEVEL: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(GetMousePosition(), buttonLeft.dest)) {
                    switchScreens(&game->currentScreen, MENU, &game->framesCounter);
                }
                if (CheckCollisionPointRec(GetMousePosition(), buttonRight.dest)) {
                    switchScreens(&game->currentScreen, GAME, &game->framesCounter);
                }
            }
        }
        break;
        case GAME: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(GetMousePosition(), buttonLeft.dest)) {
                    switchScreens(&game->currentScreen, LEVEL, &game->framesCounter);
                }
                if (CheckCollisionPointRec(GetMousePosition(), buttonRight.dest)) {
                    switchScreens(&game->currentScreen, GAMEOVER, &game->framesCounter);
                }
            }
        }
        break;
        case GAMEOVER: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(GetMousePosition(), buttonLeft.dest)) {
                    switchScreens(&game->currentScreen, GAME, &game->framesCounter);
                }
                if (CheckCollisionPointRec(GetMousePosition(), buttonRight.dest)) {
                    // Do nothing
                }
            }
        }
        break;
        default: break;
    }
}

void update(Game *game, UIButtons *buttonLeft, UIButtons *buttonRight, Rectangle *bg) {
    switch (game->currentScreen) {
        case LOGO: {
            (game->framesCounter)++;


            double currentTime = GetTime();

            if (currentTime >= LOGO_DELAY_SECS) {
                switchScreens(&game->currentScreen, MENU, &game->framesCounter);
            }
        }
        break;
        case MENU: {
            (game->framesCounter)++;


            if (CheckCollisionPointRec(GetMousePosition(), buttonRight->dest)) {
                buttonRight->hover = true;
            } else {
                buttonRight->hover = false;
            }
        }
        break;
        case LEVEL: {
            (game->framesCounter)++;

            if (CheckCollisionPointRec(GetMousePosition(), buttonLeft->dest)) {
                buttonLeft->hover = true;
            } else {
                buttonLeft->hover = false;
            }
            if (CheckCollisionPointRec(GetMousePosition(), buttonRight->dest)) {
                buttonRight->hover = true;
            } else {
                buttonRight->hover = false;
            }
        }
        break;
        case GAME: {
            (game->framesCounter)++;



            if (CheckCollisionPointRec(GetMousePosition(), buttonLeft->dest)) {
                buttonLeft->hover = true;
            } else {
                buttonLeft->hover = false;
            }
            if (CheckCollisionPointRec(GetMousePosition(), buttonRight->dest)) {
                buttonRight->hover = true;
            } else {
                buttonRight->hover = false;
            }
        }
        break;
        case GAMEOVER: {
            (game->framesCounter)++;

            if (CheckCollisionPointRec(GetMousePosition(), buttonLeft->dest)) {
                buttonLeft->hover = true;
            } else {
                buttonLeft->hover = false;
            }
        }
        break;
        default: break;
    }

    // if (*currentScreen == TRANSITION_START) {
    //     (*framesCounter)++;
    // }
    // if (*currentScreen == TRANSITION_END) {
    //     (*framesCounter)++;
    //     // if (state == TWEENING) {
    //     //     scale = EaseSineOut((float)framesCounter, 0, 70, duration);
    //     //     destRect.width  += scale;
    //     //     destRect.height += scale;
    //     //     origin.x = (float) destRect.width / 2;
    //     //     origin.y = (float) destRect.height / 2;
    //     //     // printf("%.1f,%.1f\n", destRect.width, destRect.height);
    //     //     printf("scale: %f\n", scale);
    //     //     if (framesCounter > duration) {
    //     //         framesCounter = 0;
    //     //         scale = 70.0;
    //     //         state = IDLE;
    //     //         printf("scale: %f\n", scale);
    //     //         printf("Done.\n");
    //     //     }
    //     // }
    //     // if (state == IDLE) {

    //     // }
    // }
}

void drawButtonButton(UIButtons button) {
    if (button.hover) {
        DrawTexturePro(*button.texture, button.hoverSrc, button.dest, (Vector2) {0}, 0, WHITE);
    } else {
        DrawTexturePro(*button.texture, button.src, button.dest, (Vector2) {0}, 0, WHITE);
    }
}
void drawButtons(UIButtons buttonLeft, UIButtons buttonRight) {
    drawButtonButton(buttonLeft);
    drawButtonButton(buttonRight);
}

void draw(Game game, Vector2 textPosition, Vector2 textOrigin, UIButtons buttonLeft, UIButtons buttonRight, Rectangle bg) {
    BeginDrawing();
    // ClearBackground(WHITE);

    // printf("%d\n", *framesCounter);

    switch (game.currentScreen) {
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

            // Draw blinking text
            if (((game.framesCounter)/30)%2 == 0) DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 + 80, 20, BLACK);
        }
        break;
        default: break;
    }

    // Transition
    if (game.currentTransition == TRANSITION_START) {
        DrawRectangle(bg.x, bg.y, bg.width, bg.height, PINK);
    }
    if (game.currentTransition == TRANSITION_END) {
        DrawRectangle(bg.x, bg.y, bg.width, bg.height, ORANGE);
        // DrawRectangle(GetScreenWidth() / 2, 0, GetScreenWidth(), GetScreenHeight(), ORANGE);
        // DrawRectanglePro((Rectangle) {destRect.x, destRect.y, destRect.width, destRect.height}, origin, 0, WHITE);
        // DrawTexturePro(transitionTexture, sourceRec, destRect, origin, rotation, ORANGE);
    }

    EndDrawing();
}

