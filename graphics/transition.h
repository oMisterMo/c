#include <stdio.h>
#include <assert.h>

#define LOADING_DELAY 2.5f
#define LOGO_DELAY_SECS 3

#define FADE_TIME 1.5f
#define FADE_INCREMENT 0.05f        // Base increment for linear fade

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
    TweenState state;           // IDLE | TWEENING
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
    Texture2D *flower;
} GameUI;

typedef struct Game {
    GameScreen currentScreen;
    GameScreen nextScreen;
    TransitionState currentTransition;
    int framesCounter;
    float frameEntered;
    float logoCounter;      // I do not need this now that I have frameEntered
} Game;

char* screenName(GameScreen screen) {
    if (screen == 0) return "Loading";
    if (screen == 1) return "Logo";
    if (screen == 2) return "Menu";
    if (screen == 3) return "Level";
    if (screen == 4) return "Game";
    if (screen == 5) return "Game Over";
    // if (screen == 5) return "Transition Out";
    // if (screen == 6) return "Transition In";
    return "Error";
}

void switchScreens(Game *game, GameScreen next) {
    assert(next >= LOADING && next <= GAMEOVER);
    printf("------------------------------\n");
    printf("Switch screen: %s\n", screenName(next));
    printf("Entered at %.2f\n", GetTime());
    printf("------------------------------\n");

    // game->framesCounter = 0; // But I need this, think about it
    // game->frameEntered = GetTime();
    // game->currentScreen = next;  // I dont need to trasision statight away

    game->currentTransition = TRANSITION_START;
    game->nextScreen = next;
}


