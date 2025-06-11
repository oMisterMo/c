/**
 * Functional Requirements:
 *  - Set background tiles in UI
 *  - Render background tiles
 *  - Show toast when user saves/loads/resets file
 *  - Player spawn location (ID = 'p')
 *  - Middle click to drag
 *  - Scroll to zoom
 *  - Select multiple tiles
 *  - Draw multiple tiles
 *  - Add RayGUI
 *      - Fill horizonal tiles ✔️
 *      - Fill vertical tiles ✔️
 *      - Fill all tiles
 *      - Save multiple files
 *      - Load multiple files
 *          - Dropdown list of all maps in resources
 *          - Click to load
 *      - Set custom tiles x/y
 *      - Set custom tile size width/height
 *      - Add export button
 *
 *
 *
 * Non-Functional Requirements:
 *  - Rain animation
 *  - Snow animation
 *  - Particles
 *  - Screen shake
 *  - Add more usable tiles
 *  - Better way to load texture atlas (using ray texture packer)
 *  - Player physics/collisions
 *
 *
 * Number of character idsL
 * A-Z
 * a-z
 * 0-9
 *
 * Total = 62 unique
 *
 *
 * ======================
 *
 * Mario Maker
 *
 * 27 or 13 Tiles          (y-axis)
 * 270 or 240 Tiles        (x-axis)
 *
 * TILE_WIDTH * NO_OF_TILES_X = TOTAL PIXELS
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include "raylib.h"
#include "raymath.h"

#define WORLD_WIDTH 1280
#define WORLD_HEIGHT 720

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 270

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#define NO_OF_TILES_X WORLD_WIDTH / TILE_WIDTH      // 40
#define NO_OF_TILES_Y WORLD_HEIGHT / TILE_HEIGHT    // 22.5

int blankTile = 3;  // Blank tile at index 3, could use any random number

enum CameraType {
    CAMERA_TILESET = 0,
    CAMERA_WORLD,
    CAMERA_SCREEN,
    CAMERA_SCREEN_SHAKE,
};

enum TileType {
    TILE_EMPTY = 0,

    TILE_GRASS_TOP,
    TILE_GRASS_MIDDLE,
    TILE_GRASS_QUESTION,
    TILE_GRASS_BRICK,
    TILE_GRASS_BRICK_BROKEN,
    TILE_GRASS_EXCLAMATION,

    TILE_BOX,
    TILE_SIGN,
    TILE_LADDER,
};

enum FillMode {
    FILL_OFF = 0,
    FILL_EMPTY,
    FILL_ALL,
    FILL_HORIZONTAL,
    FILL_VERTICAL
};

typedef struct BoolFlags {
    bool showGUI;
    bool showGUIwindow;
    bool showWindowBorder;
    bool showScreenBorder;
} BoolFlags;

typedef struct ScreenShake {
    float shake;
    float maxAngle;
    float maxOffset;
    float angle;
    float offsetX;
    float offsetY;
} ScreenShake;

typedef struct Tile {
    int id; // what tile (grass, water)
    Rectangle srcRect;  // spritesheet index
    Rectangle destRect; // position
} Tile;

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
    Camera2D shakyCamera;
    ScreenShake screenShake;

    Texture2D checkered;
    Texture2D tileset;

    Vector2 tileSelected;
    Tile *tiles;
    int fillMode;
    bool overwriteTiles;
} Game;

float GetRandomValueFloat(float min, float max) {
    // Use perlin noise, not random values
    // https://www.youtube.com/watch?v=tu-Qe66AvtY
    return (((float)rand()/(float)(RAND_MAX)) * 2) - 1;
}

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
        switch (game->cameraType) {
            case CAMERA_TILESET:
                DrawText("Camera Tileset", 20, WORLD_HEIGHT - 40, 20, WHITE);
                break;
            case CAMERA_WORLD:
                DrawText("Camera World", 20, WORLD_HEIGHT - 40, 20, WHITE);
                break;
            case CAMERA_SCREEN:
                DrawText("Camera Screen", 20, WORLD_HEIGHT - 40, 20, WHITE);
                break;
            case CAMERA_SCREEN_SHAKE:
                DrawText("Camera Shake", 20, WORLD_HEIGHT - 40, 20, WHITE);
                break;
        }

        Vector2 windowStart = GetScreenToWorld2D((Vector2){0,0}, game->worldCamera);
        Vector2 windowEnd = GetScreenToWorld2D((Vector2){WORLD_WIDTH,WORLD_HEIGHT}, game->worldCamera);
        Vector2 screenStart = GetScreenToWorld2D((Vector2){0,0}, game->screenCamera);
        Vector2 screenEnd = GetScreenToWorld2D((Vector2){SCREEN_WIDTH,SCREEN_HEIGHT}, game->screenCamera);

        DrawText(TextFormat("x (%d,%d)", (int) game->windowStart.x / TILE_WIDTH, (int)(game->windowEnd.x / TILE_WIDTH) + 1), 20, 60, 20, WHITE);
        DrawText(TextFormat("y (%d,%d)", (int) game->windowStart.y / TILE_HEIGHT, (int)(game->windowEnd.y / TILE_HEIGHT) + 1), 20, 100, 20, WHITE);
        DrawText(TextFormat("player (%d,%d)", (int) game->player.x, (int)(game->player.y)), 20, 160, 20, WHITE);
        DrawText(TextFormat("world  (%d,%d) x%.2f", (int) game->worldCamera.target.x, (int)(game->worldCamera.target.y), game->worldCamera.zoom), 20, 200, 20, WHITE);
        DrawText(TextFormat("screen (%d,%d) x%.2f", (int) game->screenCamera.target.x, (int)(game->screenCamera.target.y), game->screenCamera.zoom), 20, 240, 20, WHITE);


        DrawRectangle(20, 265, 240, 10, ColorAlpha(BLACK, 0.5f));
        DrawText(TextFormat("w start (%d,%d)", (int) windowStart.x , (int)(windowStart.y)), 20, 280, 20, WHITE);
        DrawText(TextFormat("w end (%d,%d)", (int) windowEnd.x , (int)(windowEnd.y)), 20, 320, 20, WHITE);
        DrawText(TextFormat("s start (%d,%d)", (int) screenStart.x , (int)(screenStart.y)), 20, 360, 20, WHITE);
        DrawText(TextFormat("s end (%d,%d)", (int) screenEnd.x , (int)(screenEnd.y)), 20, 400, 20, WHITE);


        DrawText(TextFormat("shake (%.2f)",  game->screenShake.shake), 20, 440, 20, WHITE);
    }
}

void DrawCameraWorld(Game *game) {
    BeginMode2D(game->worldCamera);
    DrawAxis();

    // Red tiles
    int i = 0;
    for (int y = 0; y < NO_OF_TILES_Y; ++y) {
        for (int x = 0; x < NO_OF_TILES_X; ++x) {
            DrawRectangle(x * TILE_WIDTH, y * TILE_HEIGHT,
                TILE_WIDTH, TILE_HEIGHT, GRAY);
            DrawRectangleLines(x * TILE_WIDTH, y * TILE_HEIGHT,
                TILE_WIDTH, TILE_HEIGHT, WHITE);
            ++i;
        }
    }

    // Texture tiles
    for (int y = 0; y < NO_OF_TILES_Y; ++y) {
        for (int x = 0; x < NO_OF_TILES_X; ++x) {
            // y * NO_OF_TILES_X + x
            Rectangle src = game->tiles[y * NO_OF_TILES_X + x].srcRect;
            Rectangle dest = game->tiles[y * NO_OF_TILES_X + x].destRect;
            DrawTexturePro(game->tileset,src,dest,(Vector2){0},0, WHITE);
        }
    }

    if (game->boolFlags.showWindowBorder) {
        DrawRectangleLinesEx(game->worldBounds, 8, YELLOW);
    }
    if (game->boolFlags.showScreenBorder) {
        DrawRectangleLinesEx(game->screenBounds, 8, BLUE);
    }

    DrawRectangleRec(game->player, ColorAlpha(ORANGE, 0.2f));
    EndMode2D();
}

void DrawCameraScreen(Game *game) {
    // TODO: this method is identical to DrawCamerWorld except for the line below, and the drawing of empty tiles.
    BeginMode2D(game->screenCamera);
    DrawAxis();

    // Texture tiles
    for (int y = 0; y < NO_OF_TILES_Y; ++y) {
        for (int x = 0; x < NO_OF_TILES_X; ++x) {
            Rectangle src = game->tiles[y * NO_OF_TILES_X + x].srcRect;
            Rectangle dest = game->tiles[y * NO_OF_TILES_X + x].destRect;
            DrawTexturePro(game->tileset,src,dest,(Vector2){0},0, WHITE);
        }
    }

    if (game->boolFlags.showWindowBorder) {
        DrawRectangleLinesEx(game->worldBounds, 8, YELLOW);
    }
    if (game->boolFlags.showScreenBorder) {
        DrawRectangleLinesEx(game->screenBounds, 8, BLUE);
    }

    DrawRectangleRec(game->player, ColorAlpha(ORANGE, 0.2f));
    EndMode2D();
}

void DrawCameraScreenShake(Game *game) {
    // TODO: this method is exactly identical to DrawCamerScreen except for the line below (at this moment).
    BeginMode2D(game->shakyCamera);
    DrawAxis();

    // Texture tiles
    for (int y = 0; y < NO_OF_TILES_Y; ++y) {
        for (int x = 0; x < NO_OF_TILES_X; ++x) {
            Rectangle src = game->tiles[y * NO_OF_TILES_X + x].srcRect;
            Rectangle dest = game->tiles[y * NO_OF_TILES_X + x].destRect;
            DrawTexturePro(game->tileset,src,dest,(Vector2){0},0, WHITE);
        }
    }

    if (game->boolFlags.showWindowBorder) {
        DrawRectangleLinesEx(game->worldBounds, 8, YELLOW);
    }
    if (game->boolFlags.showScreenBorder) {
        DrawRectangleLinesEx(game->screenBounds, 8, BLUE);
    }

    DrawRectangleRec(game->player, ColorAlpha(ORANGE, 0.2f));
    EndMode2D();
}

int TileIndexToId(int x, int y) {
    int id = TILE_EMPTY;

    // Wall
    if (x == 0 && y == 0) {
        id = TILE_GRASS_TOP;
    }
    if (x == 0 && y == 1) {
        id = TILE_GRASS_MIDDLE;
    }
    if (x == 0 && y == 2) {
        id = TILE_GRASS_QUESTION;
    }
    if (x == 1 && y == 0) {
        id = TILE_GRASS_BRICK;
    }
    if (x == 1 && y == 1) {
        id = TILE_GRASS_BRICK_BROKEN;
    }
    if (x == 1 && y == 2) {
        id = TILE_GRASS_EXCLAMATION;
    }

    // Others
    if (x == 7 && y == 3) {
        id = TILE_BOX;
    }
    if (x == 8 && y == 3) {
        id = TILE_SIGN;
    }
    if (x == 9 && y == 3) {
        id = TILE_LADDER;
    }
    return id;
}

Vector2 IdToTileIndex(char id) {
    Vector2 index = {blankTile, blankTile};

    // Wall
    if (id == TILE_GRASS_TOP) {
        index.x = 0;
        index.y = 0;
    }
    if (id == TILE_GRASS_MIDDLE) {
        index.x = 0;
        index.y = 1;
    }
    if (id == TILE_GRASS_QUESTION) {
        index.x = 0;
        index.y = 2;
    }
    if (id == TILE_GRASS_BRICK) {
        index.x = 1;
        index.y = 0;
    }
    if (id == TILE_GRASS_BRICK_BROKEN) {
        index.x = 1;
        index.y = 1;
    }
    if (id == TILE_GRASS_EXCLAMATION) {
        index.x = 1;
        index.y = 2;
    }

    // Others
    if (id == TILE_BOX) {
        index.x = 7;
        index.y = 3;
    }
    if (id == TILE_SIGN) {
        index.x = 8;
        index.y = 3;
    }
    if (id == TILE_LADDER) {
        index.x = 9;
        index.y = 3;
    }
    return index;
}

void SaveMap(Game *game) {
    // Calculate size required
    // int size = NO_OF_TILES_X * NO_OF_TILES_Y * 2;
    // I'll do it later
    // Just using random number thats high enough for now


    // Save map to text
    char data[8192 * 2] = "";
    size_t offset = 0;

    for (int y = 0; y < NO_OF_TILES_Y; ++y){
        for (int x = 0; x < NO_OF_TILES_X; ++x) {
            int xIndex = (int) game->tiles[y * NO_OF_TILES_X + x].srcRect.x / TILE_WIDTH;
            int yIndex = (int) game->tiles[y * NO_OF_TILES_X + x].srcRect.y / TILE_HEIGHT;

            // Store the {x,y} texture location
            // offset += snprintf(data + offset, sizeof(data) - offset, "{%d, %d} ", xIndex, yIndex);

            // Store the ID
            offset += snprintf(data + offset, sizeof(data) - offset, "%d ", TileIndexToId(xIndex, yIndex));
        }
        offset += snprintf(data + offset, sizeof(data) - offset, "\n");
    }
    SaveFileText("./resources/maps/map.txt", data);
}

void LoadMap(Game *game) {
    // ATTEMPT 2
    FILE *file = fopen("./resources/maps/map.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int ch;
    int i = 0;
    while (fscanf(file, "%d", &ch) == 1) {
        Vector2 index = IdToTileIndex(ch);
        game->tiles[i].id = ch;
        game->tiles[i].srcRect = (Rectangle) {index.x * TILE_WIDTH, index.y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT};
        // putchar(ch);
        // printf("%d", ch);
        ++i;
    }

    fclose(file);


    // ATTEMPT 1
    // char *map = LoadFileText("map.txt");
    // // printf("%s", map);
    // for (int y = 0; y < NO_OF_TILES_Y; ++y) {
    //     for (int x = 0; x < NO_OF_TILES_X; ++x) {
    //         char id = map[y * NO_OF_TILES_X + x];
    //         Vector2 index = IdToTileIndex(id - '0');
    //         game->tiles[y * NO_OF_TILES_X + x].id = id;
    //         game->tiles[y * NO_OF_TILES_X + x].srcRect = (Rectangle) {index.x * TILE_WIDTH, index.y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT};
    //         printf("%c", id);

    //     }
    // }
    // UnloadFileText(map);
}

void Input(Game *game) {
    if (IsKeyPressed(KEY_SPACE)) {
        if (game->cameraType == CAMERA_WORLD) {
            game->cameraType = CAMERA_TILESET;
        } else {
            game->cameraType = CAMERA_WORLD;
        }
    }
    if (IsKeyPressed(KEY_EQUAL)) {
        // *lineThick += 1;
    }
    if (IsKeyPressed(KEY_ONE)) {
        game->cameraType = CAMERA_TILESET;
        // game->boolFlags.showGUI = !game->boolFlags.showGUI;
    }
    if (IsKeyPressed(KEY_TWO)) {
        game->cameraType = CAMERA_WORLD;
        // game->boolFlags.showScreenBorder = false;
        // game->boolFlags.showGUIwindow = !game->boolFlags.showGUIwindow;
    }
    if (IsKeyPressed(KEY_THREE)) {
        game->cameraType = CAMERA_SCREEN;
        // game->boolFlags.showScreenBorder = true;
    }
    // Screen shake
    if (IsKeyPressed(KEY_FOUR)) {
        printf("Shake...\n");
        game->cameraType = CAMERA_SCREEN_SHAKE;

        // TODO method to add shake so that I can limit in one place
        game->screenShake.shake += 0.3f;
        if (game->screenShake.shake > 1.0f) game->screenShake.shake = 1.0f;
    }


    if (IsKeyPressed(KEY_EIGHT)) {
        game->boolFlags.showWindowBorder = !game->boolFlags.showWindowBorder;
    }
    if (IsKeyPressed(KEY_NINE)) {
        game->boolFlags.showScreenBorder = !game->boolFlags.showScreenBorder;
    }
    if (IsKeyPressed(KEY_ZERO)) {
        game->boolFlags.showGUI = !game->boolFlags.showGUI;
    }
    // Reset
    if (IsKeyPressed(KEY_R)) {
        printf("Reset tiles...\n");
        for (int y = 0; y < NO_OF_TILES_Y; ++y) {
            for (int x = 0; x < NO_OF_TILES_X; ++x) {
                game->tiles[y * NO_OF_TILES_X + x].id = TILE_EMPTY;
                game->tiles[y * NO_OF_TILES_X + x].srcRect = (Rectangle) {blankTile * TILE_WIDTH,blankTile * TILE_HEIGHT,TILE_WIDTH,TILE_HEIGHT};
            }
        }
    }
    // Load map
    if (IsKeyPressed(KEY_L)) {
        printf("Load map...\n");
        LoadMap(game);
    }
    // Save map
    if (IsKeyPressed(KEY_PERIOD)) {
        printf("Save map...\n");
        SaveMap(game);
    }
    // Set tile modes
    if (IsKeyPressed(KEY_H)) {
        game->fillMode = FILL_HORIZONTAL;
    }
    if (IsKeyPressed(KEY_V)) {
        game->fillMode = FILL_VERTICAL;
    }
    if (IsKeyPressed(KEY_O)) {
        game->fillMode = FILL_OFF;
    }

    switch (game->cameraType) {
        case CAMERA_TILESET:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                printf("%d, %d\n", GetMouseX() / TILE_WIDTH, GetMouseY() / TILE_HEIGHT);
                game->tileSelected.x = GetMouseX() / TILE_WIDTH;
                game->tileSelected.y = GetMouseY() / TILE_WIDTH;
            }
            break;
        case CAMERA_WORLD:
            // if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            //     printf("down\n");
            // }
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), game->worldCamera);
                int x = (int) (mouse.x / TILE_WIDTH);
                int y = (int) (mouse.y / TILE_HEIGHT);
                if (x < 0 || x > NO_OF_TILES_X) return;
                if (y < 0 || y > NO_OF_TILES_Y) return;
                Tile *tile = &game->tiles[y * NO_OF_TILES_X + x];
                // printf("Touching tile id %d\n", tile->id);
                if (!game->overwriteTiles) {
                    if (tile->id != TILE_EMPTY) return;
                }
                printf("draw %d,%d\n", x, y);

                switch (game->fillMode) {
                    case FILL_OFF:
                        tile->srcRect.x = game->tileSelected.x * TILE_WIDTH;
                        tile->srcRect.y = game->tileSelected.y * TILE_HEIGHT;
                        tile->id = TileIndexToId(tile->srcRect.x / TILE_WIDTH, tile->srcRect.y / TILE_HEIGHT);
                        break;
                    case FILL_EMPTY:
                        // Should find contained tiles only
                        break;
                        case FILL_ALL:
                        // Should find contained tiles only
                        break;
                    case FILL_HORIZONTAL:
                        for (int i = x + 1; i < NO_OF_TILES_X; ++i) {
                            Tile *tile = &game->tiles[y * NO_OF_TILES_X + i];
                            if (tile->id > TILE_EMPTY) break;
                            tile->srcRect.x = game->tileSelected.x * TILE_WIDTH;
                            tile->srcRect.y = game->tileSelected.y * TILE_HEIGHT;
                            tile->id = TileIndexToId(tile->srcRect.x / TILE_WIDTH, tile->srcRect.y / TILE_HEIGHT);
                        }
                        for (int i = x; i >= 0; --i) {
                            Tile *tile = &game->tiles[y * NO_OF_TILES_X + i];
                            if (tile->id > TILE_EMPTY) break;
                            tile->srcRect.x = game->tileSelected.x * TILE_WIDTH;
                            tile->srcRect.y = game->tileSelected.y * TILE_HEIGHT;
                            tile->id = TileIndexToId(tile->srcRect.x / TILE_WIDTH, tile->srcRect.y / TILE_HEIGHT);
                        }
                        break;
                    case FILL_VERTICAL:
                        // Should it stop when it finds a non empty tile?
                        for (int i = y + 1; i < NO_OF_TILES_Y; ++i) {
                            Tile *tile = &game->tiles[i * NO_OF_TILES_X + x];
                            if (tile->id > TILE_EMPTY) break;
                            tile->srcRect.x = game->tileSelected.x * TILE_WIDTH;
                            tile->srcRect.y = game->tileSelected.y * TILE_HEIGHT;
                            tile->id = TileIndexToId(tile->srcRect.x / TILE_WIDTH, tile->srcRect.y / TILE_HEIGHT);
                        }
                        for (int i = y; i >= 0; --i) {
                            Tile *tile = &game->tiles[i * NO_OF_TILES_X + x];
                            if (tile->id > TILE_EMPTY) break;
                            tile->srcRect.x = game->tileSelected.x * TILE_WIDTH;
                            tile->srcRect.y = game->tileSelected.y * TILE_HEIGHT;
                            tile->id = TileIndexToId(tile->srcRect.x / TILE_WIDTH, tile->srcRect.y / TILE_HEIGHT);
                        }
                        break;
                }
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), game->worldCamera);
                int x = (int) (mouse.x / TILE_WIDTH);
                int y = (int) (mouse.y / TILE_HEIGHT);
                if (x < 0 || x > NO_OF_TILES_X) return;
                if (y < 0 || y > NO_OF_TILES_Y) return;

                // Store the source rect pointer
                Tile *tile = &game->tiles[y * NO_OF_TILES_X + x];
                // if (tile->id == TILE_EMPTY) return;
                printf("erase %d,%d\n", x, y);

                // Set the empty tile
                tile->srcRect.x = blankTile * TILE_WIDTH;
                tile->srcRect.y = blankTile * TILE_HEIGHT;
                tile->id = TILE_EMPTY;
            }
            // if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            //     Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), game->worldCamera);
            //     int x = (int) (mouse.x / TILE_WIDTH);
            //     int y = (int) (mouse.y / TILE_HEIGHT);
            //     if (x < 0 || x > NO_OF_TILES_X) return;
            //     if (y < 0 || y > NO_OF_TILES_Y) return;

            //     Tile *tile = &game->tiles[y * NO_OF_TILES_X + x];
            //     printf("Touching tile id %d\n", tile->id);
            //     if (tile->id != TILE_EMPTY) return;
            //     printf("%d,%d\n", x, y);
            //     printf("Fill\n");

            //     switch (game->fillMode) {
            //         case FILL_OFF:
            //             // Store the source rect pointer
            //             Rectangle *rect = &game->tiles[y * NO_OF_TILES_X + x].srcRect;
            //             // Set the selected tile
            //             rect->x = game->tileSelected.x * TILE_WIDTH;
            //             rect->y = game->tileSelected.y * TILE_HEIGHT;
            //             break;
            //         case FILL_EMPTY:
            //             // Should find contained tiles only
            //             break;
            //             case FILL_ALL:
            //             // Should find contained tiles only
            //             break;
            //         case FILL_HORIZONTAL:
            //             for (int i = x + 1; i < NO_OF_TILES_X; ++i) {
            //                 Tile *tile = &game->tiles[y * NO_OF_TILES_X + i];
            //                 if (tile->id > TILE_EMPTY) break;
            //                 tile->srcRect.x = game->tileSelected.x * TILE_WIDTH;
            //                 tile->srcRect.y = game->tileSelected.y * TILE_HEIGHT;
            //                 tile->id = TileIndexToId(tile->srcRect.x / TILE_WIDTH, tile->srcRect.y / TILE_HEIGHT);
            //             }
            //             for (int i = x; i >= 0; --i) {
            //                 Tile *tile = &game->tiles[y * NO_OF_TILES_X + i];
            //                 if (tile->id > 0) break;
            //                 tile->srcRect.x = game->tileSelected.x * TILE_WIDTH;
            //                 tile->srcRect.y = game->tileSelected.y * TILE_HEIGHT;
            //                 tile->id = TileIndexToId(tile->srcRect.x / TILE_WIDTH, tile->srcRect.y / TILE_HEIGHT);
            //             }
            //             break;
            //         case FILL_VERTICAL:
            //             // Should it stop when it finds a non empty tile?
            //             for (int i = y + 1; i < NO_OF_TILES_Y; ++i) {
            //                 Tile *tile = &game->tiles[i * NO_OF_TILES_X + x];
            //                 // Rectangle *rect = &game->tiles[i * NO_OF_TILES_X + x].srcRect;
            //                 if (tile->id > TILE_EMPTY) break;
            //                 tile->srcRect.x = game->tileSelected.x * TILE_WIDTH;
            //                 tile->srcRect.y = game->tileSelected.y * TILE_HEIGHT;
            //                 tile->id = TileIndexToId(tile->srcRect.x / TILE_WIDTH, tile->srcRect.y / TILE_HEIGHT);
            //             }
            //             for (int i = y; i >= 0; --i) {
            //                 Tile *tile = &game->tiles[i * NO_OF_TILES_X + x];
            //                 // Rectangle *rect = &game->tiles[i * NO_OF_TILES_X + x].srcRect;
            //                 if (tile->id > TILE_EMPTY) break;
            //                 tile->srcRect.x = game->tileSelected.x * TILE_WIDTH;
            //                 tile->srcRect.y = game->tileSelected.y * TILE_HEIGHT;
            //                 tile->id = TileIndexToId(tile->srcRect.x / TILE_WIDTH, tile->srcRect.y / TILE_HEIGHT);
            //             }
            //             break;
            //     }
            // }
            break;
        case CAMERA_SCREEN:
            break;
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

    if (game->cameraType == CAMERA_SCREEN || game->cameraType == CAMERA_SCREEN_SHAKE) {
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

        // Camera follow player
        // game->screenCamera.target.x = game->player.x;
        // game->screenCamera.target.y = game->player.y;
        // Camera follow player (smooth)
        game->screenCamera.target.x += (game->player.x - game->screenCamera.target.x) * .1;
        game->screenCamera.target.y += (game->player.y - game->screenCamera.target.y) * .1;

        // Center blue bounds around player
        game->screenBounds.x = game->player.x - SCREEN_WIDTH / 2;
        game->screenBounds.y = game->player.y - SCREEN_HEIGHT / 2;
        if (game->screenBounds.x < game->worldBounds.x) game->screenBounds.x = game->worldBounds.x;
        if (game->screenBounds.x + game->screenBounds.width > game->worldBounds.width) game->screenBounds.x = game->worldBounds.width - game->screenBounds.width;
        if (game->screenBounds.y < game->worldBounds.y) game->screenBounds.y = game->worldBounds.y;
        if (game->screenBounds.y + game->screenBounds.height > game->worldBounds.height) game->screenBounds.y = game->worldBounds.height - game->screenBounds.height;

        // Bound screen camera to game world
        float camX = game->screenCamera.offset.x / game->screenCamera.zoom;
        float camY = game->screenCamera.offset.y / game->screenCamera.zoom;
        if (game->screenCamera.target.x < camX ) game->screenCamera.target.x = camX;
        if (game->screenCamera.target.y < camY ) game->screenCamera.target.y = camY;
        if (game->screenCamera.target.x > WORLD_WIDTH - camX) game->screenCamera.target.x = WORLD_WIDTH - camX;
        if (game->screenCamera.target.y > WORLD_HEIGHT - camY) game->screenCamera.target.y = WORLD_HEIGHT - camY;
    }

    if (game->cameraType == CAMERA_SCREEN_SHAKE) {

        if (game->screenShake.shake > 0) {
            game->screenShake.shake -= 0.01;

            // Update shake
            game->screenShake.angle = game->screenShake.shake * GetRandomValueFloat(-1, 1);
            game->screenShake.offsetX = game->screenShake.maxOffset * game->screenShake.shake * GetRandomValueFloat(-1, 1);
            game->screenShake.offsetY = game->screenShake.maxOffset * game->screenShake.shake * GetRandomValueFloat(-1, 1);

            // Apply shake
            game->shakyCamera.rotation = game->screenCamera.rotation + game->screenShake.angle;
            game->shakyCamera.target =  Vector2Add(game->screenCamera.target,  (Vector2){game->screenShake.offsetX, game->screenShake.offsetX});

        } else {
            game->screenShake.shake = 0;
            game->cameraType = CAMERA_SCREEN;
        }
    }

    // Bound world camera
    // if (worldCamera.target.x < 0) worldCamera.target.x = 0;
    // if (worldCamera.target.y < 0) worldCamera.target.y = 0;
    // if (worldCamera.target.x > WORLD_WIDTH) worldCamera.target.x = WORLD_WIDTH;
    // if (worldCamera.target.y > WORLD_HEIGHT) worldCamera.target.y = WORLD_HEIGHT;

}

void Draw(Game *game) {
    BeginDrawing();
        ClearBackground(BLACK);

        DrawTexture(game->checkered, 0, 0, WHITE);

        if (game->cameraType == CAMERA_TILESET) {
            DrawTexture(game->tileset, 0, 0, WHITE);
            DrawRectangleLinesEx((Rectangle) {game->tileSelected.x * TILE_WIDTH, game->tileSelected.y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT}, 4 ,ColorAlpha(RED, 1.0f));
        } else if (game->cameraType == CAMERA_WORLD) {
            DrawCameraWorld(game);
        } else if (game->cameraType == CAMERA_SCREEN) {
            DrawCameraScreen(game);
        } else if (game->cameraType == CAMERA_SCREEN_SHAKE) {
            // DrawCameraScreen(game);
            DrawCameraScreenShake(game);
        }

        DrawGUI(game);

        DrawFPS(WORLD_WIDTH - MeasureText("60 FPS", 20) - 20, 20);
    EndDrawing();
}

int main(void) {

    srand((unsigned int)time(NULL));

    InitWindow(WORLD_WIDTH, WORLD_HEIGHT, "Tile Map Editor");

    Texture2D checkered = CreateCheckeredBackground();
    Texture2D tileset = LoadTexture("resources/tilesets/world_tileset.png"); // 16 x 16 tileset
    tileset.width *= 2;
    tileset.height *= 2;

    // World
    // Rectangle *tiles = malloc(sizeof(Rectangle) * NO_OF_TILES_X * NO_OF_TILES_Y);
    Tile tiles[NO_OF_TILES_X * NO_OF_TILES_Y];
    Rectangle tile = { 0, 0, TILE_WIDTH, TILE_HEIGHT };
    int lineThick = 1;

    int i = 0;
    for (int y = 0; y < NO_OF_TILES_Y; ++y) {
        for (int x = 0; x < NO_OF_TILES_X; ++x) {
            // y * NO_OF_TILES_Y + x
            // printf("%d - %d\n", i, y * NO_OF_TILES_Y + x);
            tiles[i].id = TILE_EMPTY;
            tiles[i].destRect = (Rectangle) {
                x * TILE_WIDTH, y * TILE_HEIGHT,
                TILE_WIDTH, TILE_HEIGHT};
            tiles[i].srcRect = (Rectangle) {blankTile * TILE_WIDTH,blankTile*TILE_HEIGHT,TILE_WIDTH,TILE_HEIGHT};

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
    // screenCamera.target = (Vector2){ player.x, player.y };
    screenCamera.rotation = 0.0f;
    screenCamera.zoom = 1.5f;

    Camera2D shakyCamera = { 0 };
    shakyCamera.offset = (Vector2){ WORLD_WIDTH/2.0f, WORLD_HEIGHT/2.0f };
    shakyCamera.rotation = 0.0f;
    shakyCamera.zoom = 1.5f;

    Rectangle worldBounds = { 0, 0, WORLD_WIDTH, WORLD_HEIGHT };
    Rectangle screenBounds = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    ScreenShake screenShake = {
        .shake = 0.0f,
        .maxAngle = 10.0f,
        .maxOffset = 10.0f,
        // Not sure this works, might have to do it after
        // .angle = screenShake.shake * GetRandomValueFloat(-1, 1),
        // .offsetX = screenShake.maxOffset * screenShake.shake * GetRandomValueFloat(-1, 1),
        // .offsetY = screenShake.maxOffset * screenShake.shake * GetRandomValueFloat(-1, 1),
    };
    screenShake.angle = screenShake.shake * GetRandomValueFloat(-1, 1);
    screenShake.offsetX = screenShake.maxOffset * screenShake.shake * GetRandomValueFloat(-1, 1);
    screenShake.offsetY = screenShake.maxOffset * screenShake.shake * GetRandomValueFloat(-1, 1);

    int cameraType = CAMERA_SCREEN;
    int fillMode = FILL_OFF;
    // int fillMode = FILL_HORIZONTAL;
    // int fillMode = FILL_VERTICAL;

    // GUI
    Rectangle guiWindow = { 0, 0, 300, WORLD_HEIGHT };
    BoolFlags boolFlags = {
        .showGUI = false,
        .showGUIwindow = true,
        .showWindowBorder = false,
        .showScreenBorder = false
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
    game.screenCamera = screenCamera;
    game.shakyCamera = shakyCamera;
    game.checkered = checkered;
    game.tileset = tileset;
    game.tileSelected = (Vector2) { 0, 0};
    game.tiles = tiles;
    game.fillMode = fillMode;
    game.overwriteTiles = false;
    game.screenShake = screenShake;

    LoadMap(&game);

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
