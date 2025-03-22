#include <stdio.h>
#include <stdbool.h>
#include <math.h>   // For sin(), cos(), floor()
#include <stdlib.h> // For rand()

#include "raylib.h"
#include "reasings.h"
#include "stb_perlin.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768



typedef enum {
    IDLE = 0,
    TWEENING
} TweenState;

typedef struct Tween {
    Vector2 currentPosition;      // Tween start
    Vector2 targetPosition;     // Tween end [could be consts]
    int state;                  // IDLE | TWEENING
    int frameCounter;           // Current time in tween
    int duration;               // How long to tween in frames e.g 30 frames = 500ms, 60 = 1sec
} Tween;

typedef struct Sprite {
    Texture2D texture;
    Rectangle bounds;
    Vector2 velocity;
    // Vector2 acceleration;
    Tween tween;
} Sprite;

typedef struct GameObject {
    Texture2D texture;
    Rectangle bounds;
    Tween tween;

    // Shake
    float shakeDuration;  // Duration of the shake effect
    float shakeIntensity; // Intensity of the shake (in pixels)
    float noiseTime;      // Time step for Perlin noise
} GameObject;



// UGGGGHH
// Gradient function for smooth interpolation
float grad(int hash, float x) {
    int h = hash & 15; // Limit to 4 bits
    float grad = 1.0f + (h & 7); // Gradient value in range [1, 8]
    if (h & 8) grad = -grad;    // Negate for some gradients
    return grad * x;
}
// Fade function for smoother transitions
float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}
// Linear interpolation helper function
float lerp(float t, float a, float b) {
    return a + t * (b - a);
}
// 1D Perlin noise function
float perlinNoise(float x) {
    int X = (int)floor(x) & 255; // Find the unit grid cell
    x -= floor(x);               // Relative X coordinate
    float u = fade(x);           // Smooth the fade

    // Hash values for the corners
    int A = rand() % 256;  // Example random hash values
    int B = rand() % 256;

    // Linearly interpolate between the gradients
    return lerp(u, grad(A, x), grad(B, x - 1));
}
void ApplyPerlinScreenShake(GameObject *obj) {
    if (obj->shakeDuration > 0.0f) {
        // Use Perlin noise to generate offsets
        obj->bounds.x = obj->bounds.x +  perlinNoise(obj->noiseTime) * obj->shakeIntensity;
        obj->bounds.y = obj->bounds.y +  perlinNoise(obj->noiseTime + 100.0f) * obj->shakeIntensity; // Offset for Y-axis

        // Increment noiseTime to get new noise values each frame
        obj->noiseTime += 0.1f;

        // Reduce the shake duration over time
        obj->shakeDuration -= 0.016f; // Assuming a frame time of ~16ms
    } else {
        // Reset offsets when shake is done
        obj->bounds.x = (GetScreenWidth() - obj->bounds.width) / 2;
        obj->bounds.y = (GetScreenHeight() - obj->bounds.height) / 2;
    }
}