void inputMenu(Game *game, GameUI gameUI) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), gameUI.left.dest)) {
            // Do nothing
        }
        if (CheckCollisionPointRec(GetMousePosition(), gameUI.right.dest)) {
            switchScreens(game, LEVEL);
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
            switchScreens(game, MENU);
        }
        if (CheckCollisionPointRec(GetMousePosition(), gameUI.right.dest)) {
            switchScreens(game, GAME);
        }
    }
}
void inputGame(Game *game, GameUI gameUI) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), gameUI.left.dest)) {
            switchScreens(game, LEVEL);
        }
        if (CheckCollisionPointRec(GetMousePosition(), gameUI.right.dest)) {
            switchScreens(game, GAMEOVER);
        }
    }
}
void inputGameover(Game *game, GameUI gameUI) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), gameUI.left.dest)) {
            switchScreens(game, GAME);
        }
        if (CheckCollisionPointRec(GetMousePosition(), gameUI.right.dest)) {
            // Do nothing
        }
    }
}
void handleInput(Game *game, GameUI gameUI, Rectangle *bg, float *fadeAmount) {
    // printf("currentScreen: %d\n", currentScreen);

    if (game->currentTransition == TRANSITION_START || game->currentTransition == TRANSITION_END) {
        // printf("No Touching...!\n");
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


void updateHoverButtonState(UIButtons *button) {
    if (CheckCollisionPointRec(GetMousePosition(), button->dest)) {
        button->hover = true;
    } else {
        button->hover = false;
    }
}
void updateLoading(Game *game, GameUI *gameUI) {
    (game->framesCounter)++;
    (game->logoCounter)++;

    // Load logo screen after a 2.5 seconds
    if (game->logoCounter > 60 * LOADING_DELAY) {
        switchScreens(game, LOGO);
    }

    // Reset frames counter to get the loading balls effect
    if (game->framesCounter > 150) {
        game->framesCounter = 0;
    }
}
void updateLogo(Game *game, GameUI *gameUI) {
    (game->framesCounter)++;

    // double currentTime = GetTime();
    // if (currentTime - game->frameEntered >= LOGO_DELAY_SECS + 1) {
    //     switchScreens(game, MENU);
    // }
    // printf("%d\n", game->framesCounter);
    if (game->framesCounter >= 60 * LOGO_DELAY_SECS) {
        switchScreens(game, MENU);
    }

}
void updateMenu(Game *game, GameUI *gameUI) {
    (game->framesCounter)++;


    updateHoverButtonState(&gameUI->right);
}
void updateLevel(Game *game, GameUI *gameUI) {
    (game->framesCounter)++;

    updateHoverButtonState(&gameUI->left);
    updateHoverButtonState(&gameUI->right);
}
void updateGame(Game *game, GameUI *gameUI) {
    (game->framesCounter)++;

    updateHoverButtonState(&gameUI->left);
    updateHoverButtonState(&gameUI->right);
}
void updateGameover(Game *game, GameUI *gameUI) {
    (game->framesCounter)++;

    updateHoverButtonState(&gameUI->left);
}
void updateFade(Game *game, float *fadeAmount) {
    // printf("currentFadeType: %d\n", currentFadeType);
    if (game->currentTransition == IDLE) {
        // printf("idle\n");
    }
    if (game->currentTransition == TRANSITION_START) {

        // GO FULLY BLACK
        *fadeAmount += FADE_INCREMENT;
        // printf("fade out (black)...\n");

        // Wait 3 SECONDS AND CHANGE SCREENS
        if (*fadeAmount >= FADE_TIME) {
            *fadeAmount = FADE_TIME;

            // USER CAN NOT SEE NOW
            game->currentScreen = game->nextScreen;
            game->currentTransition = TRANSITION_END;
        }

    }
    if (game->currentTransition == TRANSITION_END) {

        // printf("fade in (visible)...\n");
        *fadeAmount -= FADE_INCREMENT;
        if (*fadeAmount <= 0.0f) {
            *fadeAmount = 0.0f;
            game->currentTransition = IDLE;

            // NEXT SCENE FULLY VISIABLE
            game->framesCounter = 0;
            game->frameEntered = GetTime();
        }

    }

}
void update(Game *game, GameUI *gameUI, Rectangle *bg, float *fadeAmount) {
    if (game->currentTransition == TRANSITION_START || game->currentTransition == TRANSITION_END) {
        updateFade(game, fadeAmount);
        return;
    }
    switch (game->currentScreen) {
        case LOADING: {
            updateLoading(game, gameUI);
        }
        break;
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
void drawLoading(Game game, GameUI gameUI) {
    ClearBackground(BLACK);

    int fontSize = 30;
    float radius = 5, pad = 30, textW = MeasureText("Loading", fontSize);
    float posX =  pad + radius, posY = GetScreenHeight() - pad - radius;
    float spaceBetween = radius * 2;
    float flowerW = gameUI.flower->width;
    Color color = WHITE;
    float flowers = true;

    DrawText("Loading", posX, posY - fontSize / 2, fontSize, color);
    if (game.framesCounter > 30) {
        if (flowers) {
            DrawTexture(*gameUI.flower, textW + spaceBetween + posX , posY - gameUI.flower->height / 2, WHITE);
        } else {
            DrawCircle(textW + posX, posY, radius, color);
        }
    }
    if (game.framesCounter > 60) {
        if (flowers) {
            DrawTexture(*gameUI.flower, textW + spaceBetween + posX + flowerW , posY - gameUI.flower->height / 2, WHITE);
        } else {
            DrawCircle(textW + posX + spaceBetween, posY, radius, color);
        }
    }
    if (game.framesCounter > 120) {
        if (flowers) {
            DrawTexture(*gameUI.flower, textW + spaceBetween + posX + flowerW * 2, posY - gameUI.flower->height / 2, WHITE);
        } else {
            DrawCircle(textW + posX + spaceBetween * 2, posY, radius, color);
        }
    }
}
void drawLogo(Game game) {
    ClearBackground(WHITE);
    int countdown = LOGO_DELAY_SECS - (game.framesCounter / 60);
    // int countdown = (LOGO_DELAY_SECS + 1) -  (GetTime() - game.frameEntered);
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
void draw(Game game, GameUI gameUI, Rectangle bg, float fadeAmount) {
    BeginDrawing();
    // ClearBackground(WHITE);

    // printf("%d\n", *framesCounter);

    switch (game.currentScreen) {
        case LOADING: {
            drawLoading(game, gameUI);
        }
        break;
        case LOGO: {
            drawLogo(game);
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
        // DrawRectangle(bg.x, bg.y, bg.width, bg.height, Fade(PINK, fadeAmount));
        DrawRectangle(bg.x, bg.y, bg.width, bg.height, Fade(BLACK, fadeAmount));
    }
    if (game.currentTransition == TRANSITION_END) {
        // DrawRectangle(bg.x, bg.y, bg.width, bg.height, Fade(ORANGE, fadeAmount));
        DrawRectangle(bg.x, bg.y, bg.width, bg.height, Fade(BLACK, fadeAmount));
        // DrawRectangle(GetScreenWidth() / 2, 0, GetScreenWidth(), GetScreenHeight(), ORANGE);
        // DrawRectanglePro((Rectangle) {destRect.x, destRect.y, destRect.width, destRect.height}, origin, 0, WHITE);
        // DrawTexturePro(transitionTexture, sourceRec, destRect, origin, rotation, ORANGE);
    }

    EndDrawing();
}

