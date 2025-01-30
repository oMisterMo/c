#include "raylib.h"

int main() {


    InitWindow(800, 400, "First Window");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Input

        // Update

        // Collisions

        // Draw
        BeginDrawing();

        ClearBackground(BLUE);
        DrawFPS(20, 20);

        EndDrawing();
    }


    CloseWindow();


    return 0;
}
