#include "raylib.h"
#include <stdio.h>

typedef struct Spritesheet {
    int totalFrames;
    Rectangle frameRec;     // Rec source to draw
    int frameIndex;         // Current frame from 0 -> TOTAL_FRAMES - 1
    int frameCounter;
    int frameSpeed;
} Spritesheet;

void updateBunny(Spritesheet *bunnySheet, Texture2D bunnyTexture) {
    // printf("%p\n", bunnySheet);
    bunnySheet->frameCounter++;

    if (bunnySheet->frameCounter >= (60 / bunnySheet->frameSpeed)) {
        bunnySheet->frameCounter = 0;
        bunnySheet->frameIndex++;

        // I don't want to display the last frame
        if (bunnySheet->frameIndex > bunnySheet->totalFrames - 2) bunnySheet->frameIndex = 0;

        bunnySheet->frameRec.x = (float) bunnySheet->frameIndex * (float) bunnyTexture.width / bunnySheet->totalFrames;
    }
}

int main() {

    InitWindow(1600, 768, "Animation");
    SetWindowMonitor(2);

    Texture2D bunnyTexture = LoadTexture("resources/sprites/jumpbunny_anim1.png");
    Texture2D slime  = LoadTexture("resources/sprites/slime_green.png");
    slime.width *= 8;
    slime.height *= 8;

    //============================
    int TOTAL_FRAMES_BUNNY = 9;
    Spritesheet bunnySheet = {
        TOTAL_FRAMES_BUNNY,
        (Rectangle){ 0, 0, bunnyTexture.width / TOTAL_FRAMES_BUNNY, bunnyTexture.height },
        0, 0, 5
    };
    // printf("**%p**\n", &bunnySheet);
    //============================



    SetTargetFPS(60);
    // Game Loop
    while (!WindowShouldClose()) {
        // Update
        updateBunny(&bunnySheet, bunnyTexture);





        // Draw
        BeginDrawing();
        ClearBackground(ORANGE);




        DrawTextureRec(bunnyTexture, bunnySheet.frameRec, (Vector2) { 0 } , WHITE);
        DrawTexture(slime, 0, bunnyTexture.height , WHITE);





        EndDrawing();
    }



    UnloadTexture(bunnyTexture);
    UnloadTexture(slime);
    CloseWindow();


    return 0;
}
