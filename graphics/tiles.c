#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"

#define WORLD_WIDTH 1280
#define WORLD_HEIGHT 720

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 270

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#define NO_OF_TILES_X WORLD_WIDTH / TILE_WIDTH 
#define NO_OF_TILES_Y WORLD_HEIGHT / TILE_HEIGHT

enum CameraType {
    CAMERA_WORLD,
    CAMERA_SCREEN
};

void DrawAxis() {
    // DrawLine(-1000, 0, 1000, 0, WHITE);
    // DrawLine(0, -1000, 0, 1000, WHITE);
    DrawLineEx((Vector2){-5000,0}, (Vector2){5000,0}, 4, WHITE);
    DrawLineEx((Vector2){0,-5000}, (Vector2){0,5000}, 4, WHITE);
}

void DrawGUI(Rectangle guiWindow, float guiAlpha, int cameraType, Vector2 windowStart, Vector2 windowEnd) {
    DrawRectangleRec(guiWindow, ColorAlpha(BLUE, guiAlpha));
    DrawText(cameraType == CAMERA_WORLD ? "Camera World" : "Camera Screen", 20, WORLD_HEIGHT - 40, 20, WHITE);
    DrawText(TextFormat("x (%d,%d)", (int) windowStart.y / TILE_HEIGHT, (int)(windowEnd.y / TILE_HEIGHT) + 1), 20, 60, 20, WHITE);
    DrawText(TextFormat("y (%d,%d)", (int) windowStart.x / TILE_WIDTH, (int)(windowEnd.x / TILE_WIDTH) + 1), 20, 100, 20, WHITE);
}

void DrawCameraWorld(Camera2D worldCamera, Texture checked, Rectangle worldBounds, Rectangle screenBounds, Rectangle player) {
    BeginMode2D(worldCamera);
    DrawTexture(checked, 0, 0, WHITE);
    DrawAxis();

    // printf("windowStart %d, %d\n", (int) windowStart.x, (int) windowStart.y);
    // printf("windowEnd %d, %d\n", (int) windowEnd.x, (int) windowEnd.y);
    // Draw every tile
    // int i = 0;
    // for (int y = 0; y < NO_OF_TILES_Y; ++y) {
    //     for (int x = 0; x < NO_OF_TILES_X; ++x) {
    //         // DrawRectangleRec(tiles[i], RED);
    //         // DrawRectangleLinesEx(tiles[i], lineThick % 4, WHITE);
    //         DrawRectangle(x * TILE_WIDTH, y * TILE_HEIGHT,
    //             TILE_WIDTH, TILE_HEIGHT, RED);
    //         DrawRectangleLines(x * TILE_WIDTH, y * TILE_HEIGHT,
    //             TILE_WIDTH, TILE_HEIGHT, WHITE);
    //         ++i;
    //     }
    // }

    // Draw only visible tiles
    // int i = 0;
    // int startX = (int) windowStart.x / TILE_WIDTH;
    // int endX = (int) (windowEnd.x / TILE_WIDTH) + 1;
    // int startY = (int) windowStart.y / TILE_HEIGHT;
    // int endY = (int) (windowEnd.y / TILE_HEIGHT) + 1;

    // for (int y = startY; y < endY     ; ++y) {
    //     for (int x = startX; x < endX ; ++x) {
    //         // DrawRectangleRec(tiles[i], RED);
    //         // DrawRectangleLinesEx(tiles[i], lineThick % 4, WHITE);
    //         // if (startX <= 0) startX = 0;
    //         // if (startY <= 0) startY = 0;
    //         // if (endX < NO_OF_TILES_X) endX = NO_OF_TILES_X;
    //         // if (endY < NO_OF_TILES_Y) endY = NO_OF_TILES_Y;
    //         // if (endX > NO_OF_TILES_X) endX = NO_OF_TILES_X;
    //         // if (endY > NO_OF_TILES_Y) endY = NO_OF_TILES_Y;

    //         DrawRectangle(x * TILE_WIDTH, y * TILE_HEIGHT,
    //             TILE_WIDTH, TILE_HEIGHT, RED);
    //         DrawRectangleLines(x * TILE_WIDTH, y * TILE_HEIGHT,
    //             TILE_WIDTH, TILE_HEIGHT, WHITE);
    //         ++i;
    //     }
    // }

    int i = 0;
    for (int y = 0; y < (int) (WORLD_HEIGHT / TILE_HEIGHT) + 1; ++y) {
        for (int x = 0; x < (int) (WORLD_WIDTH / TILE_WIDTH) + 1; ++x) {
            // DrawRectangleRec(tiles[i], RED);
            // DrawRectangleLinesEx(tiles[i], lineThick % 4, WHITE);
            DrawRectangle(x * TILE_WIDTH, y * TILE_HEIGHT,
                TILE_WIDTH, TILE_HEIGHT, RED);
            DrawRectangleLines(x * TILE_WIDTH, y * TILE_HEIGHT,
                TILE_WIDTH, TILE_HEIGHT, WHITE);
            ++i;
        }
    }
    DrawRectangleLinesEx(worldBounds, 8, YELLOW);
    DrawRectangleLinesEx(screenBounds, 8, BLUE);

    DrawRectangleRec(player, ORANGE);
    EndMode2D();
}

