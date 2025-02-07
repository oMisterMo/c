#include <stdio.h>
#include "raylib.h"

#define SIZE 32     // default single sprite size


Rectangle GetRandomSource() {
    return (Rectangle) { SIZE * GetRandomValue(0, 1), SIZE * GetRandomValue(0, 4), SIZE, SIZE };
}

int main() {

    InitWindow(1200, 800, "Hello Window");


    // Init variables
    Texture2D redTexture = LoadTexture("resources/sprites/red.png");
    Texture2D greenTexture = LoadTexture("resources/sprites/green.png");
    Texture2D blueTexture = LoadTexture("resources/sprites/blue.png");
    Texture2D nPatchTexture = LoadTexture("resources/ui/ninepatch_button.png");

    // What to draw?
    NPatchInfo srcInfo = { (Rectangle) { 0.0f, 64.0f, 64.0f, 64.0f }, 16, 16, 16, 16, NPATCH_NINE_PATCH };
    // Where to draw?
    Rectangle destRect = { 160.0f, 160.0f, SIZE, SIZE };
    // Set the size
    float scale = 5.0f;
    destRect.width = SIZE * scale;
    destRect.height = SIZE * scale;

    // Get first image from the red spritesheet
    Rectangle srcRect = { 0, 0, SIZE, SIZE };
    // Randomise the x,y offset
    srcRect = GetRandomSource();


    SetTargetFPS(60);
    while(!WindowShouldClose()) {

        // Handle input

        // Update

        // Draw
        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawFPS(20, 20);

        // Testing, draw textures
        // DrawTexture(nPatchTexture, 100, 300, WHITE);
        // DrawTexture(redTexture, 0, 0, WHITE);

        // Draw the resizeable UI background
        DrawTextureNPatch(nPatchTexture, srcInfo, destRect, (Vector2) { 0 }, 0, WHITE);
        
        // Draw the image on top of the background
        DrawTexturePro(blueTexture, srcRect, destRect, (Vector2) { 0 }, 0, WHITE);

        EndDrawing();
    }


    UnloadTexture(nPatchTexture);
    UnloadTexture(redTexture);
    UnloadTexture(greenTexture);
    UnloadTexture(blueTexture);
    CloseWindow();


    return 0;
}
