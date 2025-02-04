#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>

void ClampScreen(Vector2 *position, int width, int height) {
    if (position->x <= 0) position->x = 0;
    if (position->y <= 0) position->y = 0;
    if (position->x + width >= GetScreenWidth()) position->x = GetScreenWidth() - width;
    if (position->y + height >= GetScreenHeight()) position->y = GetScreenHeight() - height;
}

void ClampScreenRec(Rectangle *rect) {
    if (rect->x <= 0) rect->x = 0;
    if (rect->y <= 0) rect->y = 0;
    if (rect->x + rect->width >= GetScreenWidth()) rect->x = GetScreenWidth() - rect->width;
    if (rect->y + rect->height >= GetScreenHeight()) rect->y = GetScreenHeight() - rect->height;
}

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

    Rectangle logoBounds = { (GetScreenWidth() - logo.width ) / 2, (GetScreenHeight() - logo.height ) / 2, logo.width, logo.height };
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
            if (CheckCollisionPointRec(touchPosition, logoBounds)) {
                isDragging = true;
            }
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (isDragging) {
                logoBounds.x = touchPosition.x - logoBounds.width / 2;
                logoBounds.y = touchPosition.y - logoBounds.height / 2;
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            isDragging = false;

            ClampScreenRec(&logoBounds);
        }

        // Update

        // Collision detection


        // Draw
        BeginDrawing();
        ClearBackground(WHITE);

        DrawTexture(logo, logoBounds.x, logoBounds.y, WHITE);
        DrawFPS(20, 20);

        EndDrawing();

    }

    UnloadTexture(logo);
    CloseWindow();

    return 0;
}
