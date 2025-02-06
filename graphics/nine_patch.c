#include <stdio.h>
#include "raylib.h"

int main() {

    InitWindow(1200, 800, "Hello Window");


    // Init variables
    Texture2D nPatchTexture = LoadTexture("resources/ui/ninepatch_button.png");
    // What to draw?
    NPatchInfo srcInfo = { (Rectangle){ 0.0f, 64.0f, 64.0f, 64.0f }, 16, 16, 16, 16, NPATCH_NINE_PATCH };
    // Where to draw?
    Rectangle destRect = { 160.0f, 160.0f, 32.0f, 32.0f };
    // Set the size
    destRect.width = 100;
    destRect.height = 100;

    SetTargetFPS(60);
    while(!WindowShouldClose()) {

        // Handle input

        // Update

        // Draw
        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawFPS(20, 20);

        // DrawTexture(nPatchTexture, 100, 300, WHITE);
        
        DrawTextureNPatch(nPatchTexture, srcInfo, destRect, (Vector2) { 0 }, 0, WHITE);

        EndDrawing();
    }


    UnloadTexture(nPatchTexture);
    CloseWindow();


    return 0;
}
