#include "raylib.h"
#include <stdio.h>

typedef struct Spritesheet {
    int totalFrames;
    Rectangle frameRec;     // Draw a part of a texture defined by a rectangle
    int currentFrame;       // The current frame, ranges from 0 to TOTAL_FRAMES - 1
    int frameCounter;
    int frameSpeed;
} Spritesheet;
typedef struct SpritesheetGrid {
    Rectangle frameRec;         // Draw a part of a texture defined by a rectangle
    int currentFrame;           // The current frame, ranges from 0 to TOTAL_FRAMES - 1
    int currentLine;            // The current frame, ranges from 0 to TOTAL_FRAMES - 1
    int frameCounter;
    int frameSpeed;
} SpritesheetGrid;

/**
 * Single row animation
 */
void updateBunny(Spritesheet *bunnySheet, Texture2D bunnyTexture) {
    // printf("%p\n", bunnySheet);
    bunnySheet->frameCounter++;

    if (bunnySheet->frameCounter >= (60 / bunnySheet->frameSpeed)) {
        bunnySheet->frameCounter = 0;
        bunnySheet->currentFrame++;

        // I don't want to display the last frame so I subtract 2 and not 1
        if (bunnySheet->currentFrame > bunnySheet->totalFrames - 2) bunnySheet->currentFrame = 0;

        bunnySheet->frameRec.x = (float) bunnySheet->currentFrame * (float) bunnyTexture.width / bunnySheet->totalFrames;
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
        buttonSheet->currentFrame++;

        if (buttonSheet->currentFrame > buttonSheet->totalFrames - 1) buttonSheet->currentFrame = 0;

        buttonSheet->frameRec.y = (float) buttonSheet->currentFrame * (float) buttonTexture.height / buttonSheet->totalFrames;
    }
}

/**
 * Grid animation
 */
void updateSlime(SpritesheetGrid *slimeSheet, Texture2D slimeTexture, int xFrames, int yFrames) {
    // printf("%p\n", slimeSheet);
    slimeSheet->frameCounter++;

    float frameWidth = (float) (slimeTexture.width / xFrames);
    float frameHeight = (float) (slimeTexture.height / yFrames);

    if (slimeSheet->frameCounter >= (60 / slimeSheet->frameSpeed)) {

        slimeSheet->currentFrame++;

        if (slimeSheet->currentFrame >= xFrames) {
            slimeSheet->currentFrame = 0;
            slimeSheet->currentLine++;
            if (slimeSheet->currentLine >= yFrames) {
                // DONE ANMATING ALL FRAMES
                slimeSheet->currentLine = 0;
            }
        }
        slimeSheet->frameCounter = 0;


        slimeSheet->frameRec.x = frameWidth * slimeSheet->currentFrame;
        slimeSheet->frameRec.y = frameHeight * slimeSheet->currentLine;

    }
}

int main() {

    InitWindow(1600, 768, "Animation");
    SetWindowMonitor(2);

    Texture2D bunnyTexture = LoadTexture("resources/sprites/jumpbunny_anim1.png");
    Texture2D buttonTexture  = LoadTexture("resources/sprites/button.png");
    Texture2D slimeTexture  = LoadTexture("resources/sprites/slime_green.png");

    slimeTexture.width *= 8;
    slimeTexture.height *= 8;

    //============================
    int TOTAL_FRAMES_BUNNY = 9;
    Spritesheet bunnySheet = {
        TOTAL_FRAMES_BUNNY,
        (Rectangle){ 0, 0, bunnyTexture.width / TOTAL_FRAMES_BUNNY, bunnyTexture.height },
        0, 0, 5
    };
    //============================
    int TOTAL_FRAMES_BUTTON = 3;
    Spritesheet buttonSheet = {
        TOTAL_FRAMES_BUTTON,
        (Rectangle){ 0, 0, buttonTexture.width, buttonTexture.height / TOTAL_FRAMES_BUTTON },
        0, 0, 1
    };
    //============================
    int TOTAL_X_FRAMES_SLIME = 4;
    int TOTAL_Y_FRAMES_SLIME = 3;
    float frameWidth = (float) (slimeTexture.width / TOTAL_X_FRAMES_SLIME);
    float frameHeight = (float) (slimeTexture.height / TOTAL_Y_FRAMES_SLIME);
    SpritesheetGrid slimeSheet = {
        (Rectangle){ 0, 0, frameWidth, frameHeight },
        0, 0, 0, 4
    };


    SetTargetFPS(60);
    // Game Loop
    while (!WindowShouldClose()) {

        // Update
        updateBunny(&bunnySheet, bunnyTexture);
        updateButton(&buttonSheet, buttonTexture);
        updateSlime(&slimeSheet, slimeTexture, TOTAL_X_FRAMES_SLIME, TOTAL_Y_FRAMES_SLIME);

        // Draw
        BeginDrawing();
        ClearBackground(ORANGE);

        DrawTextureRec(bunnyTexture, bunnySheet.frameRec, (Vector2) { 0 } , WHITE);
        DrawTextureRec(buttonTexture, buttonSheet.frameRec, (Vector2) { bunnyTexture.width / TOTAL_FRAMES_BUNNY, 0 } , WHITE);
        DrawTextureRec(slimeTexture, slimeSheet.frameRec, (Vector2) { 0, bunnyTexture.height } , WHITE);

        EndDrawing();

    }

    UnloadTexture(bunnyTexture);
    UnloadTexture(buttonTexture);
    UnloadTexture(slimeTexture);
    CloseWindow();

    return 0;
}
