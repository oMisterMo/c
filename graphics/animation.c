#include "raylib.h"
#include <stdio.h>

typedef struct Spritesheet {
    int totalFrames;
    Rectangle frameRec;     // Draw a part of a texture defined by a rectangle
    int frameIndex;         // Current frame from 0 -> TOTAL_FRAMES - 1
    int frameCounter;
    int frameSpeed;
} Spritesheet;

/**
 * Single row animation
 */
void updateBunny(Spritesheet *bunnySheet, Texture2D bunnyTexture) {
    // printf("%p\n", bunnySheet);
    bunnySheet->frameCounter++;

    if (bunnySheet->frameCounter >= (60 / bunnySheet->frameSpeed)) {
        bunnySheet->frameCounter = 0;
        bunnySheet->frameIndex++;

        // I don't want to display the last frame so I subtract 2 and not 1
        if (bunnySheet->frameIndex > bunnySheet->totalFrames - 2) bunnySheet->frameIndex = 0;

        bunnySheet->frameRec.x = (float) bunnySheet->frameIndex * (float) bunnyTexture.width / bunnySheet->totalFrames;
    }
}

/**
 * Single Column animation
 */
void updateButton(Spritesheet *buttonSheet, Texture2D buttonTexture) {
    // printf("%p\n", buttonSheet);
    buttonSheet->frameCounter++;

    if (buttonSheet->frameCounter >= (60 / buttonSheet->frameSpeed)) {
        buttonSheet->frameCounter = 0;
        buttonSheet->frameIndex++;

        if (buttonSheet->frameIndex > buttonSheet->totalFrames - 1) buttonSheet->frameIndex = 0;

        buttonSheet->frameRec.y = (float) buttonSheet->frameIndex * (float) buttonTexture.height / buttonSheet->totalFrames;
    }
}

int main() {

    InitWindow(1600, 768, "Animation");
    SetWindowMonitor(2);

    Texture2D bunnyTexture = LoadTexture("resources/sprites/jumpbunny_anim1.png");
    Texture2D slime  = LoadTexture("resources/sprites/slime_green.png");
    Texture2D buttonTexture  = LoadTexture("resources/sprites/button.png");
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
    int TOTAL_FRAMES_BUTTON = 3;
    Spritesheet buttonSheet = {
        TOTAL_FRAMES_BUTTON,
        (Rectangle){ 0, 0, buttonTexture.width, buttonTexture.height / TOTAL_FRAMES_BUTTON },
        0, 0, 1
    };


    SetTargetFPS(60);
    // Game Loop
    while (!WindowShouldClose()) {
        // Update
        updateBunny(&bunnySheet, bunnyTexture);
        updateButton(&buttonSheet, buttonTexture);


        // Draw
        BeginDrawing();
        ClearBackground(ORANGE);

        DrawTextureRec(bunnyTexture, bunnySheet.frameRec, (Vector2) { 0 } , WHITE);
        DrawTextureRec(buttonTexture, buttonSheet.frameRec, (Vector2) { bunnyTexture.width / TOTAL_FRAMES_BUNNY, 0 } , WHITE);
        DrawTexture(slime, 0, bunnyTexture.height , WHITE);

        EndDrawing();
    }

    UnloadTexture(bunnyTexture);
    UnloadTexture(slime);
    UnloadTexture(buttonTexture);
    CloseWindow();

    return 0;
}
