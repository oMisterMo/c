/**
 * Functional Requirements:
 *  - Set background tiles in UI
 *  - Render background tiles
 *  - Show ghost tile (preview) ✔️
 *  - Show toast when user saves/loads/resets file ✔️
 *  - Player spawn location (ID = 'p')
 *  - Middle click to drag
 *  - Scroll to zoom
 *  - Fullscreen ✔️
 *  - Select multiple tiles
 *  - Draw multiple tiles
 *  - Add RayGUI
 *      - Fill horizonal tiles ✔️
 *      - Fill vertical tiles ✔️
 *      - Select layer to add tiles
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
 *  - Particles
 *      - Rain animation
 *      - Snow animation
 *  - Screen shake ✔️
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
#include <time.h>           // Required for: time()
#include "raylib.h"
#include "raymath.h"
#include "reasings.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "stb_perlin.h"

#define NO_OF_PARTICLES 1 << 6
// #define NO_OF_PARTICLES 1
#define WORLD_WIDTH 1280
#define WORLD_HEIGHT 720

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 270

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#define NO_OF_TILES_X WORLD_WIDTH / TILE_WIDTH      // 40
#define NO_OF_TILES_Y WORLD_HEIGHT / TILE_HEIGHT    // 22.5

int WINDOW_WIDTH  = WORLD_WIDTH;
int WINDOW_HEIGHT  = WORLD_HEIGHT;

int blankTile = 3;  // Blank tile at index 3, could use any random number
int guiX = 260; // 300 - 40
int guiY = 40;  // 40
int guiWidth = 300;

typedef enum CameraType {
    CAMERA_TILESET = 0,
    CAMERA_WORLD,
    CAMERA_SCREEN,
    CAMERA_SCREEN_SHAKE,
} CameraType;

typedef enum TileType {
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
} TileType;

typedef enum FillMode {
    FILL_OFF = 0,   // TODO: Rename to FILL_STAMP
    FILL_EMPTY,     // TODO: Remove
    FILL_ALL,       // TODO: Use BFS/Dijkstra algorithm
    FILL_HORIZONTAL,
    FILL_VERTICAL
} FillMode;

typedef enum TweenState {
    IDLE = 0,
    TWEENING
} TweenState;

typedef struct Tween {
    int state;                  // IDLE | TWEENING
    int frameCounter;           // Current time in tween
    Vector2 currentPosition;    // Tween start
    Vector2 targetPosition;     // Tween end [could be consts]
    int duration;               // How long to tween in frames e.g 30 frames = 500ms, 60 = 1sec
} Tween;

typedef struct Particle {
    unsigned int id;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    float size;

    float scale;
    float scaleStart;
    float scaleEnd;
    float scaleVelocity;
    // float rotation;

    float alpha;
    float alphaStart;
    float alphaEnd;
    float alphaVelocity;

    Color color;
    Color colorStart;
    Color colorEnd;

    bool dead;
    float ageSeconds;
    float ageStartSeconds;
    float currentTimeOffsetX;   // For perlin noise
    float currentTimeOffsetY;   // For perlin noise
    // Texture2D img;
} Particle;

typedef struct BoolFlags {
    bool showGUI;
    bool showGUIwindow;
    bool showWindowBorder;
    bool showScreenBorder;
    bool showAxis;
    bool showParticles;
} BoolFlags;

typedef struct GUIFlags {
    bool dropdownActive;
    int dropdownState;
} GUIFlags;

typedef struct ScreenShake {
    float shake;
    float maxAngle;
    float maxOffset;
    float angle;
    float offsetX;
    float offsetY;
} ScreenShake;

typedef struct Tile {
    unsigned int id;    // what tile (grass, water): 4 bytes
    Rectangle srcRect;  // spritesheet index: 16 bytes
    Rectangle destRect; // position: 16 bytes
} Tile;

typedef struct Toast {
    char *message;
    Rectangle bounds;
    Vector2 position;
    Tween tween;
} Toast;

typedef struct Game {
    Rectangle player;
    Rectangle worldBounds;
    Rectangle screenBounds;

    BoolFlags boolFlags;
    GUIFlags guiFlags;

    CameraType cameraType;
    Camera2D worldCamera;
    Camera2D screenCamera;
    Camera2D shakyCamera;
    ScreenShake screenShake;

    Texture2D checkered;
    Texture2D tileset;
    Texture2D img;

    Vector2 tileSelected;
    Tile *tiles;
    Tile *backgroundTiles;
    FillMode fillMode;
    bool overwriteTiles;

    Toast toast;
    Particle *particles;
} Game;

int GetWidth() {
    if (IsWindowFullscreen()) {
        // printf("%d\n", WINDOW_WIDTH);
        return WINDOW_WIDTH;
    }
    // printf("%d\n", WORLD_WIDTH);
    return WORLD_WIDTH;
}

int GetHeight() {
    if (IsWindowFullscreen()) {
        return WINDOW_HEIGHT;
    }
    return WORLD_HEIGHT;
}

// =========== Particles ===========

int GenerateRandomId() {
    static int id = 0;
    return id++;
}

float GetRandomValueFloat(float min, float max) {
    // Use perlin noise, not random values
    // https://www.youtube.com/watch?v=tu-Qe66AvtY
    return (((float)rand()/(float)(RAND_MAX)) * abs(max - min)) + min;
    // return (((float)rand()/(float)(RAND_MAX)) * 2) - 1; // between -1 and 1
}

float GetPerlin(float time) {
    float p = stb_perlin_noise3(time / 12 , 0.0f, 0.0f, 0, 0, 0);
    // float p = stb_perlin_fbm_noise3(currentTime, y, z, 2.0f, 0.5f, 1);

    // Clamp between -1.0f and 1.0f
    if (p < -1.0f) p = -1.0f;
    if (p > 1.0f) p = 1.0f;

    // We need to normalize the data from [-1..1] to [0..1]
    float np = (p + 1.0f) / 2.0f;

    return np;
}

float GetPerlinPro(float x, float y, float z) {
    float p = stb_perlin_noise3(x / 12 , y, z, 0, 0, 0);
    // float p = stb_perlin_fbm_noise3(currentTime, y, z, 2.0f, 0.5f, 1);

    // Clamp between -1.0f and 1.0f
    if (p < -1.0f) p = -1.0f;
    if (p > 1.0f) p = 1.0f;

    // We need to normalize the data from [-1..1] to [0..1]
    float np = (p + 1.0f) / 2.0f;

    return np;
}

void UpdateParticlePerlin(Particle *p) {
    float currentTime = GetTime();
    // printf("currentTime: %.2f\n", currentTime);

    // GetPerlin() returns a random value between [0 - 1]
    // I then add a random offset to avoid the same movement
    float x = GetPerlinPro(currentTime + p->currentTimeOffsetX / 12, p->id * 7.23f, 0.0f);
    float y = GetPerlinPro(currentTime + p->currentTimeOffsetY / 12, 0.0f, p->id * 3.71f);

    // Fill the screen
    p->position.x = (x *  GetWidth() * 2) - GetWidth() / 2;
    p->position.y = (y * GetHeight() * 2) - GetWidth() / 2;
    // p->velocity.x = x * GetWidth();
    // p->velocity.y = y * GetHeight();
    // printf("perlin (x,y):\t %.2f, %.2f\n", x, y);
}

void SetPaticleFall(Particle *p) {
    float speed = 50;

    // PERLIN NOISE STUFF
    p->position.x = 0;
    p->position.y = 0;
    p->currentTimeOffsetX = GetRandomValueFloat(10000, 500000);
    p->currentTimeOffsetY = GetRandomValueFloat(500000, 100000);

    // REGULAR
    // position
    p->position.x = GetRandomValue(0, GetWidth());
    p->position.y = GetRandomValue(0, GetHeight());
    p->velocity.x = GetRandomValueFloat(-100, 100);
    p->velocity.y = GetRandomValueFloat(50, speed);
    p->acceleration.x = 0;
    p->acceleration.y = 0;

    p->size = GetRandomValue(3, 4);

    // p->velocity.x = GetRandomValueFloat(50, speed);
    // p->velocity.y = GetRandomValueFloat(50, speed);

    // scale
    p->scaleStart = 1.0f;
    p->scaleEnd = 0.0f;
    p->scale = p->scaleStart;
    p->scaleVelocity = GetRandomValueFloat(-0.1, -1); // do I still need this?

    // printf("scale %f\n", p->scale);
    // printf("scale vel %f\n", p->scaleVelocity);

    // alpha
    p->alphaStart = 0.0f;
    p->alphaEnd = 0.0f;
    p->alpha = p->alphaStart;
    p->alphaVelocity = GetRandomValueFloat(-0.1, -1);   // do I still need this?

    // color
    p->colorStart = WHITE;
    p->colorEnd = BROWN;
    p->color = p->colorStart;


    p->dead = false;
    p->ageSeconds = GetRandomValueFloat(1, 15);
    p->ageStartSeconds = p->ageSeconds;
}

void CreateSimpleParticleEffect(Particle *particles) {
    for (int i = 0; i < NO_OF_PARTICLES; ++i) {
        Particle *p = &particles[i];
        SetPaticleFall(p);
    }
}

void UpdateParticles(Game *game) {
    if (game->boolFlags.showParticles) {
        Particle *particles = game->particles;
        float dt = GetFrameTime();
        for (int i = 0; i < NO_OF_PARTICLES; ++i) {
            Particle *p = &particles[i];
            if (p->dead) continue;
            if (p->ageSeconds <= 0) {
                // printf("Age is over...\n");
                p->dead = true;
                p->ageSeconds = 0;
                SetPaticleFall(p);
                continue;
            }
            p->ageSeconds -= dt;

            // Update position
            p->velocity.x += p->acceleration.x * dt;
            p->velocity.y += p->acceleration.y * dt;
            p->position.x += p->velocity.x * dt;
            p->position.y += p->velocity.y * dt;
            // p->position.x += 0.2;
            // p->position.y += 0.2;

            // If I want to mess around with it, not got it moving nicely
            // UpdateParticlePerlin(p);


            // Update rotation

            // Update scale
            float d = p->ageSeconds / p->ageStartSeconds;
            float t = 1.0f - d; // progresses from 0 to 1 over the lifetime

            // p->scale += p->scaleVelocity * dt;
            // p->scale = Clamp(p->scale, 0, 30);
            p->scale = p->scaleStart * d;

            // Update alpha
            // 1 - mo
            // p->alpha += p->alphaVelocity * dt;
            // p->alpha = Clamp(p->alpha, 0, 1);

            // 2 - creates a triangle fade: 0 → 1 → 0
            p->alpha = 1.0f - fabsf(2.0f * t - 1.0f);

            // 3 - prettier easing: smooth in and out
            // p->alpha = sinf(t * PI);

            // 4 - sharper fade
            // p->alpha = powf(1.0f - fabsf(2.0f * t - 1.0f), 2.0f);

            // Update color
            p->color = ColorLerp(p->colorStart, p->colorEnd, d);

            // printf("t: %.2f\n", t);
            // printf("ageSeconds %.2f\n", p->ageSeconds);
            // printf("scale %.2f\n", p->scale);
            // printf("alpha %.2f\n", p->alpha);
            // printf("---------------\n");
        }
    }
}

void DrawParticles(Game *game) {
    if (game->boolFlags.showParticles) {
        Particle *particles = game->particles;
        for (int i = 0; i < NO_OF_PARTICLES; ++i) {
            Particle p = particles[i];
            if (p.dead) continue;
            if (p.ageSeconds <= 0) continue;

            DrawRectangleRec((Rectangle){p.position.x,p.position.y,p.size*p.scale,p.size*p.scale},ColorAlpha(p.color, p.alpha));
            // DrawTexturePro(game->img, (Rectangle){0,0,game->img.width,game->img.height}, (Rectangle){p.position.x, p.position.y, game->img.width * p.scale,game->img.height * p.scale}, (Vector2){0}, 0, ColorAlpha(p.color, p.alpha));
            // printf("%.2f,%.2f\n", p.position.x, p.position.y);
        }
    }
}

// =========== Screen shake ===========

void ApplyShake(Game *game, float trauma) {
    printf("Shake...\n");
    game->cameraType = CAMERA_SCREEN_SHAKE;
    game->screenShake.shake += trauma;
    if (game->screenShake.shake > 1.0f) game->screenShake.shake = 1.0f;
}

void UpdateScreenShake(Game *game) {
    if (game->screenShake.shake > 0) {
        // TODO: FRAME RATE INDEPENDENT
        game->screenShake.shake -= 0.01;

        // Update shake
        game->screenShake.angle = game->screenShake.shake * GetRandomValueFloat(-1, 1);
        game->screenShake.offsetX = game->screenShake.maxOffset * game->screenShake.shake * GetRandomValueFloat(-1, 1);
        game->screenShake.offsetY = game->screenShake.maxOffset * game->screenShake.shake * GetRandomValueFloat(-1, 1);

        // Just rotation
        // game->shakyCamera.rotation = game->screenCamera.rotation + game->screenShake.angle;
        // game->shakyCamera.target = game->screenCamera.target;

        // Just translation
        // game->shakyCamera.target = Vector2Add(game->screenCamera.target,  (Vector2){game->screenShake.offsetX, game->screenShake.offsetX});

        // Apply rotation and translation
        game->shakyCamera.rotation = game->screenCamera.rotation + game->screenShake.angle;
        game->shakyCamera.target = Vector2Add(game->screenCamera.target,  (Vector2){game->screenShake.offsetX, game->screenShake.offsetX});

    } else {
        game->screenShake.shake = 0;
        game->cameraType = CAMERA_SCREEN;
    }
}

// =========== Toast ===========

void TweenToast(Toast *toast) {
    if (toast->tween.state == TWEENING) {
        // printf("twwenz %d\n", toast->tween.frameCounter);
        toast->tween.frameCounter++;

        // Tween
        toast->position.y  = EaseQuadOut(
            (float) toast->tween.frameCounter,
            toast->tween.currentPosition.y,
            toast->tween.targetPosition.y - toast->tween.currentPosition.y,
            toast->tween.duration
        );

        // Tween complete
        if (toast->tween.frameCounter >= toast->tween.duration) {
            printf("complete\n");
            toast->tween.frameCounter = 0;
            toast->tween.state = IDLE;

            // Set final position
            toast->position.y = toast->tween.currentPosition.y;
        }
    }
}

void PlayToastTween(Toast *toast, char *message) {
    toast->message = message;
    toast->tween.currentPosition.x = (GetWidth() - MeasureText(toast->message, 20)) / 2;
    toast->tween.currentPosition.y = GetHeight() + 50;
    toast->tween.targetPosition.x = (GetWidth() - MeasureText(toast->message, 20)) / 2;
    toast->tween.targetPosition.y = GetHeight() - 50;
    toast->position.x = (GetWidth() - MeasureText(toast->message, 20)) / 2;
    toast->position.y = GetHeight() + 50;
    toast->tween.state = TWEENING;
    toast->tween.frameCounter = 0;
}

// =========== Utils ===========

Texture2D CreateCheckeredBackground() {
    // Checkered background
    int w = GetMonitorWidth(2);
    int h = GetMonitorHeight(2);
    printf("w %d\n", w);
    Color *pixels = (Color *)malloc(w*h*sizeof(Color));

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (((x/32+y/32)/1)%2 == 0) pixels[y*w + x] = ColorAlpha(ORANGE, 0.1f);
            else pixels[y*w + x] = ColorAlpha(GOLD, 0.1f);
        }
    }

    // Load pixels data into an image structure and create texture
    Image checkedIm = {
        .data = pixels,             // We can assign pixels directly to data
        .width = w,
        .height = h,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .mipmaps = 1
    };

    Texture2D checkered = LoadTextureFromImage(checkedIm);
    UnloadImage(checkedIm);     // free img.data aka pixels
    return checkered;
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

// =========== Game ===========

void DrawAxis() {
    // DrawLine(-1000, 0, 1000, 0, WHITE);
    // DrawLine(0, -1000, 0, 1000, WHITE);
    DrawLineEx((Vector2){-5000,0}, (Vector2){5000,0}, 4, ColorAlpha(WHITE, 0.5f));
    DrawLineEx((Vector2){0,-5000}, (Vector2){0,5000}, 4, ColorAlpha(WHITE, 0.5f));
}

void DrawGUIWorld(Game *game) {
    Vector2 windowStart = GetScreenToWorld2D((Vector2){0,0}, game->worldCamera);
    Vector2 windowEnd = GetScreenToWorld2D((Vector2){WORLD_WIDTH,WORLD_HEIGHT}, game->worldCamera);
    Vector2 screenStart = GetScreenToWorld2D((Vector2){0,0}, game->screenCamera);
    Vector2 screenEnd = GetScreenToWorld2D((Vector2){SCREEN_WIDTH,SCREEN_HEIGHT}, game->screenCamera);

    // UI
    if (game->boolFlags.showGUI) {
        if (game->boolFlags.showGUIwindow) {
            // Draw window
            DrawRectangleRec((Rectangle) { 0, 0, guiWidth, GetHeight() }, ColorAlpha(GRAY, 0.8f));
            DrawRectangleRec((Rectangle) { GetWidth() - guiWidth, 0, guiWidth, GetHeight() }, ColorAlpha(GRAY, 0.8f));
        }
        switch (game->cameraType) {
            case CAMERA_TILESET:
                DrawText("Camera Tileset", 20, GetHeight() - 40, 20, WHITE);
                break;
            case CAMERA_WORLD:
                DrawText("Camera World", 20, GetHeight() - 40, 20, WHITE);
                break;
            case CAMERA_SCREEN:
                DrawText("Camera Screen", 20, GetHeight() - 40, 20, WHITE);
                break;
            case CAMERA_SCREEN_SHAKE:
                DrawText("Camera Shake", 20, GetHeight() - 40, 20, WHITE);
                break;
        }

        // LEFT

        // Tiles on screen
        DrawText(TextFormat("x (%d,%d)", (int) windowStart.x / TILE_WIDTH, (int)(windowEnd.x / TILE_WIDTH) + 1), 20, 60, 20, WHITE);
        DrawText(TextFormat("y (%d,%d)", (int) windowStart.y / TILE_HEIGHT, (int)(windowEnd.y / TILE_HEIGHT) + 1), 20, 100, 20, WHITE);

        // Camera position
        DrawText(TextFormat("player (%d,%d)", (int) game->player.x, (int)(game->player.y)), 20, 160, 20, WHITE);
        DrawText(TextFormat("world  (%d,%d) x%.2f", (int) game->worldCamera.target.x, (int)(game->worldCamera.target.y), game->worldCamera.zoom), 20, 200, 20, WHITE);
        DrawText(TextFormat("screen (%d,%d) x%.2f", (int) game->screenCamera.target.x, (int)(game->screenCamera.target.y), game->screenCamera.zoom), 20, 240, 20, WHITE);

        DrawRectangle(20, 265, 240, 10, ColorAlpha(BLACK, 0.5f));

        // ScreenToWorld2D
        // DrawText(TextFormat("w start (%d,%d)", (int) windowStart.x , (int)(windowStart.y)), 20, 280, 20, WHITE);
        // DrawText(TextFormat("w end (%d,%d)", (int) windowEnd.x , (int)(windowEnd.y)), 20, 320, 20, WHITE);
        // DrawText(TextFormat("s start (%d,%d)", (int) screenStart.x , (int)(screenStart.y)), 20, 360, 20, WHITE);
        // DrawText(TextFormat("s end (%d,%d)", (int) screenEnd.x , (int)(screenEnd.y)), 20, 400, 20, WHITE);

        // Screenshake
        DrawText(TextFormat("shake (%.2f)",  game->screenShake.shake), 20, 440, 20, WHITE);

        // Tile info
        DrawText(TextFormat("No x tiles %d", NO_OF_TILES_X), 20, 480, 20, WHITE);
        DrawText(TextFormat("No y tiles %d", NO_OF_TILES_Y), 20, 520, 20, WHITE);

        // RIGHT
        GuiCheckBox((Rectangle){ GetWidth() - guiX, guiY, 20, 20 }, "Close", &game->boolFlags.showGUI);
        GuiCheckBox((Rectangle){ GetWidth() - guiX, guiY + (1 * 40), 20, 20 }, "Background", &game->boolFlags.showGUIwindow);
        GuiCheckBox((Rectangle){ GetWidth() - guiX, guiY + (2 * 40), 20, 20}, "Window border", &game->boolFlags.showWindowBorder);
        GuiCheckBox((Rectangle){ GetWidth() - guiX, guiY + (3 * 40), 20, 20}, "Screen border", &game->boolFlags.showScreenBorder);
        GuiCheckBox((Rectangle){ GetWidth() - guiX, guiY + (4 * 40), 20, 20}, "Axis", &game->boolFlags.showAxis);
        GuiCheckBox((Rectangle){ GetWidth() - guiX, guiY + (5 * 40), 20, 20}, "Particles", &game->boolFlags.showParticles);
        // GuiCheckBox((Rectangle){ GetWidth() - guiX, guiY + (6 * 40), 20, 20}, "Background Tile", &game->boolFlags.isBackgroundTile);
    }
    int pad = 20;
    int h = 40;
    DrawRectangleRounded((Rectangle) {game->toast.position.x - pad, game->toast.position.y - h / 4, MeasureText(game->toast.message, 20) + pad * 2, h}, 10, 4, ColorAlpha(BLACK, 1.0f));
    DrawText(TextFormat("%s",  game->toast.message), game->toast.position.x, game->toast.position.y, 20, WHITE);
}

void DrawGUITileset(Game *game) {
    Vector2 windowStart = GetScreenToWorld2D((Vector2){0,0}, game->worldCamera);
    Vector2 windowEnd = GetScreenToWorld2D((Vector2){WORLD_WIDTH,WORLD_HEIGHT}, game->worldCamera);
    Vector2 screenStart = GetScreenToWorld2D((Vector2){0,0}, game->screenCamera);
    Vector2 screenEnd = GetScreenToWorld2D((Vector2){SCREEN_WIDTH,SCREEN_HEIGHT}, game->screenCamera);
    // UI
    if (game->boolFlags.showGUI) {
        if (game->boolFlags.showGUIwindow) {
            // Draw window
            DrawRectangleRec((Rectangle) { GetWidth() - guiWidth, 0, guiWidth, GetHeight() }, ColorAlpha(GRAY, 0.8f));
        }
        switch (game->cameraType) {
            case CAMERA_TILESET:
                DrawText("Camera Tileset", 20, GetHeight() - 40, 20, WHITE);
                break;
            case CAMERA_WORLD:
                DrawText("Camera World", 20, GetHeight() - 40, 20, WHITE);
                break;
            case CAMERA_SCREEN:
                DrawText("Camera Screen", 20, GetHeight() - 40, 20, WHITE);
                break;
            case CAMERA_SCREEN_SHAKE:
                DrawText("Camera Shake", 20, GetHeight() - 40, 20, WHITE);
                break;
        }

        // LEFT

        // RIGHT
        GuiCheckBox((Rectangle){ GetWidth() - guiX, guiY, 20, 20 }, "Close", &game->boolFlags.showGUI);
        // GuiCheckBox((Rectangle){ GetWidth() - guiX, guiY + (1 * 40), 20, 20}, "Background Tile", &game->boolFlags.isBackgroundTile);
        GuiLabel((Rectangle){ GetWidth() - guiX, guiY + (1 * 40), MeasureText("Select Layer", 20), 20}, "Select Layer");
        if (GuiDropdownBox((Rectangle){ GetWidth() - guiX, guiY + (2 * 40), MeasureText("Select Layer", 20), 20}, "Foreground;Background", &game->guiFlags.dropdownState, game->guiFlags.dropdownActive)) {
            game->guiFlags.dropdownActive = !game->guiFlags.dropdownActive;
            /*
                dropdownActive = whether the box is open or not
                0 = do not show dropdown
                1 = show drop down

                dropdownState = the selected dropdown item
                0 = first item
                1 = second item
                ...
            */
        }
    }
}

