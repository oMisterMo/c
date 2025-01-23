#include "raylib.h"
#include <stdio.h>

int main() {

    int screenWidth = 960;
    int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Transition");
    SetWindowMonitor(2);

    Texture2D transitionTexture = LoadTexture("resources/ui/transition.png");
    int rotation = 0;
    float scale = .2;
    transitionTexture.width *= scale;
    transitionTexture.height *= scale;
    Rectangle sourceRec = { 0, 0, transitionTexture.width, transitionTexture.height };  // multiply width by -1 to flip
    Rectangle destRect = { screenWidth / 2, screenHeight / 2, transitionTexture.width, transitionTexture.height };
    Vector2 origin = { (float) transitionTexture.width / 2, (float) transitionTexture.height / 2 };


    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        // Handle input

        // Update
        // rotation += 1;
        if (destRect.width >= 0) {
            destRect.width  -= 1;
            destRect.height -= 1;
            origin.x = (float) destRect.width / 2;
            origin.y = (float) destRect.height / 2;
            printf("%.1f,%.1f\n", destRect.width, destRect.height);
        }



        // Draw
        BeginDrawing();
        ClearBackground(ORANGE);

        DrawRectanglePro((Rectangle) {destRect.x, destRect.y, destRect.width, destRect.height}, origin, 0, WHITE);
        DrawTexturePro(transitionTexture, sourceRec, destRect, origin, rotation, ORANGE);


        EndDrawing();
    }


    UnloadTexture(transitionTexture);
    CloseWindow();
    return 0;
}
