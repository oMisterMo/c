#include <stdio.h>
#include "raylib.h"
#include "math.h"


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
    // Texture2D star = LoadTexture("resources/sprites/star-fill.png");
    Texture2D star = LoadTexture("resources/sprites/stars_icon_hover.png");

    printf("-------------------\n");
    printf("Init Variables\n");
    printf("-------------------\n");
    Game game = { 0 };
    printf("size of Game: %ld\n", sizeof(Game));

    Rectangle src = { 0, 0, star.width, star.height };
    Rectangle dest = { (GetScreenWidth()) / 2, (GetScreenHeight()) / 2,
        star.width, star.height };
    Vector2 origin = { star.width / 2, star.height / 2 };

    
    SetTargetFPS(60);
    
    printf("-------------------\n");
    printf("GAME\n");
    printf("-------------------\n");


    while(!WindowShouldClose()) {
        // Input

        // Update

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);


        // Draw bounds
        // DrawRectangleRec(src, BLUE);
        // DrawRectanglePro(dest, origin, 0, Fade(RED, 0.4f));

        // Draw shape
        DrawTexturePro(star, src, dest, origin,
        sinf(GetTime()) * 90 , WHITE);

        EndDrawing();

    }

    printf("-------------------\n");
    printf("DESTROY\n");
    printf("-------------------\n");

    UnloadTexture(star);
    CloseWindow();


    return 0;
}