void DrawGUI(Game *game) {
    switch(game->cameraType) {
        case CAMERA_TILESET:
            DrawGUITileset(game);
            break;
        default:
            DrawGUIWorld(game);
            break;
    }

}

void DrawCameraWorld(Game *game) {
    BeginMode2D(game->worldCamera);

    if (game->boolFlags.showAxis) {
        DrawAxis();
    }

    // Background tiles
    for (int y = 0; y < NO_OF_TILES_Y; ++y) {
        for (int x = 0; x < NO_OF_TILES_X; ++x) {
            Rectangle src = game->backgroundTiles[y * NO_OF_TILES_X + x].srcRect;
            Rectangle dest = game->backgroundTiles[y * NO_OF_TILES_X + x].destRect;
            DrawTexturePro(game->tileset,src,dest,(Vector2){0},0, WHITE);
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

    // Red tiles
    int i = 0;
    for (int y = 0; y < NO_OF_TILES_Y; ++y) {
        for (int x = 0; x < NO_OF_TILES_X; ++x) {
            // DrawRectangle(x * TILE_WIDTH, y * TILE_HEIGHT,
            //     TILE_WIDTH, TILE_HEIGHT, GRAY);
            DrawRectangleLines(x * TILE_WIDTH, y * TILE_HEIGHT,
                TILE_WIDTH, TILE_HEIGHT, ColorAlpha(WHITE, 0.25f));
            ++i;
        }
    }

    // TODO - LATER FIX THIS FOR BG TILES!!!

    // Draw Preview tiles
    Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), game->worldCamera);
    int x = (int) (mouse.x / TILE_WIDTH);
    int y = (int) (mouse.y / TILE_HEIGHT);
    // When the mouse is hovering on the tile map
    if (x >= 0 && x < NO_OF_TILES_X && y >= 0 && y < NO_OF_TILES_Y && !IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        // printf("%d,%d\n", x , y);
        switch (game->fillMode) {
            case FILL_OFF:
                    DrawTexturePro(game->tileset,
                        (Rectangle){game->tileSelected.x * TILE_WIDTH,game->tileSelected.y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT},
                        (Rectangle){x * TILE_WIDTH , y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT},
                        (Vector2){0},0, ColorAlpha(WHITE, 0.2f));
                break;
            case FILL_HORIZONTAL:
                // For all tiles
                // for (int i = 0; i < NO_OF_TILES_X; i++) {
                //     DrawTexturePro(game->tileset,
                //         (Rectangle){game->tileSelected.x * TILE_WIDTH,game->tileSelected.y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT},
                //         (Rectangle){i * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT},
                //         (Vector2){0},0, ColorAlpha(WHITE, 0.2f));
                // }
                // Break on found tile
                for (int i = x + 1; i < NO_OF_TILES_X; ++i) {
                    Tile *tile = &game->tiles[y * NO_OF_TILES_X + i];
                    if (tile->id > TILE_EMPTY) break;
                        DrawTexturePro(game->tileset,
                        (Rectangle){game->tileSelected.x * TILE_WIDTH,game->tileSelected.y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT},
                        (Rectangle){i * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT},
                        (Vector2){0},0, ColorAlpha(WHITE, 0.2f));
                }
                for (int i = x; i >= 0; --i) {
                    Tile *tile = &game->tiles[y * NO_OF_TILES_X + i];
                    if (tile->id > TILE_EMPTY) break;
                        DrawTexturePro(game->tileset,
                        (Rectangle){game->tileSelected.x * TILE_WIDTH,game->tileSelected.y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT},
                        (Rectangle){i * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT},
                        (Vector2){0},0, ColorAlpha(WHITE, 0.2f));
                }
                break;
            case FILL_VERTICAL:
                // For all tiles
                // for (int i = 0; i < NO_OF_TILES_Y; i++) {
                    // DrawTexturePro(game->tileset,
                    //     (Rectangle){game->tileSelected.x * TILE_WIDTH,game->tileSelected.y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT},
                    //     (Rectangle){x * TILE_WIDTH , i * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT},
                    //     (Vector2){0},0, ColorAlpha(WHITE, 0.2f));
                // }
                // Break on found tile
                for (int i = y + 1; i < NO_OF_TILES_Y; ++i) {
                    Tile *tile = &game->tiles[i * NO_OF_TILES_X + x];
                    if (tile->id > TILE_EMPTY) break;
                         DrawTexturePro(game->tileset,
                        (Rectangle){game->tileSelected.x * TILE_WIDTH,game->tileSelected.y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT},
                        (Rectangle){x * TILE_WIDTH , i * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT},
                        (Vector2){0},0, ColorAlpha(WHITE, 0.2f));
                }
                for (int i = y; i >= 0; --i) {
                    Tile *tile = &game->tiles[i * NO_OF_TILES_X + x];
                    if (tile->id > TILE_EMPTY) break;
                        DrawTexturePro(game->tileset,
                        (Rectangle){game->tileSelected.x * TILE_WIDTH,game->tileSelected.y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT},
                        (Rectangle){x * TILE_WIDTH , i * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT},
                        (Vector2){0},0, ColorAlpha(WHITE, 0.2f));
                }
                break;
        }
    }

    if (game->boolFlags.showWindowBorder) {
        DrawRectangleLinesEx(game->worldBounds, 8, YELLOW);
    }
    if (game->boolFlags.showScreenBorder) {
        DrawRectangleLinesEx(game->screenBounds, 8, BLUE);
    }

    // DrawRectangleRec(game->player, ColorAlpha(ORANGE, 0.2f));
    DrawParticles(game);
    EndMode2D();
}

