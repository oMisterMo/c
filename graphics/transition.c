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
    printf("Original address: %p\n", &buttonsTexture);

    // Tween
    Tween tween = { (Vector2) {}, (Vector2) {}, TWEENING, 0, 60 * 3 };
    int framesCounter = 0;
    float duration = 60 * 1.5;
    int state = TWEENING;
    Rectangle bg = { 0, 0, GetScreenWidth(), GetScreenHeight() };
    float screenEnterTime = 0;

    Vector2 textPosition = { 40, 40 };
    Vector2 textOrigin = { 0 };
    TransitionState currentTransition = IDLE;
    GameScreen currentScreen = MENU;

    printf("--------------\n");
    printf("ButtonLeft address: %p\n", (buttonLeft.texture));
    printf("--------------\n");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        // Handle input
        handleInput(&currentScreen, &currentTransition, &framesCounter, buttonLeft, buttonRight, &bg);

        // Update
        update(&currentScreen, &currentTransition, &framesCounter, &buttonLeft, &buttonRight, &bg);

        // Draw
        draw(currentScreen, currentTransition, framesCounter, textPosition, textOrigin, buttonLeft, buttonRight, bg);
    }


    UnloadTexture(transitionTexture);
    UnloadTexture(buttonsTexture);
    CloseWindow();
    return 0;
}

