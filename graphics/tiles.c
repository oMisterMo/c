#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
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

typedef struct BoolFlags {
    bool showGUI;
    bool showGUIwindow;
} BoolFlags;

typedef struct Game {
    Rectangle player;
    Rectangle worldBounds;
    Rectangle screenBounds;
    Rectangle guiWindow;

    Vector2 windowStart;
    Vector2 windowEnd ;

    BoolFlags boolFlags;

    int cameraType;
    Camera2D worldCamera;
    Camera2D screenCamera;

    Texture2D checkered;
    Texture2D tileset;
} Game;

Texture2D CreateCheckeredBackground() {
    // Checkered background
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

    Texture2D checkered = LoadTextureFromImage(checkedIm);
    UnloadImage(checkedIm);     // free img.data aka pixels
    return checkered;
}

void DrawAxis() {
    // DrawLine(-1000, 0, 1000, 0, WHITE);
    // DrawLine(0, -1000, 0, 1000, WHITE);
    DrawLineEx((Vector2){-5000,0}, (Vector2){5000,0}, 4, WHITE);
    DrawLineEx((Vector2){0,-5000}, (Vector2){0,5000}, 4, WHITE);
}

void DrawGUI(Game *game) {
    if (game->boolFlags.showGUI) {
        if (game->boolFlags.showGUIwindow) {
            DrawRectangleRec(game->guiWindow, ColorAlpha(GRAY, 0.8f));
        }
        DrawText(game->cameraType == CAMERA_WORLD ? "Camera World" : "Camera Screen", 20, WORLD_HEIGHT - 40, 20, WHITE);
        DrawText(TextFormat("x (%d,%d)", (int) game->windowStart.x / TILE_WIDTH, (int)(game->windowEnd.x / TILE_WIDTH) + 1), 20, 60, 20, WHITE);
        DrawText(TextFormat("y (%d,%d)", (int) game->windowStart.y / TILE_HEIGHT, (int)(game->windowEnd.y / TILE_HEIGHT) + 1), 20, 100, 20, WHITE);
        DrawText(TextFormat("player (%d,%d)", (int) game->player.x, (int)(game->player.y)), 20, 160, 20, WHITE);
        DrawText(TextFormat("screen (%d,%d)", (int) game->screenCamera.target.x, (int)(game->screenCamera.target.y)), 20, 200, 20, WHITE);
        DrawText(TextFormat("world  (%d,%d)", (int) game->worldCamera.target.x, (int)(game->worldCamera.target.y)), 20, 240, 20, WHITE);
    }
}

void DrawCameraWorld(Game *game) {
    BeginMode2D(game->worldCamera);
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
    DrawRectangleLinesEx(game->worldBounds, 8, YELLOW);
    DrawRectangleLinesEx(game->screenBounds, 8, BLUE);

    DrawRectangleRec(game->player, ORANGE);
    EndMode2D();
}

void DrawCameraScreen(Game *game) {
    // TODO: this method is exactly identical to DrawCamerWorld except for the line below (at this moment).
    BeginMode2D(game->screenCamera);
    DrawAxis();

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
    DrawRectangleLinesEx(game->worldBounds, 8, YELLOW);
    DrawRectangleLinesEx(game->screenBounds, 8, BLUE);

    DrawRectangleRec(game->player, ORANGE);
    EndMode2D();
}

void Input(Game *game) {
    if (IsKeyPressed(KEY_SPACE)) {
        if (game->cameraType == CAMERA_WORLD) {
            game->cameraType = CAMERA_SCREEN;
        } else {
            game->cameraType = CAMERA_WORLD;
        }
    }
    if (IsKeyPressed(KEY_EQUAL)) {
        // *lineThick += 1;
    }
    if (IsKeyPressed(KEY_ONE)) {
        game->boolFlags.showGUI = !game->boolFlags.showGUI;
    }
    if (IsKeyPressed(KEY_TWO)) {
        game->boolFlags.showGUIwindow = !game->boolFlags.showGUIwindow;
    }
    if (IsKeyPressed(KEY_THREE)) {
        // nothing yet
    }
}

