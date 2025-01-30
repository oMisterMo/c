#include "raylib.h"
#include <stdio.h>

#define NUM_FRAMES_BUNNY 9

#define NUM_LINES_BUTTON 3

#define NUM_FRAMES_SLIME 4
#define NUM_LINES_SLIME 3

typedef struct Spritesheet {
    Rectangle frameRec;         // Draw a part of a texture defined by a rectangle
    int currentFrame;           // The current frame, x-axis
    int currentLine;            // The current frame, y-axis
    int frameCounter;
    int frameSpeed;
} Spritesheet;

/**
 * Single row animation
 */
void updateBunny(Spritesheet *bunnySheet, Texture2D bunnyTexture) {
    bunnySheet->frameCounter++;

    // Slow down frame speed
    if (bunnySheet->frameCounter >= (60 / bunnySheet->frameSpeed)) {

        // Time to update current frame index and reset counter
        bunnySheet->frameCounter = 0;
        bunnySheet->currentFrame++;

        // Clamp index - I don't want to display the last frame so I subtract 2 and not 1
        if (bunnySheet->currentFrame > NUM_FRAMES_BUNNY - 2) bunnySheet->currentFrame = 0;

        // Update source rect
        bunnySheet->frameRec.x = (float) bunnySheet->currentFrame * (float) bunnyTexture.width / NUM_FRAMES_BUNNY;


    }
}

/**
 * Single Column animation
 */
void updateButton(Spritesheet *buttonSheet, Texture2D buttonTexture) {
    buttonSheet->frameCounter++;

    if (buttonSheet->frameCounter >= (60 / buttonSheet->frameSpeed)) {
        buttonSheet->frameCounter = 0;
        buttonSheet->currentLine++;

        if (buttonSheet->currentLine > NUM_LINES_BUTTON - 1) buttonSheet->currentLine = 0;

        buttonSheet->frameRec.y = (float) buttonSheet->currentLine * (float) buttonTexture.height / NUM_LINES_BUTTON;
    }
}

/**
 * Grid animation
 */
void updateSlime(Spritesheet *slimeSheet, Texture2D slimeTexture) {
    slimeSheet->frameCounter++;

    float frameWidth = (float) (slimeTexture.width / NUM_FRAMES_SLIME);
    float frameHeight = (float) (slimeTexture.height / NUM_LINES_SLIME);

    if (slimeSheet->frameCounter >= (60 / slimeSheet->frameSpeed)) {

        slimeSheet->currentFrame++;

        if (slimeSheet->currentFrame >= NUM_FRAMES_SLIME) {
            slimeSheet->currentFrame = 0;
            slimeSheet->currentLine++;
            if (slimeSheet->currentLine >= NUM_LINES_SLIME) {
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

    Texture2D bunnyTexture = LoadTexture("resources/sprites/jumpbunny_anim1.png");
    Texture2D buttonTexture  = LoadTexture("resources/sprites/button.png");
    Texture2D slimeTexture  = LoadTexture("resources/sprites/slime_green.png");

    slimeTexture.width *= 8;
    slimeTexture.height *= 8;

    //============================
    Spritesheet bunnySheet = {
        (Rectangle){ 0, 0, bunnyTexture.width / NUM_FRAMES_BUNNY, bunnyTexture.height },
        0, 0, 0, 5
    };
    //============================
    Spritesheet buttonSheet = {
        (Rectangle){ 0, 0, buttonTexture.width, buttonTexture.height / NUM_LINES_BUTTON },
        0, 0, 0, 1
    };
    //============================
    float frameWidth =  (float) (slimeTexture.width / NUM_FRAMES_SLIME);
    float frameHeight = (float) (slimeTexture.height / NUM_LINES_SLIME);
    Spritesheet slimeSheet = {
        (Rectangle){ 0, 0, frameWidth, frameHeight },
        0, 0, 0, 4
    };


    SetTargetFPS(60);
    // Game Loop
    while (!WindowShouldClose()) {

        // Update
        updateBunny(&bunnySheet, bunnyTexture);
        updateButton(&buttonSheet, buttonTexture);
        updateSlime(&slimeSheet, slimeTexture);

        // Draw
        BeginDrawing();
        ClearBackground(ORANGE);

        DrawTextureRec(bunnyTexture, bunnySheet.frameRec, (Vector2) { 0 } , WHITE);
        DrawTextureRec(buttonTexture, buttonSheet.frameRec, (Vector2) { bunnyTexture.width / NUM_FRAMES_BUNNY, 0 } , WHITE);
        DrawTextureRec(slimeTexture, slimeSheet.frameRec, (Vector2) { 0, bunnyTexture.height } , WHITE);

        EndDrawing();

    }

    UnloadTexture(bunnyTexture);
    UnloadTexture(buttonTexture);
    UnloadTexture(slimeTexture);
    CloseWindow();

    return 0;
}
