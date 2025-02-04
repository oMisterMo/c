#include <stdio.h>
#include "raylib.h"

//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

void update(GameScreen *currentScreen, int *frameCounter);
void draw(GameScreen *currentScreen, int *frameCounter);

int main() {


    printf("Hello world\n");
    printf("RAYLIB_VERSION_MAJOR: %d\n", RAYLIB_VERSION_MAJOR);

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;


    InitWindow(screenWidth, screenHeight, "Mo Man");
    SetTargetFPS(60);

    // TODO: Initialize all required variables and load all required data here!
    GameScreen currentScreen = LOGO;



    int frameCounter = 0;          // Useful to count frames

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        ClearBackground(RAYWHITE);

        // Update
        update(&currentScreen, &frameCounter);

        // Draw
        BeginDrawing();

        draw(&currentScreen, &frameCounter);
        DrawFPS(GetScreenWidth() - 80 - 20, 20);
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context

    return 0;
}

void update(GameScreen *currentScreen, int *frameCounter) {
    switch (*currentScreen) {
        case LOGO:
        {
            // TODO: Update LOGO screen variables here!

            (*frameCounter)++;    // Count frames

            // Wait for 3 seconds (180 frames) before jumping to TITLE screen
            if (*frameCounter > 180) {
                *currentScreen = TITLE;
            }
        } break;
        case TITLE:
        {
            // TODO: Update TITLE screen variables here!

            // Press enter to change to GAMEPLAY screen
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                *currentScreen = GAMEPLAY;
            }
        } break;
        case GAMEPLAY:
        {
            // TODO: Update GAMEPLAY screen variables here!

            // Press enter to change to ENDING screen
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                *currentScreen = ENDING;
            }
        } break;
        case ENDING:
        {
            // TODO: Update ENDING screen variables here!

            // Press enter to return to TITLE screen
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                *currentScreen = TITLE;
            }
        } break;
        default: break;
    }
}

void draw(GameScreen *currentScreen, int *frameCounter) {
    switch(*currentScreen)
    {
        case LOGO:
        {
            // TODO: Draw LOGO screen here!
            DrawText("LOGO SCREEN", GetScreenWidth() / 2 - 290 / 2, 20, 40, LIGHTGRAY);
            DrawText("WAIT for 3 SECONDS...", 290, 220, 20, GRAY);
            DrawText(TextFormat("Frames: %d", *frameCounter), 20, 100, 20, DARKGREEN);

        } break;
        case TITLE:
        {
            // TODO: Draw TITLE screen here!
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);
            DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
            DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);

        } break;
        case GAMEPLAY:
        {
            // TODO: Draw GAMEPLAY screen here!
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
            DrawText("GAMEPLAY SCREEN", 20, 20, 40, MAROON);
            DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);

        } break;
        case ENDING:
        {
            // TODO: Draw ENDING screen here!
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLUE);
            DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
            DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);

        } break;
        default: break;
    }
}
