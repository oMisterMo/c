#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>

int main() {

    int screenWidth = 800 * 2;
    int screenHeight = 400 * 2;

    // Before window creation
    // FLAG_VSYNC_HINT | FLAG_FULLSCREEN_MODE | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_UNDECORATED
    SetConfigFlags( FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_UNDECORATED );
    InitWindow(screenWidth, screenHeight, "Logo");
    //After window creation

    // Load logo texture
    Texture2D logo = LoadTexture("resources/logo/raylib_256x256.png");

    Rectangle logoPosition = { (GetScreenWidth() - logo.width ) / 2, (GetScreenHeight() - logo.height ) / 2, logo.width, logo.height };
    Vector2 touchPosition = { 0, 0 };
    bool isDragging = false;

    int frameCounter = 0;

    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        frameCounter++;

        // Handle input
        if (IsKeyPressed(KEY_F)) {
            ToggleFullscreen();
            // screenWidth = GetScreenWidth();
            // screenHeight = GetScreenHeight();
            // SetWindowSize(screenWidth, screenHeight);
            // printf("screenWidth: %d\n", GetScreenWidth());
            // printf("screenHeight: %d\n", GetScreenHeight());
            // printf("monitorWidth: %d\n", GetMonitorWidth(1));
            // printf("monitorHeight: %d\n", GetMonitorHeight(1));
            // screenWidth = GetMonitorWidth(1);
            // screenHeight = GetMonitorHeight(1);
            // SetupViewport(screenWidth, screenHeight);
            // SetupFramebuffer(screenWidth, screenHeight);
        }
        if (IsKeyPressed(KEY_S)) TakeScreenshot("img.png");

        touchPosition = GetTouchPosition(0);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(touchPosition, logoPosition)) {
                isDragging = true;
            }
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (isDragging) {
                logoPosition.x = touchPosition.x - logoPosition.width / 2;
                logoPosition.y = touchPosition.y - logoPosition.height / 2;
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            isDragging = false;

            if (logoPosition.x  <= 0) {
                logoPosition.x = 0;
            }
            if (logoPosition.x + logoPosition.width > GetScreenWidth()) {
                logoPosition.x =  GetScreenWidth() - logoPosition.width;
            }
            if (logoPosition.y  <= 0) {
                logoPosition.y = 0;
            }
            if (logoPosition.y + logoPosition.height > GetScreenHeight()) {
                logoPosition.y =  GetScreenHeight() - logoPosition.height;
            }

        }

        // Update

        // Collision detection


        // Draw
        BeginDrawing();
        ClearBackground(WHITE);

        DrawTexture(logo, logoPosition.x, logoPosition.y, WHITE);
        DrawFPS(20, 20);

        EndDrawing();

    }

    UnloadTexture(logo);
    CloseWindow();

    return 0;
}