void DrawCameraScreen(Game *game) {
    // TODO: this method is identical to DrawCamerWorld except for the line below, and the drawing of empty tiles.
    BeginMode2D(game->screenCamera);

    // DrawAxis();

    // Background tiles
    for (int y = 0; y < NO_OF_TILES_Y; ++y) {
        for (int x = 0; x < NO_OF_TILES_X; ++x) {
            Rectangle src = game->backgroundTiles[y * NO_OF_TILES_X + x].srcRect;
            Rectangle dest = game->backgroundTiles[y * NO_OF_TILES_X + x].destRect;
            DrawTexturePro(game->tileset,src,dest,(Vector2){0},0, WHITE);
        }
    }

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
    DrawParticles(game);
    EndMode2D();
}

void DrawCameraScreenShake(Game *game) {
    // TODO: this method is exactly identical to DrawCamerScreen except for the line below (at this moment).
    BeginMode2D(game->shakyCamera);

    // DrawAxis();

    // Background tiles
    for (int y = 0; y < NO_OF_TILES_Y; ++y) {
        for (int x = 0; x < NO_OF_TILES_X; ++x) {
            Rectangle src = game->backgroundTiles[y * NO_OF_TILES_X + x].srcRect;
            Rectangle dest = game->backgroundTiles[y * NO_OF_TILES_X + x].destRect;
            DrawTexturePro(game->tileset,src,dest,(Vector2){0},0, WHITE);
        }
    }

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
    DrawParticles(game);
    EndMode2D();
}

