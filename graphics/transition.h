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

typedef struct GameUI {
    UIButtons left;
    UIButtons right;
} GameUI;

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


void inputMenu(Game *game, GameUI gameUI) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), gameUI.left.dest)) {
            // Do nothing
        }
        if (CheckCollisionPointRec(GetMousePosition(), gameUI.right.dest)) {
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
void inputLevel(Game *game, GameUI gameUI) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), gameUI.left.dest)) {
            switchScreens(&game->currentScreen, MENU, &game->framesCounter);
        }
        if (CheckCollisionPointRec(GetMousePosition(), gameUI.right.dest)) {
            switchScreens(&game->currentScreen, GAME, &game->framesCounter);
        }
    }
}
void inputGame(Game *game, GameUI gameUI) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), gameUI.left.dest)) {
            switchScreens(&game->currentScreen, LEVEL, &game->framesCounter);
        }
        if (CheckCollisionPointRec(GetMousePosition(), gameUI.right.dest)) {
            switchScreens(&game->currentScreen, GAMEOVER, &game->framesCounter);
        }
    }
}
void inputGameover(Game *game, GameUI gameUI) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), gameUI.left.dest)) {
            switchScreens(&game->currentScreen, GAME, &game->framesCounter);
        }
        if (CheckCollisionPointRec(GetMousePosition(), gameUI.right.dest)) {
            // Do nothing
        }
    }
}
void handleInput(Game *game, GameUI gameUI, Rectangle *bg) {
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
            inputMenu(game, gameUI);
        }
        break;
        case LEVEL: {
            inputLevel(game, gameUI);
        }
        break;
        case GAME: {
            inputGame(game, gameUI);
        }
        break;
        case GAMEOVER: {
            inputGameover(game, gameUI);
        }
        break;
        default: break;
    }
}



void updateLogo(Game *game, GameUI *gameUI) {
    (game->framesCounter)++;


    double currentTime = GetTime();
    if (currentTime >= LOGO_DELAY_SECS) {
        switchScreens(&game->currentScreen, MENU, &game->framesCounter);
    }
}
void updateMenu(Game *game, GameUI *gameUI) {
    (game->framesCounter)++;


    if (CheckCollisionPointRec(GetMousePosition(), gameUI->right.dest)) {
        gameUI->right.hover = true;
    } else {
        gameUI->right.hover = false;
    }
}
void updateLevel(Game *game, GameUI *gameUI) {
    (game->framesCounter)++;

    if (CheckCollisionPointRec(GetMousePosition(), gameUI->left.dest)) {
        gameUI->left.hover = true;
    } else {
        gameUI->left.hover = false;
    }
    if (CheckCollisionPointRec(GetMousePosition(), gameUI->right.dest)) {
        gameUI->right.hover = true;
    } else {
        gameUI->right.hover = false;
    }
}
void updateGame(Game *game, GameUI *gameUI) {
    (game->framesCounter)++;



    if (CheckCollisionPointRec(GetMousePosition(), gameUI->left.dest)) {
        gameUI->left.hover = true;
    } else {
        gameUI->left.hover = false;
    }
    if (CheckCollisionPointRec(GetMousePosition(), gameUI->right.dest)) {
        gameUI->right.hover = true;
    } else {
        gameUI->right.hover = false;
    }
}
void updateGameover(Game *game, GameUI *gameUI) {
    (game->framesCounter)++;

    if (CheckCollisionPointRec(GetMousePosition(), gameUI->left.dest)) {
        gameUI->left.hover = true;
    } else {
        gameUI->left.hover = false;
    }
}
void update(Game *game, GameUI *gameUI, Rectangle *bg) {
    switch (game->currentScreen) {
        case LOGO: {
            updateLogo(game, gameUI);
        }
        break;
        case MENU: {
            updateMenu(game, gameUI);
        }
        break;
        case LEVEL: {
            updateLevel(game, gameUI);
        }
        break;
        case GAME: {
            updateGame(game, gameUI);
        }
        break;
        case GAMEOVER: {
            updateGameover(game, gameUI);
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
void drawLogo() {
    ClearBackground(WHITE);
    int countdown = (int)(LOGO_DELAY_SECS - GetTime()) + 1;
    int fontSize = 240;
    // char *num = 48 + countdown;
    int textW = MeasureText(TextFormat("%d", countdown), fontSize);
    int fontX = (GetScreenWidth() - textW) / 2;
    int fontY = GetScreenHeight() / 2 - 100;

    // Draw screen name
    DrawTextPro(GetFontDefault(), "Logo", (Vector2) { 40, 40 }, (Vector2) { 0 }, 0, 40, 20, BLACK);
    // Draw countdown
    DrawText(TextFormat("%d", countdown), fontX, fontY, fontSize, BLACK);
}
void drawMenu(UIButtons buttonRight) {
    ClearBackground(BLUE);
    DrawTextPro(GetFontDefault(), "Menu", (Vector2) { 40, 40 }, (Vector2) { 0 }, 0, 40, 20, BLACK);
    drawButtonButton(buttonRight);
}
void drawLevel(UIButtons buttonLeft, UIButtons buttonRight) {
    ClearBackground(RED);
    DrawTextPro(GetFontDefault(), "Level", (Vector2) { 40, 40 }, (Vector2) { 0 }, 0, 40, 20, BLACK);
    drawButtons(buttonLeft, buttonRight);
}
void drawGame(UIButtons buttonLeft, UIButtons buttonRight) {
    ClearBackground(GREEN);
    DrawTextPro(GetFontDefault(), "Game", (Vector2) { 40, 40 }, (Vector2) { 0 }, 0, 40, 20, BLACK);
    drawButtons(buttonLeft, buttonRight);
}
void drawGameover(Game game, UIButtons buttonLeft) {
    ClearBackground(YELLOW);
    DrawTextPro(GetFontDefault(), "Gameover", (Vector2) { 40, 40 }, (Vector2) { 0 }, 0, 40, 20, BLACK);
    drawButtonButton(buttonLeft);

    // Draw blinking text
    if (((game.framesCounter)/30)%2 == 0) DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 + 80, 20, BLACK);
}
void draw(Game game, GameUI gameUI, Rectangle bg) {
    BeginDrawing();
    // ClearBackground(WHITE);

    // printf("%d\n", *framesCounter);

    switch (game.currentScreen) {
        case LOGO: {
            drawLogo();
        }
        break;
        case MENU: {
            drawMenu(gameUI.right);
        }
        break;
        case LEVEL: {
            drawLevel(gameUI.left, gameUI.right);
        }
        break;
        case GAME: {
            drawGame(gameUI.left, gameUI.right);
        }
        break;
        case GAMEOVER: {
            drawGameover(game, gameUI.left);
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

