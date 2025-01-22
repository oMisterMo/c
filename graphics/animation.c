#include "raylib.h"

int main() {

    InitWindow(1600, 768, "Animation");
    SetWindowMonitor(2);

    Texture2D jumpbunny = LoadTexture("resources/sprites/jumpbunny_anim1.png");
    Texture2D slime  = LoadTexture("resources/sprites/slime_green.png");
    slime.width *= 8;
    slime.height *= 8;
    int TOTAL_FRAMES_BUNNY = 9;

    //============================
    Rectangle frameRec = { 0, 0, jumpbunny.width / TOTAL_FRAMES_BUNNY, jumpbunny.height };
    int currentFrame = 0;       // Frame index 0 -> TOTAL_FRAMES - 1
    int frameCounter = 0;
    int frameSpeed = 5;

    SetTargetFPS(60);
    // Game Loop
    while (!WindowShouldClose()) {
        // Update
        frameCounter++;

        if (frameCounter >= (60 / frameSpeed)) {
            frameCounter = 0;
            currentFrame++;

            // I don't want to display the last frame
            if (currentFrame > TOTAL_FRAMES_BUNNY - 2) currentFrame = 0;

            frameRec.x = (float) currentFrame * (float) jumpbunny.width / TOTAL_FRAMES_BUNNY;
        }

        // Draw
        BeginDrawing();
        ClearBackground(ORANGE);




        DrawTextureRec(jumpbunny, frameRec, (Vector2) { 0 } , WHITE);
        DrawTexture(slime, 0, jumpbunny.height , WHITE);





        EndDrawing();
    }



    UnloadTexture(jumpbunny);
    UnloadTexture(slime);
    CloseWindow();


    return 0;
}