// THE GOOD STUFF
void InitMo(GameObject *mo) {
    Rectangle bounds = { 0, SCREEN_HEIGHT / 2  - mo->texture.height / 2, mo->texture.width, mo->texture.height };
    Tween tween = { 0 };
    tween.currentPosition = (Vector2) { bounds.x, bounds.y };
    tween.targetPosition = (Vector2) { GetScreenWidth() - mo->texture.width , SCREEN_HEIGHT / 2 };
    tween.state = IDLE;
    tween.frameCounter = 0;
    tween.duration = 60 * 2;

    mo->tween = tween;
    mo->bounds = bounds;
}
void OnLeftClick(GameObject *mo) {
    printf("left mouse pressed...\n");
    if (CheckCollisionPointRec(GetMousePosition(), mo->bounds)) {
        // reset mo
        mo->tween.state = TWEENING;
        return;
    }

    // Reset all

    // reset mo
    mo->tween.state = TWEENING;
}
void OnRightClick(GameObject *mo) {
    mo->shakeDuration = 0.5f;
    mo->shakeIntensity = 0.5f;
}
void HandleInput(GameObject *mo) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        OnLeftClick(mo);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        OnRightClick(mo);
    }
}
void UpdateMo(GameObject *mo) {
    if (mo->tween.state == TWEENING) {
        mo->tween.frameCounter++;

        // Tween
        mo->bounds.x  = EaseElasticOut(
            (float) mo->tween.frameCounter,
            mo->tween.currentPosition.x,
            mo->tween.targetPosition.x - mo->tween.currentPosition.x,
            mo->tween.duration
        );

        // Tween complete
        if (mo->tween.frameCounter >= mo->tween.duration) {
            mo->tween.frameCounter = 0;
            mo->tween.state = IDLE;


            // Set final position
            mo->bounds.x = mo->tween.currentPosition.x;
        }
    }
}
float GetPerlin(float time) {
    float p = stb_perlin_noise3(time / 3 , 0.0f, 0.0f, 0, 0, 0);
    // float p = stb_perlin_fbm_noise3(currentTime, y, z, 2.0f, 0.5f, 1);

    // Clamp between -1.0f and 1.0f
    if (p < -1.0f) p = -1.0f;
    if (p > 1.0f) p = 1.0f;

    // We need to normalize the data from [-1..1] to [0..1]
    float np = (p + 1.0f) / 2.0f;

    return np;
}
void UpdatePerlin(GameObject *mo, float timeLastSpawn, float spawnInterval) {
    float currentTime = GetTime();

    if (currentTime - timeLastSpawn > spawnInterval) {

        float x = GetPerlin(currentTime);
        float y = GetPerlin(currentTime + 10000);

        // Multiply by size
        // pos.x = np * GetScreenWidth();
        mo->bounds.x = x * GetScreenWidth();
        mo->bounds.y = y * GetScreenHeight();
        printf("result %.2f\n", x);

    }
}
void DrawMo(GameObject mo) {
    // Draw Mo
    // DrawRectangleRec(mo.bounds, WHITE);
    DrawTextureV(mo.texture, (Vector2) { mo.bounds.x, mo.bounds.y }, WHITE);
    // DrawText(TextFormat("x %.1f\ny %.1f", mo.bounds.x, mo.bounds.y), mo.bounds.x + mo.texture.width / 2 - 50, mo.bounds.y - 50, 20, WHITE);
}







int main() {

    printf("-------------------\n");
    printf("INIT WINDOW\n");
    printf("-------------------\n");

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Perlin");
    SetMousePosition(-1, -1);


    printf("-------------------\n");
    printf("LOAD TEXTURES\n");
    printf("-------------------\n");
    Texture2D bunny = LoadTexture("resources/sprites/piece.png");
    // Create perlin texture -> Not needed
    Image perlin = GenImagePerlinNoise(100, 100, 0, 0, 1.0f);
    Texture2D perlinTexture = LoadTextureFromImage(perlin);
    UnloadImage(perlin);


    printf("-------------------\n");
    printf("INIT VARIABLES\n");
    printf("-------------------\n");
    GameObject mo = {
        .texture = bunny,
        .shakeDuration = 0.0f,
        .shakeIntensity = 0.0f,
        .noiseTime = 0.0f
     };
    InitMo(&mo);

    // Random stuff
    float spawnInterval = 0.3f; // Every 1 second
    float timeLastSpawn = 0.0;
    int index = 0;
    Vector2 pos = { 0, GetScreenHeight() / 2 };

    SetTargetFPS(60);

    printf("-------------------\n");
    printf("GAME\n");
    printf("-------------------\n");
    while(!WindowShouldClose()) {

        // Input
        HandleInput(&mo);

        // Update
        UpdatePerlin(&mo, timeLastSpawn, spawnInterval);
        // UpdateMo(&mo);
        // ApplyPerlinScreenShake(&mo);

        // Draw
        BeginDrawing();

            ClearBackground(SKYBLUE);
            // float size = 15;
            // DrawCircle(pos.x, pos.y, size, BLACK);
            DrawMo(mo);

        EndDrawing();
    }

    printf("-------------------\n");
    printf("DESTROY\n");
    printf("-------------------\n");

    UnloadTexture(bunny);
    UnloadTexture(perlinTexture);
    CloseWindow();
}