void OnMouseDownLeft(Game *game, Tile *tiles, int x, int y) {
        // printf("Touching tile id %d\n", tile->id);
        Tile *tile = &tiles[y * NO_OF_TILES_X + x];
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
                    Tile *t = &tiles[y * NO_OF_TILES_X + i];
                    if (t->id > TILE_EMPTY) break;
                    t->srcRect.x = game->tileSelected.x * TILE_WIDTH;
                    t->srcRect.y = game->tileSelected.y * TILE_HEIGHT;
                    t->id = TileIndexToId(t->srcRect.x / TILE_WIDTH, t->srcRect.y / TILE_HEIGHT);
                }
                for (int i = x; i >= 0; --i) {
                    Tile *t = &tiles[y * NO_OF_TILES_X + i];
                    if (t->id > TILE_EMPTY) break;
                    t->srcRect.x = game->tileSelected.x * TILE_WIDTH;
                    t->srcRect.y = game->tileSelected.y * TILE_HEIGHT;
                    t->id = TileIndexToId(t->srcRect.x / TILE_WIDTH, t->srcRect.y / TILE_HEIGHT);
                }
                break;
            case FILL_VERTICAL:
                // Should it stop when it finds a non empty tile?
                for (int i = y + 1; i < NO_OF_TILES_Y; ++i) {
                    Tile *t = &tiles[i * NO_OF_TILES_X + x];
                    if (t->id > TILE_EMPTY) break;
                    t->srcRect.x = game->tileSelected.x * TILE_WIDTH;
                    t->srcRect.y = game->tileSelected.y * TILE_HEIGHT;
                    t->id = TileIndexToId(t->srcRect.x / TILE_WIDTH, t->srcRect.y / TILE_HEIGHT);
                }
                for (int i = y; i >= 0; --i) {
                    Tile *t = &tiles[i * NO_OF_TILES_X + x];
                    if (t->id > TILE_EMPTY) break;
                    t->srcRect.x = game->tileSelected.x * TILE_WIDTH;
                    t->srcRect.y = game->tileSelected.y * TILE_HEIGHT;
                    t->id = TileIndexToId(t->srcRect.x / TILE_WIDTH, t->srcRect.y / TILE_HEIGHT);
                }
                break;
        }
}

