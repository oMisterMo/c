#include "raylib.h"
#include "reasings.h"
#include "transition.h"
#include <stdio.h>



#define MIN_SCALE 0
#define MAX_SCALE 2


int main() {

    int screenWidth = 960;
    int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Transition");
    SetWindowMonitor(2);

    // Load Textures
    Texture2D transitionTexture = LoadTexture("resources/ui/transition.png");
    Texture2D buttonsTexture = LoadTexture("resources/ui/buttons_navigation.png");

    // Set texture attribute
    float scale = 0.01;
    transitionTexture.width *= scale;
    transitionTexture.height *= scale;
    Rectangle sourceRec = { 0, 0, transitionTexture.width, transitionTexture.height };  // multiply width by -1 to flip
    Rectangle destRect = { screenWidth / 2, screenHeight / 2, transitionTexture.width, transitionTexture.height };
    Vector2 origin = { (float) transitionTexture.width / 2, (float) transitionTexture.height / 2 };

    // Set buttons attrubutes
    buttonsTexture.width *= 0.3;
    buttonsTexture.height *= 0.3;
    int BUTTON_W = buttonsTexture.width / 6;
    int BUTTON_H = buttonsTexture.height / 2;
    Rectangle buttonsSrc[12];
    // Regular buttonsSrc
    for (int i = 0; i < 6; i++) {
        buttonsSrc[i] = (Rectangle) {
            (float) i * buttonsTexture.width / 6,
            0,
            (float) buttonsTexture.width / 6,
            (float) buttonsTexture.height / 2
        };
    }
    // Hover button (i + 6)
    for (int i = 6; i < 12; i++) {
        buttonsSrc[i] = (Rectangle) {
            (float) i * buttonsTexture.width / 6,
            buttonsTexture.height / 2,
            (float) buttonsTexture.width / 6,
            (float) buttonsTexture.height / 2
        };
    }
    Rectangle buttonDestLeft = { 0, (GetScreenHeight() - BUTTON_H ) / 2, BUTTON_W, BUTTON_H };
    Rectangle buttonDestRight = { GetScreenWidth() - BUTTON_W, (GetScreenHeight() - BUTTON_H ) / 2, BUTTON_W, BUTTON_H };
    UIButtons buttonLeft = { &buttonsTexture, buttonsSrc[UI_LEFT], buttonsSrc[UI_LEFT + 6], buttonDestLeft, false };
    UIButtons buttonRight = { &buttonsTexture, buttonsSrc[UI_RIGHT], buttonsSrc[UI_RIGHT + 6], buttonDestRight, false };
    printf("--------------\n");
    printf("buttonsTexture: %p\n", &buttonsTexture);

    // Tween
    Rectangle tweenBG = { 0, 0, GetScreenWidth(), GetScreenHeight() };

    Game game = { MENU, IDLE, 0 };

    printf("--------------\n");
    printf("ButtonLeft.texture: %p\n", (buttonLeft.texture));
    printf("--------------\n");



    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        // Handle input
        handleInput(&game, buttonLeft, buttonRight, &tweenBG);

        // Update
        update(&game, &buttonLeft, &buttonRight, &tweenBG);

        // Draw
        draw(game, buttonLeft, buttonRight, tweenBG);
    }


    UnloadTexture(transitionTexture);
    UnloadTexture(buttonsTexture);
    CloseWindow();
    return 0;
}

