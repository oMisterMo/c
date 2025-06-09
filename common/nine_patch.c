#include <stdio.h>
#include "raylib.h"

#define SIZE 32     // default single sprite size


Rectangle GetRandomSource() {
    return (Rectangle) { SIZE * GetRandomValue(0, 1), SIZE * GetRandomValue(0, 4), SIZE, SIZE };
}

bool CheckCollisionPointRecPro(Vector2 point, Rectangle rec, Vector2 origin) {
    bool collision = false;

    if ((point.x >= rec.x - origin.x) && (point.x < (rec.x - origin.x + rec.width)) && (point.y >= rec.y - origin.y) && (point.y < (rec.y - origin.y + rec.height))) collision = true;

    return collision;
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
    // Get first image from the red spritesheet
    Rectangle srcRect = { 0, 0, SIZE, SIZE };
    // Randomise the x,y offset
    srcRect = GetRandomSource();
    // Set the size
    float scale = 5.0f;
    float w = SIZE * scale;
    float h = SIZE * scale;
    // Where to draw?
    Rectangle destRect = { (GetScreenWidth()) / 2 , (GetScreenHeight()) / 2, w, h };
    // destRect.x -= w/2;
    // destRect.y -= h/2;
    // destRect.x = 0;
    // destRect.y = 0;
    // Additional properties
    Vector2 origin = (Vector2) { w/2, h/2 };
    int rotation = 90;


    SetTargetFPS(60);
    while(!WindowShouldClose()) {

        rotation += 5 * 0.2;

        // Handle input
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // if (CheckCollisionPointRec(GetMousePosition(), destRect)) {
            //     srcRect = GetRandomSource();
            // }
            if (CheckCollisionPointRecPro(GetMousePosition(), destRect, origin)) {
                srcRect = GetRandomSource();
            }
        }

        // Update

        // Draw
        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawFPS(20, 20);

        // Testing, draw textures
        // DrawTexture(nPatchTexture, 100, 300, WHITE);
        // DrawTexture(redTexture, 0, 0, WHITE);


        // Draw the actual position
        DrawRectangleRec(destRect, RED);
        DrawRectanglePro(destRect, origin, 0, BLUE);

        // Draw the resizeable UI background
        // DrawTextureNPatch(nPatchTexture, srcInfo, (Rectangle) { destRect.x + origin.x, destRect.y + origin.y, destRect.width, destRect.height }, origin, rotation, WHITE);
        DrawTextureNPatch(nPatchTexture, srcInfo, destRect, origin, rotation, WHITE);
        
        // Draw the image on top of the background
        // DrawTexturePro(blueTexture, srcRect, (Rectangle) { destRect.x + origin.x, destRect.y + origin.y, destRect.width, destRect.height }, origin, rotation, WHITE);
        DrawTexturePro(blueTexture, srcRect, destRect, origin, rotation, WHITE);

        EndDrawing();
    }


    UnloadTexture(nPatchTexture);
    UnloadTexture(redTexture);
    UnloadTexture(greenTexture);
    UnloadTexture(blueTexture);
    CloseWindow();


    return 0;
}