void OnMouseDownRight(Tile *tiles, int x, int y) {
    Tile *tile = &tiles[y * NO_OF_TILES_X + x];
    // if (tile->id == TILE_EMPTY) return;
    printf("erase %d,%d\n", x, y);
    // Set the empty tile
    tile->srcRect.x = blankTile * TILE_WIDTH;
    tile->srcRect.y = blankTile * TILE_HEIGHT;
    tile->id = TILE_EMPTY;
}

void Input(Game *game) {
    // Switch camera tileset/world
    if (IsKeyPressed(KEY_SPACE)) {
        if (game->cameraType == CAMERA_WORLD) {
            game->cameraType = CAMERA_TILESET;
        } else {
            game->cameraType = CAMERA_WORLD;
        }
    }
    // Camera = tileset
    if (IsKeyPressed(KEY_ONE)) {
        game->cameraType = CAMERA_TILESET;
        // game->boolFlags.showGUI = !game->boolFlags.showGUI;
    }
    // Camera = world
    if (IsKeyPressed(KEY_TWO)) {
        game->cameraType = CAMERA_WORLD;
        // game->boolFlags.showScreenBorder = false;
        // game->boolFlags.showGUIwindow = !game->boolFlags.showGUIwindow;
    }
    // Camera = screen
    if (IsKeyPressed(KEY_THREE)) {
        game->cameraType = CAMERA_SCREEN;
        // game->boolFlags.showScreenBorder = true;
    }
    // Camera = screenshake
    if (IsKeyPressed(KEY_FOUR)) {
        ApplyShake(game, 0.3f);
    }
    // UI stuff
    if (IsKeyPressed(KEY_EIGHT)) {
        game->boolFlags.showWindowBorder = !game->boolFlags.showWindowBorder;
    }
    if (IsKeyPressed(KEY_NINE)) {
        game->boolFlags.showScreenBorder = !game->boolFlags.showScreenBorder;
    }
    if (IsKeyPressed(KEY_ZERO)) {
        game->boolFlags.showGUI = !game->boolFlags.showGUI;
    }
    // Fullscreen
    if (IsKeyPressed(KEY_F)) {
        ToggleFullscreen();

        if (IsWindowFullscreen()) {
            printf("Fullscreen\n");
            WINDOW_WIDTH = GetMonitorWidth(2);
            WINDOW_HEIGHT = GetMonitorHeight(2);

            // Update screen camera
            game->screenCamera.offset.x = WINDOW_WIDTH / 2;
            game->screenCamera.offset.y = WINDOW_HEIGHT / 2;
            // Update world camera
            game->worldCamera.offset.x = WINDOW_WIDTH / 2;
            game->worldCamera.offset.y = WINDOW_HEIGHT / 2;
            // Update shaky camera
            game->shakyCamera.offset.x = game->screenCamera.offset.x;
            game->shakyCamera.offset.y = game->screenCamera.offset.y;
        } else {
            printf("Windowed\n");
            WINDOW_WIDTH = WORLD_WIDTH;
            WINDOW_HEIGHT = WORLD_HEIGHT;

            // Update screen camera
            game->screenCamera.offset.x = WORLD_WIDTH / 2;
            game->screenCamera.offset.y = WORLD_HEIGHT / 2;
            // Update world camera
            game->worldCamera.offset.x = WORLD_WIDTH / 2;
            game->worldCamera.offset.y = WORLD_HEIGHT / 2;
            // Update shaky camera
            game->shakyCamera.offset.x = game->screenCamera.offset.x;
            game->shakyCamera.offset.y = game->screenCamera.offset.y;
        }
        PlayToastTween(&game->toast, "Toggle Fullscreen");
    }

    switch (game->cameraType) {
        case CAMERA_TILESET:

            // Do not let users interact with world while menu is up
            if (game->boolFlags.showGUI) {
                // Clicked right section
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) { GetWidth() - guiWidth, 0, guiWidth, GetHeight() })) {
                    return;
                }
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                printf("%d, %d\n", GetMouseX() / TILE_WIDTH, GetMouseY() / TILE_HEIGHT);
                game->tileSelected.x = GetMouseX() / TILE_WIDTH;
                game->tileSelected.y = GetMouseY() / TILE_WIDTH;
            }
            break;
        case CAMERA_WORLD:

            // Do not let users interact with world while menu is up
            if (game->boolFlags.showGUI) {
                // Clicked left sections
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) { 0, 0, guiWidth, GetHeight() })) {
                    return;
                }

                // Clicked right section
                if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) { GetWidth() - guiWidth, 0, guiWidth, GetHeight() })) {
                    return;
                }
            }

            // Reset
            if (IsKeyPressed(KEY_R)) {
                printf("Reset tiles...\n");
                for (int y = 0; y < NO_OF_TILES_Y; ++y) {
                    for (int x = 0; x < NO_OF_TILES_X; ++x) {
                        game->tiles[y * NO_OF_TILES_X + x].id = TILE_EMPTY;
                        game->tiles[y * NO_OF_TILES_X + x].srcRect = (Rectangle) {blankTile * TILE_WIDTH,blankTile * TILE_HEIGHT,TILE_WIDTH,TILE_HEIGHT};
                        game->backgroundTiles[y * NO_OF_TILES_X + x].id = TILE_EMPTY;
                        game->backgroundTiles[y * NO_OF_TILES_X + x].srcRect = (Rectangle) {blankTile * TILE_WIDTH,blankTile * TILE_HEIGHT,TILE_WIDTH,TILE_HEIGHT};
                    }
                }

                PlayToastTween(&game->toast, "Reset");
            }
            // Load map
            if (IsKeyPressed(KEY_L)) {
                printf("Load map...\n");
                LoadMap(game);

                PlayToastTween(&game->toast, "Load");
            }
            // Save map
            if (IsKeyPressed(KEY_PERIOD)) {
                printf("Save map...\n");
                SaveMap(game);
                PlayToastTween(&game->toast, "Save");
            }
            // Set tile modes
            if (IsKeyPressed(KEY_H)) {
                game->fillMode = FILL_HORIZONTAL;
                PlayToastTween(&game->toast, "Horizontal Fill");
            }
            if (IsKeyPressed(KEY_V)) {
                game->fillMode = FILL_VERTICAL;
                PlayToastTween(&game->toast, "Vertical Fill");
            }
            if (IsKeyPressed(KEY_O)) {
                game->fillMode = FILL_OFF;
                PlayToastTween(&game->toast, "Stamp");
            }

            // Draw tiles
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse = GetScreenToWorld2D(GetMousePosition(), game->worldCamera);
                int x = (int) (mouse.x / TILE_WIDTH);
                int y = (int) (mouse.y / TILE_HEIGHT);
                if (x < 0 || x > NO_OF_TILES_X) return;
                if (y < 0 || y > NO_OF_TILES_Y) return;

                switch (game->guiFlags.dropdownState) {
                    case 0:
                        // Foreground Layer
                        OnMouseDownLeft(game, game->tiles, x, y);
                        break;
                    case 1:
                        // Background Layer
                        OnMouseDownLeft(game, game->backgroundTiles, x, y);
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
                switch (game->guiFlags.dropdownState) {
                    case 0:
                        // Foreground Layer
                        OnMouseDownRight(game->tiles, x, y);
                        break;
                    case 1:
                        // Background Layer
                        OnMouseDownRight(game->backgroundTiles, x, y);
                        break;
                }

            }
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

        UpdateScreenShake(game);
    }

    // Bound world camera
    // if (worldCamera.target.x < 0) worldCamera.target.x = 0;
    // if (worldCamera.target.y < 0) worldCamera.target.y = 0;
    // if (worldCamera.target.x > WORLD_WIDTH) worldCamera.target.x = WORLD_WIDTH;
    // if (worldCamera.target.y > WORLD_HEIGHT) worldCamera.target.y = WORLD_HEIGHT;

    TweenToast(&game->toast);
    UpdateParticles(game);
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
            // Screenshake bar
            DrawRectangle(20, 20, game->screenShake.shake * 400, 25, ColorAlpha(GREEN, 0.6f));
            DrawRectangleLines(20, 20, 400, 25, ColorAlpha(BLACK, 0.2f));
        } else if (game->cameraType == CAMERA_SCREEN_SHAKE) {
            // DrawCameraScreen(game);
            DrawCameraScreenShake(game);
            // Screenshake bar
            DrawRectangle(20, 20, game->screenShake.shake * 400, 25, ColorAlpha(GREEN, 0.6f));
            DrawRectangleLines(20, 20, 400, 25, ColorAlpha(BLACK, 0.2f));
        }

        DrawGUI(game);

        DrawFPS(GetWidth() - MeasureText("60 FPS", 20) - 20, 20);
    EndDrawing();
}