void Update(Game *game) {
    if (game->cameraType == CAMERA_WORLD) {
        float speed = 800 * GetFrameTime();
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
            game->worldCamera.target.y -= speed;
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
            game->worldCamera.target.y += speed;
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            game->worldCamera.target.x -= speed;
        }
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            game->worldCamera.target.x += speed;
        }
    }

    if (game->cameraType == CAMERA_SCREEN) {
        float speed = 300 * GetFrameTime();
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
            game->player.y -= speed;
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
            game->player.y += speed;
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            game->player.x -= speed;
        }
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            game->player.x += speed;
        }

        game->screenCamera.target.x = game->player.x;
        game->screenCamera.target.y = game->player.y;
        game->screenBounds.x = game->player.x - SCREEN_WIDTH / 2;
        game->screenBounds.y = game->player.y - SCREEN_HEIGHT / 2;

        // Bound blue window to game world
        if (game->screenBounds.x < game->worldBounds.x) game->screenBounds.x = game->worldBounds.x;
        if (game->screenBounds.x + game->screenBounds.width > game->worldBounds.width) game->screenBounds.x = game->worldBounds.width - game->screenBounds.width;
        if (game->screenBounds.y < game->worldBounds.y) game->screenBounds.y = game->worldBounds.y;
        if (game->screenBounds.y + game->screenBounds.height > game->worldBounds.height) game->screenBounds.y = game->worldBounds.height - game->screenBounds.height;
        // Bound screen camera to game world
        if (game->screenCamera.target.x < WORLD_WIDTH / 4) game->screenCamera.target.x = WORLD_WIDTH / 4;
        if (game->screenCamera.target.x + WORLD_WIDTH / 4 > game->worldBounds.width) game->screenCamera.target.x = game->worldBounds.width - WORLD_WIDTH / 4;
        if (game->screenCamera.target.y < WORLD_HEIGHT / 4) game->screenCamera.target.y = WORLD_HEIGHT / 4;
        if (game->screenCamera.target.y + WORLD_HEIGHT / 4 > game->worldBounds.height) game->screenCamera.target.y = game->worldBounds.height - WORLD_HEIGHT / 4;
    }

    // if (worldCamera.target.x < 0) worldCamera.target.x = 0;
    // if (worldCamera.target.y < 0) worldCamera.target.y = 0;
    // if (worldCamera.target.x > WORLD_WIDTH) worldCamera.target.x = WORLD_WIDTH;
    // if (worldCamera.target.y > WORLD_HEIGHT) worldCamera.target.y = WORLD_HEIGHT;

}

void Draw(Game *game) {
    BeginDrawing();
        ClearBackground(BLACK);

        DrawTexture(game->checkered, 0, 0, WHITE);
        DrawTexture(game->tileset, 0, 0, WHITE);

        if (game->cameraType == CAMERA_WORLD) {
            DrawCameraWorld(game);
        } else if (game->cameraType == CAMERA_SCREEN) {
            DrawCameraScreen(game);
        }

        DrawGUI(game);

        DrawFPS(20, 20);
    EndDrawing();
}

int main(void) {

    InitWindow(WORLD_WIDTH, WORLD_HEIGHT, "Tiles");

    Texture2D checkered = CreateCheckeredBackground();
    Texture2D tileset = LoadTexture("resources/tilesets/world_tileset.png");

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

    // GUI
    Rectangle guiWindow = { 0, 0, 300, WORLD_HEIGHT };
    BoolFlags boolFlags = {
        .showGUI = false,
        .showGUIwindow = true
    };

    // Test
    Vector2 windowStart = GetScreenToWorld2D((Vector2){0,0}, worldCamera);
    Vector2 windowEnd = GetScreenToWorld2D((Vector2){WORLD_WIDTH,WORLD_HEIGHT}, worldCamera);

    // Game
    Game game = { 0 };
    game.player = player;
    game.worldBounds = worldBounds;
    game.screenBounds = screenBounds;
    game.guiWindow = guiWindow;
    game.windowStart = windowStart;
    game.windowEnd = windowEnd;
    game.boolFlags = boolFlags;
    game.cameraType = cameraType;
    game.worldCamera = worldCamera;
    game.screenCamera =screenCamera;
    game.checkered = checkered;
    game.tileset = tileset;

    // Log some stuff
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
        Input(&game);

        // update
        Update(&game);

        // draw
        Draw(&game);
    }

    // free(tiles);
    UnloadTexture(game.checkered);
    UnloadTexture(game.tileset);
    CloseWindow();

    return 0;
}
