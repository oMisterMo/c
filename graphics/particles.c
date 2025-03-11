#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "math.h"

// CONSTANTS
const int INITIAL_SCREEN_WIDTH = 960;
const int INITIAL_SCREEN_HEIGHT = 600;
const float INITIAL_CAMERA_ZOOM = 2.5f;
const float INITIAL_CAMERA_ROATION = 0.0f; // deg
const int NO_OF_FLOWERS = 8;

int screenWidth = INITIAL_SCREEN_WIDTH;
int screenHeight = INITIAL_SCREEN_HEIGHT;

const int GRID_LINE_LENGTH = 10500; // I chose a randomly large number

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
    Texture2D texture;          // multiple images
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    Spritesheet sheet;
    Rectangle src;
} Animation;

typedef struct Sprite {
    Texture2D texture;          // single image
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    Rectangle src;
} Sprite;

typedef struct Game {
    int id;
} Game;

void ToggleFullscreenCamera(Camera2D *camera) {
    ToggleFullscreen();

    if (IsWindowFullscreen()) {
        printf("Fullscreen\n");
        screenWidth = GetMonitorWidth(2);
        screenHeight = GetMonitorHeight(2);
    } else {
        printf("Windowed\n");
        screenWidth = INITIAL_SCREEN_WIDTH;
        screenHeight = INITIAL_SCREEN_HEIGHT;
    }

    camera->target.x = 0;
    camera->target.y = 0;
}

void ResetCamera(Camera2D *camera) {
    camera->zoom = INITIAL_CAMERA_ZOOM;
    camera->rotation = INITIAL_CAMERA_ROATION;
    camera->target = (Vector2) { 0 };
    camera->offset = (Vector2) { screenWidth/2.0f, screenHeight/2.0f  };
}

void HandleInput(Camera2D *camera) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f/camera->zoom);
        camera->target = Vector2Add(camera->target, delta);
    }

    // Camera zoom center of screen
    camera->zoom += ((float)GetMouseWheelMove()*0.4f);

    // Zoom based on mouse wheel
    float wheel = GetMouseWheelMove();
    if (wheel != 0){
        // Get the world point that is under the mouse
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), *camera);

        // Set the offset to where the mouse is
        camera->offset = GetMousePosition();

        // Set the target to match, so that the camera maps the world space point
        // under the cursor to the screen space point under the cursor at any zoom
        camera->target = mouseWorldPos;

        // Zoom increment
        float scaleFactor = 1.0f + (0.25f*fabsf(wheel));
        if (wheel < 0) scaleFactor = 1.0f/scaleFactor;
        camera->zoom = Clamp(camera->zoom*scaleFactor, 1.0f, 64.0f);
    }

    // Camera reset (zoom and rotation)
    if (IsKeyPressed(KEY_R)) {
        ResetCamera(camera);
    }
    if (IsKeyPressed(KEY_F)) {
        ToggleFullscreenCamera(camera);
    }
}


int main() {

    // Setup config
    printf("-------------------\n");
    printf("INIT WINDOW\n");
    printf("-------------------\n");
    // SetConfigFlags( FLAG_WINDOW_UNDECORATED );
    InitWindow(screenWidth, screenHeight, "Learn Colors");


    // Initialization
    printf("-------------------\n");
    printf("LOAD TEXTURES\n");
    printf("-------------------\n");
    // Texture2D star = LoadTexture("resources/sprites/star-fill.png");
    Texture2D star = LoadTexture("resources/sprites/stars_icon_hover.png");
    Texture2D flowers = LoadTexture("resources/sprites/flowers.png");

    printf("-------------------\n");
    printf("Init Variables\n");
    printf("-------------------\n");
    Game game = { 0 };
    printf("size of Game: %ld\n", sizeof(Game));

    // Star
    Rectangle src = { 0, 0, star.width, star.height };
    Rectangle dest = { 0, 0, star.width, star.height };
    Vector2 origin = { star.width / 2, star.height / 2 };

    // Flower
    int flowerW = flowers.width / NO_OF_FLOWERS;
    int flowerH = flowers.height / 2;
    Rectangle flowerSrc = { 0, 0, flowerW, flowerH };
    Rectangle flowerDest = { 0, 0, flowerW, flowerH };
    Vector2 flowersOrigin = { flowerDest.width / 2, flowerDest.height / 2 };

    // Camera
    Camera2D camera = { 0 };
    camera.target = (Vector2) { 0 };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = INITIAL_CAMERA_ROATION;
    camera.zoom = INITIAL_CAMERA_ZOOM;

    
    SetTargetFPS(60);
    
    printf("-------------------\n");
    printf("GAME\n");
    printf("-------------------\n");

    while(!WindowShouldClose()) {
        // Input
        HandleInput(&camera);
        // Update

        // Draw
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(camera);
            DrawLine(-GRID_LINE_LENGTH, 0, GRID_LINE_LENGTH, 0, Fade(WHITE, 0.1f));
            DrawLine(0, -GRID_LINE_LENGTH, 0, GRID_LINE_LENGTH, Fade(WHITE, 0.1f));
                // Draw bounds
                // DrawRectangleRec(src, BLUE);
                // DrawRectanglePro(dest, origin, 0, Fade(RED, 0.4f));

                // Draw shape
                // DrawTexturePro(star, src, dest, origin, sinf(GetTime()) * 90 , WHITE);
                DrawTexturePro(flowers, flowerSrc, flowerDest, flowersOrigin, sinf(GetTime()) * 90 , WHITE);
            EndMode2D();
        EndDrawing();

    }

    printf("-------------------\n");
    printf("DESTROY\n");
    printf("-------------------\n");

    UnloadTexture(star);
    UnloadTexture(flowers);
    CloseWindow();


    return 0;
}