// =========== Main ===========

int main(void) {

    // for (int i = 0; i < 50; i++) {
    //     printf("%.2f\n", GetRandomValueFloat(-1, 1));
    // }
    // return 0;

    srand((unsigned int)time(NULL));

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tile Map Editor");

    Texture2D checkered = CreateCheckeredBackground();
    Texture2D tileset = LoadTexture("resources/tilesets/world_tileset.png"); // 16 x 16 tileset
    // Texture2D tileset = LoadTexture("resources/tilesets/jungle.png"); // 16 x 16 tileset
    tileset.width *= 2;
    tileset.height *= 2;

    // World
    // Rectangle *tiles = malloc(sizeof(Rectangle) * NO_OF_TILES_X * NO_OF_TILES_Y);
    Tile tiles[NO_OF_TILES_X * NO_OF_TILES_Y];
    Tile backgroundTiles[NO_OF_TILES_X * NO_OF_TILES_Y];

    for (int y = 0; y < NO_OF_TILES_Y; ++y) {
        for (int x = 0; x < NO_OF_TILES_X; ++x) {
            // printf("%d - %d\n", i, y * NO_OF_TILES_Y + x);
            tiles[y * NO_OF_TILES_X + x].id = TILE_EMPTY;
            tiles[y * NO_OF_TILES_X + x].destRect = (Rectangle) {
                x * TILE_WIDTH, y * TILE_HEIGHT,
                TILE_WIDTH, TILE_HEIGHT};
            tiles[y * NO_OF_TILES_X + x].srcRect = (Rectangle) {blankTile * TILE_WIDTH,blankTile*TILE_HEIGHT,TILE_WIDTH,TILE_HEIGHT};

            // background tiles
            backgroundTiles[y * NO_OF_TILES_X + x].id = TILE_EMPTY;
            backgroundTiles[y * NO_OF_TILES_X + x].destRect = (Rectangle) {
                x * TILE_WIDTH, y * TILE_HEIGHT,
                TILE_WIDTH, TILE_HEIGHT};
            backgroundTiles[y * NO_OF_TILES_X + x].srcRect = (Rectangle) {blankTile * TILE_WIDTH,blankTile*TILE_HEIGHT,TILE_WIDTH,TILE_HEIGHT};
        }
    }

    // Player
    Rectangle player = { SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f, 20, 20};

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
    screenCamera.zoom = 2.0f;   // 2.8f is good zoom

    Camera2D shakyCamera = { 0 };
    shakyCamera.offset = screenCamera.offset;
    shakyCamera.target = (Vector2){ player.x, player.y };
    shakyCamera.rotation = screenCamera.rotation;
    shakyCamera.zoom = screenCamera.zoom;

    Rectangle worldBounds = { 0, 0, WORLD_WIDTH, WORLD_HEIGHT };
    Rectangle screenBounds = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    ScreenShake screenShake = {
        .shake = 0.0f,
        .maxAngle = 10.0f,
        .maxOffset = 10.0f,
        .angle   = 0.0f,
        .offsetX = 0.0f,
        .offsetY = 0.0f,
    };

    CameraType cameraType = CAMERA_SCREEN;
    FillMode fillMode = FILL_OFF;
    // int fillMode = FILL_HORIZONTAL;
    // int fillMode = FILL_VERTICAL;

    // GUI
    BoolFlags boolFlags = {
        .showGUI = false,
        .showGUIwindow = true,
        .showWindowBorder = false,
        .showScreenBorder = false,
        .showAxis = false,
        .showParticles = true,
    };
    GUIFlags guiFlags = {
        .dropdownActive = false,
        .dropdownState = 0
    };
    Toast toast = {
        .message = "Hello there whats goign one ok?",
        .position = (Vector2) { (GetWidth() - MeasureText(toast.message, 20)) / 2, GetHeight() + 50 },
        .tween = (Tween) {
            .currentPosition = { toast.position.x, toast.position.y },
            .targetPosition = { toast.position.x, GetHeight() - 50 },
            .duration = 140,
            .frameCounter = 0,
            .state = IDLE
         }
    };

    GuiSetStyle(DEFAULT, TEXT_SIZE, 10);
    GuiSetStyle(DEFAULT, TEXT_SPACING, 3);
    GuiSetStyle(DEFAULT, TEXT_LINE_SPACING, 25);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0xFFFFFFFF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
    GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
    // normal
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_NORMAL, 0xFFFFFF00);
    // focused
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_FOCUSED, 0xFFFFFF00);
    // opened
    GuiSetStyle(DROPDOWNBOX, BASE_COLOR_PRESSED, ColorToInt(ColorAlpha(GRAY, 0.8f)));
    GuiSetStyle(DROPDOWNBOX, TEXT_COLOR_PRESSED, 0xFFFFFFFF);
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0xFFFFFF00);


    // Particles
    size_t PARTICLES_SIZE_BYTE = sizeof(Particle) * NO_OF_PARTICLES;
    Particle *particles = RL_MALLOC(PARTICLES_SIZE_BYTE);
    for (int i = 0; i < NO_OF_PARTICLES; ++i) {
        Particle *p = &particles[i];
        p->id = GenerateRandomId();
        // printf("p->id: %d\n", p->id);
    }

    // CreateSimpleParticleEffect(particles);

    // Game
    Game game = { 0 };
    game.player = player;
    game.worldBounds = worldBounds;
    game.screenBounds = screenBounds;
    game.boolFlags = boolFlags;
    game.guiFlags = guiFlags;
    game.cameraType = cameraType;
    game.worldCamera = worldCamera;
    game.screenCamera = screenCamera;
    game.shakyCamera = shakyCamera;
    game.checkered = checkered;
    game.tileset = tileset;
    game.tileSelected = (Vector2) { 0, 0};
    game.tiles = tiles;
    game.backgroundTiles = backgroundTiles; // I should use single struct for 2d tiles layers
    game.fillMode = fillMode;
    game.overwriteTiles = false;
    game.screenShake = screenShake;
    game.toast = toast;
    game.particles = particles;
    game.img = LoadTexture("resources/sprites/piece.png");
    game.img.width /= 6;
    game.img.height /= 6;

    LoadMap(&game);

    // Log some stuff
    printf("==========================\n");
    printf("Moo\n");
    printf("==========================\n");
    Vector2 windowStart = GetScreenToWorld2D((Vector2){0,0}, game.worldCamera);
    Vector2 windowEnd = GetScreenToWorld2D((Vector2){WORLD_WIDTH,WORLD_HEIGHT}, game.worldCamera);
    Vector2 screenStart = GetScreenToWorld2D((Vector2){0,0}, game.screenCamera);
    Vector2 screenEnd = GetScreenToWorld2D((Vector2){SCREEN_WIDTH,SCREEN_HEIGHT}, game.screenCamera);
    int TOTAL_TILES_X = (int) (windowEnd.x / TILE_WIDTH) + 1;
    int TOTAL_TILES_Y = (int) (windowEnd.y / TILE_HEIGHT) + 1;
    // The position takes into account the offset, target and zoom levels.
    printf("windowStart %d, %d\n", (int) windowStart.x, (int) windowStart.y);
    printf("windowEnd %d, %d\n", (int) windowEnd.x, (int) windowEnd.y);
    printf("screenStart %d, %d\n", (int) screenStart.x, (int) screenStart.y);
    printf("screenEnd %d, %d\n", (int) screenEnd.x, (int) screenEnd.y);
    printf("---------------------\n");
    printf("Tile size: %ld bytes\n", sizeof(Tile));
    printf("Total size: %ld bytes\n", sizeof(Tile) * NO_OF_TILES_X * NO_OF_TILES_Y);
    printf("---------------------\n");
    printf("Total x tiles: %d\n", TOTAL_TILES_X);
    printf("Total y tiles: %d\n", TOTAL_TILES_Y);
    printf("---------------------\n");
    printf("Total (visible) x tiles: %d\n", (int) SCREEN_WIDTH / TILE_WIDTH);
    printf("Total (visible) y tiles: %d\n", (int) SCREEN_HEIGHT / TILE_HEIGHT);
    printf("---------------------\n");
    printf("No of particles: %d\n", NO_OF_PARTICLES);
    printf("Size particles:  %lu bytes\n", PARTICLES_SIZE_BYTE);
    printf("---------------------\n");
    printf("\n");

    // SetTargetFPS(60);
    // SetTargetFPS(100);
    while(!WindowShouldClose()) {
        // input
        Input(&game);

        // update
        Update(&game);

        // draw
        Draw(&game);
    }

    // free(tiles);
    RL_FREE(particles);
    UnloadTexture(game.checkered);
    UnloadTexture(game.tileset);
    UnloadTexture(game.img);
    CloseWindow();

    return 0;
}
