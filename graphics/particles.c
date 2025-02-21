#include <stdio.h>
#include "raylib.h"


typedef enum {
    IDLE = 0,
    TWEEN,
} TweenState;

typedef struct Spritesheet {
    Rectangle frameRec;         // Draw a part of a texture defined by a rectangle
    int currentFrame;           // The current frame, x-axis. ( frameRect.x * currentFrame )
    int currentLine;            // The current frame, y-axis. ( frameRect.y * currentLine )
    int frameCounter;
    int frameSpeed;
    bool isAnimating;
} Spritesheet;

typedef struct Animation {
    Texture2D *texture;
    Vector2 position;
    Spritesheet sheet;
} Animation;

typedef struct Game {
    int id;
    // Card cards[NO_OF_CARDS];
    // Tray trays[NO_OF_TRAYS];
    // Color *colors;
    // Texture2D *cardTextures;
    // Texture2D *trayTexture;
    // Animation *stars;
    // int frameCounter;
    // int score;
    // int counter;
    // Texture2D nPatchTexture;
    // NPatchInfo nPatchSrc;
} Game;


int main() {

    // Setup config
    printf("-------------------\n");
    printf("INIT WINDOW\n");
    printf("-------------------\n");
    int screenWidth = 960;
    int screenHeight = 600;
    // SetConfigFlags( FLAG_WINDOW_UNDECORATED );
    InitWindow(screenWidth, screenHeight, "Learn Colors");


    // Initialization
    printf("-------------------\n");
    printf("LOAD TEXTURES\n");
    printf("-------------------\n");

    printf("-------------------\n");
    printf("Init Variables\n");
    printf("-------------------\n");
    Game game = { 0 };
    printf("size of Game: %ld\n", sizeof(Game));

    
    SetTargetFPS(60);
    
    printf("-------------------\n");
    printf("GAME\n");
    printf("-------------------\n");


    while(!WindowShouldClose()) {
        // Input

        // Update

        // Draw
        BeginDrawing();
        ClearBackground(DARKGRAY);


        EndDrawing();

    }

    printf("-------------------\n");
    printf("DESTROY\n");
    printf("-------------------\n");

    CloseWindow();


    return 0;
}
