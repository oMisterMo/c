#include "raylib.h"
#include "reasings.h"
#include "transition.h"
#include <stdio.h>


#define MIN_SCALE 0
#define MAX_SCALE 2


int main() {

    int screenWidth = 960;
    int screenHeight = 600;

    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(screenWidth, screenHeight, "Transition");

    // Load Textures
    Texture2D transitionTexture = LoadTexture("resources/ui/transition.png");
    Texture2D buttonsTexture = LoadTexture("resources/ui/buttons_navigation.png");
    Texture2D flowerTexture = LoadTexture("resources/ui/icon_flower.png");
    flowerTexture.width *= 0.3;
    flowerTexture.height *= 0.3;

    // Set texture attribute
    float scale = 0.01;
    transitionTexture.width *= scale;
    transitionTexture.height *= scale;
    // Rectangle sourceRec = { 0, 0, transitionTexture.width, transitionTexture.height };  // multiply width by -1 to flip
    // Rectangle destRect = { screenWidth / 2, screenHeight / 2, transitionTexture.width, transitionTexture.height };
    // Vector2 origin = { (float) transitionTexture.width / 2, (float) transitionTexture.height / 2 };

    // Set buttons attrubutes
    buttonsTexture.width *= 0.3;
    buttonsTexture.height *= 0.3;
    int BUTTON_W = buttonsTexture.width / 6;
    int BUTTON_H = buttonsTexture.height / 2;
    Rectangle buttonsSrc[12];
    // Regular buttonsSrc
    for (int x = 0; x < 6; x++) {
        buttonsSrc[x] = (Rectangle) {
            (float) x * buttonsTexture.width / 6,
            0,
            (float) buttonsTexture.width / 6,
            (float) buttonsTexture.height / 2
        };
    }
    // Hover button (i + 6)
    for (int x = 0, i = 6; x < 6; i++, x++) {
        buttonsSrc[i] = (Rectangle) {
            (float) x * buttonsTexture.width / 6,
            buttonsTexture.height / 2,
            (float) buttonsTexture.width / 6,
            (float) buttonsTexture.height / 2
        };
    }

    // Tween
    Rectangle tweenBG = { 0, 0, GetScreenWidth(), GetScreenHeight() };
    float fadeAmount = 0.0f;

    GameUI gameUI = { 0 };
    Rectangle buttonDestLeft = { 0, (GetScreenHeight() - BUTTON_H ) / 2, BUTTON_W, BUTTON_H };
    Rectangle buttonDestRight = { GetScreenWidth() - BUTTON_W, (GetScreenHeight() - BUTTON_H ) / 2, BUTTON_W, BUTTON_H };
    gameUI.left = (UIButtons) { &buttonsTexture, buttonsSrc[UI_LEFT], buttonsSrc[UI_LEFT + 6], buttonDestLeft, false };
    gameUI.right = (UIButtons) { &buttonsTexture, buttonsSrc[UI_RIGHT], buttonsSrc[UI_RIGHT + 6], buttonDestRight, false };
    gameUI.flower = &flowerTexture;
    printf("--------------\n");
    printf("buttonsTexture: %p\n", &buttonsTexture);

    Game game = { 0 };
    game.currentScreen = LOADING;
    game.nextScreen = LOGO;
    game.currentTransition = TRANISITON_IDLE;
    game.frameEntered = 0;
    game.frameCounter = 0;
    game.logoCounter = 0;

    printf("--------------\n");
    printf("ButtonLeft.texture: %p\n", (gameUI.left.texture));
    printf("--------------\n");



    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        // Handle input
        handleInput(&game, gameUI);

        // Update
        update(&game, &gameUI, &fadeAmount);

        // Draw
        draw(game, gameUI, tweenBG, fadeAmount);
    }


    UnloadTexture(transitionTexture);
    UnloadTexture(buttonsTexture);
    UnloadTexture(flowerTexture);
    CloseWindow();
    return 0;
}

