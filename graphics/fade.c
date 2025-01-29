#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

// Define some constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define FADE_TIME 2.0f
#define FADE_INCREMENT 0.05f        // Base increment for linear fade

typedef enum {
    IDLE = 0,
    FADE_START,
    FADE_END
} Transition;

typedef enum { SCREEN_MAIN, SCREEN_GAME } Screen;



void handleUpdate(Transition *currentTransition) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (*currentTransition == FADE_START || *currentTransition == FADE_END) {
            printf("No Clicking...\n");
            return;
        }
        *currentTransition = FADE_START;
    }
}

void updateFade(Screen *currentScreen, Transition *currentTransition, float *fadeAmount) {
    // printf("currentFadeType: %d\n", currentFadeType);

    if (*currentTransition == IDLE) {
        printf("idle\n");
    }
    if (*currentTransition == FADE_START) {

        // GO FULLY BLACK
        *fadeAmount += FADE_INCREMENT;
        printf("fade to black...\n");

        // Wait 3 SECONDS AND CHANGE SCREENS
        if (*fadeAmount >= FADE_TIME) {
            *fadeAmount = FADE_TIME;

            // USER CAN NOT SEE NOW
            *currentScreen = (*currentScreen == SCREEN_MAIN) ? SCREEN_GAME : SCREEN_MAIN;
            *currentTransition = FADE_END;
        }
    }
    if (*currentTransition == FADE_END) {
        printf("fade out...\n");
        *fadeAmount -= FADE_INCREMENT;
        if (*fadeAmount <= 0.0f) {
            *fadeAmount = 0.0f;
            *currentTransition = IDLE;
        }
    }

}

void drawFadeTransition(Transition currentTransition, float *fadeAmount) {
    if (currentTransition == FADE_START || currentTransition == FADE_END) {

        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, *fadeAmount));
    }
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Fade Transition Example");


    // variables
    Screen currentScreen = SCREEN_MAIN;
    Transition currentTransition = IDLE;

    float fadeAmount = 0.0f;


    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        //Input
        handleUpdate(&currentTransition);

        // Update
        updateFade(&currentScreen, &currentTransition, &fadeAmount);

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // ===========
        // Game
        // ===========
        switch (currentScreen) {
            case SCREEN_MAIN:
            DrawText("Main Screen", 350, 280, 20, DARKGRAY);
            break;
            case SCREEN_GAME:
            DrawText("Game Screen", 350, 280, 20, BLACK);
            ClearBackground(RED);
            break;
        }

        // ===========
        // Transition
        // ===========
        drawFadeTransition(currentTransition, &fadeAmount);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
