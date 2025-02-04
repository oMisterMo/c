#include <stdio.h>
#include "raylib.h"

int main() {

    printf("RAYLIB_VERSION_MAJOR: %d\n", RAYLIB_VERSION_MAJOR);

    // Setup
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "mo");
    SetTargetFPS(60);

    // Initialize varaibles
    Font font = GetFontDefault();
    char *str = "mo";
    int fontSize = 120;
    // int textW = MeasureText("Mooooo", 30);
    int spacing = 20;

    SetTextLineSpacing(spacing);

    Vector2 text = MeasureTextEx(font, str, fontSize, spacing);
    Vector2 vel = { .x = 1, .y = 1 };
    Vector2 pos = { 0, 450 - 50 };

    int frameCounter = 0;          // Useful to count frames


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        ClearBackground(RAYWHITE);

        // Input
        if (IsKeyPressed(KEY_LEFT)) {
            vel.x = -1;
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            vel.x = 1;
        }
        if (IsKeyPressed(KEY_UP)) {
            vel.y = -1;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            vel.y = 1;
        }
        if (IsKeyReleased(KEY_LEFT)) {
            vel.x = 0;
        }
        if (IsKeyReleased(KEY_RIGHT)) {
            vel.x = 0;
        }
        if (IsKeyReleased(KEY_UP)) {
            vel.y = 0;
        }
        if (IsKeyReleased(KEY_DOWN)) {
            vel.y = 0;
        }


        // Update
        pos.x += vel.x * GetFrameTime() * 100;
        pos.y += vel.y * GetFrameTime() * 100;

        // Collision Detection
        if (pos.x > GetScreenWidth() - text.x) {
            pos.x = GetScreenWidth() - text.x;
            vel.x *= -1;
        }
        if (pos.y > GetScreenHeight() - 48 * 2) {
            pos.y = GetScreenHeight() - 48 * 2;
            vel.y *= -1;
        }
        if (pos.x <= 0) {
            pos.x = 0;
            vel.x *= -1;
        }
        if (pos.y <= -18 * 2) {
            pos.y =  -18 * 2;
            vel.y *= -1;
        }

        // Draw
        BeginDrawing();

        // DrawText("hello", GetScreenWidth() - 100, 0, 20, GRAY);
        // DrawText("Mooooo", pos.x, pos.y, 30, ORANGE);
        DrawTextEx(font, str, pos, fontSize, spacing, ORANGE);
        DrawFPS(GetScreenWidth() - 80 - 20, 20);

        EndDrawing();
    }

    // Unload - Close window and OpenGL context
    CloseWindow();        

    return 0;
}