void DrawCameraScreen() {

}


int main(void) {

    InitWindow(WORLD_WIDTH, WORLD_HEIGHT, "Tiles");

    // Checkard background
    Color *pixels = (Color *)malloc(WORLD_WIDTH*WORLD_HEIGHT*sizeof(Color));

    for (int y = 0; y < WORLD_HEIGHT; y++)
    {
        for (int x = 0; x < WORLD_WIDTH; x++)
        {
            if (((x/32+y/32)/1)%2 == 0) pixels[y*WORLD_WIDTH + x] = ColorAlpha(ORANGE, 0.1f);
            else pixels[y*WORLD_WIDTH + x] = ColorAlpha(GOLD, 0.1f);
        }
    }

    // Load pixels data into an image structure and create texture
    Image checkedIm = {
        .data = pixels,             // We can assign pixels directly to data
        .width = WORLD_WIDTH,
        .height = WORLD_HEIGHT,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .mipmaps = 1
    };

    Texture2D checked = LoadTextureFromImage(checkedIm);
    UnloadImage(checkedIm);     // free img.data aka pixels

    // World
    // Rectangle *tiles = malloc(sizeof(Rectangle) * NO_OF_TILES_X * NO_OF_TILES_Y);
    Rectangle tiles[NO_OF_TILES_X * NO_OF_TILES_Y];
    Rectangle tile = { 0, 0, TILE_WIDTH, TILE_HEIGHT };
    int lineThick = 1;

    int i = 0;
    for (int y = 0; y < NO_OF_TILES_Y; ++y) {
        for (int x = 0; x < NO_OF_TILES_X; ++x) {
            // y * NO_OF_TILES_Y + x
            // printf("%d - %d\n", i, y * NO_OF_TILES_Y + x);
            tiles[i] = (Rectangle) {
                x * TILE_WIDTH, y * TILE_HEIGHT,
                TILE_WIDTH, TILE_HEIGHT
            };
            ++i;
        }
    }

    // Player
    Rectangle player = { SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 20};

    // Camera
    Camera2D worldCamera = { 0 };
    worldCamera.offset = (Vector2){ WORLD_WIDTH/2.0f, WORLD_HEIGHT/2.0f };
    worldCamera.target = (Vector2){ WORLD_WIDTH/2.0f, WORLD_HEIGHT/2.0f };
    worldCamera.rotation = 0.0f;
    worldCamera.zoom = 1.0f;

    Camera2D screenCamera = { 0 };
    screenCamera.offset = (Vector2){ WORLD_WIDTH/2.0f, WORLD_HEIGHT/2.0f };
    screenCamera.target = (Vector2){ player.x, player.y };
    screenCamera.rotation = 0.0f;
    screenCamera.zoom = 2.0f;

    Rectangle worldBounds = { 0, 0, WORLD_WIDTH, WORLD_HEIGHT };
    Rectangle screenBounds = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    int cameraType = CAMERA_WORLD;

    float guiAlpha = 0.0f;  // Click on left size of world to show
    Rectangle guiWindow = { 0, 0, 170, WORLD_HEIGHT };

    // Test
    Vector2 windowStart = GetScreenToWorld2D((Vector2){0,0}, worldCamera);
    Vector2 windowEnd = GetScreenToWorld2D((Vector2){SCREEN_WIDTH,SCREEN_HEIGHT}, worldCamera);

    printf("==========================\n");
    printf("Moo\n");
    printf("==========================\n");
    int TOTAL_TILES_X = (int) (windowEnd.x / TILE_WIDTH) + 1;
    int TOTAL_TILES_Y = (int) (windowEnd.y / TILE_HEIGHT) + 1;
    printf("windowStart %d, %d\n", (int) windowStart.x, (int) windowStart.y);
    printf("windowEnd %d, %d\n", (int) windowEnd.x, (int) windowEnd.y);
    printf("Total x tiles: %d\n", TOTAL_TILES_X);
    printf("Total y tiles: %d\n", TOTAL_TILES_Y);

    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        // input
        if (IsKeyPressed(KEY_SPACE)) {
            if (cameraType == CAMERA_WORLD) {
                cameraType = CAMERA_SCREEN;
            } else {
                cameraType = CAMERA_WORLD;
            }
        }
        if (IsKeyPressed(KEY_ONE)) {
            lineThick += 1;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(GetMousePosition(), guiWindow)) {
                guiAlpha =  1 - guiAlpha;
            }
        }

        // update
        float speed = 400 * GetFrameTime();
        if (cameraType == CAMERA_WORLD) {
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
                worldCamera.target.y -= speed;
            }
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
                worldCamera.target.y += speed;
            }
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                worldCamera.target.x -= speed;
            }
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                worldCamera.target.x += speed;
            }
        }

        if (cameraType == CAMERA_SCREEN) {
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
                player.y -= speed;
            }
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
                player.y += speed;
            }
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                player.x -= speed;
            }
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                player.x += speed;
            }

            screenCamera.target.x = player.x;
            screenCamera.target.y = player.y;
            screenBounds.x = player.x - SCREEN_WIDTH / 2;
            screenBounds.y = player.y - SCREEN_HEIGHT / 2;
        }

        // if (worldCamera.target.x < 0) worldCamera.target.x = 0;
        // if (worldCamera.target.y < 0) worldCamera.target.y = 0;
        // if (worldCamera.target.x > WORLD_WIDTH) worldCamera.target.x = WORLD_WIDTH;
        // if (worldCamera.target.y > WORLD_HEIGHT) worldCamera.target.y = WORLD_HEIGHT;

        // draw
        Vector2 windowStart = GetScreenToWorld2D((Vector2){0,0}, worldCamera);
        Vector2 windowEnd = GetScreenToWorld2D((Vector2){SCREEN_WIDTH,SCREEN_HEIGHT}, worldCamera);
        BeginDrawing();
            ClearBackground(BLACK);


            if (cameraType == CAMERA_WORLD) {
                DrawCameraWorld(worldCamera, checked, worldBounds, screenBounds, player);
            } else if (cameraType == CAMERA_SCREEN) {
                DrawCameraScreen();
                DrawCameraWorld(screenCamera, checked, worldBounds, screenBounds, player);
            }
            
            DrawGUI(guiWindow, guiAlpha, cameraType, windowStart, windowEnd);

            DrawFPS(20, 20);
        EndDrawing();
    }

    // free(tiles);
    CloseWindow();

    return 0;
}
