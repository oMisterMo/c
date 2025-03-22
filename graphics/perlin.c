#include "raylib.h"
#include "reasings.h"
#include "stb_perlin.h"

#include <stdio.h>
#include <stdbool.h>

#include <math.h>   // For sin(), cos(), floor()
#include <stdlib.h> // For rand()


#define NO_OF_RECTS 1
#define NO_OF_SPACES (NO_OF_RECTS + 1)
#define RECT_WIDTH 100
#define RECT_HEIGHT 100
#define TOTAL_WIDTH RECT_WIDTH * NO_OF_RECTS
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)

Vector2 colors[1024] = { 0 }; 

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
void OnLeftClick(Vector2 center, GameObject *mo) {
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
void handleInput(Vector2 center, GameObject *mo) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        OnLeftClick(center, mo);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        OnRightClick(mo);
    }
}
void updateMo(GameObject *mo) {
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
void drawMo(GameObject mo) {
    // Draw Mo
    // DrawRectangleRec(mo.bounds, WHITE);
    DrawTextureV(mo.texture, (Vector2) { mo.bounds.x, mo.bounds.y }, WHITE);
    DrawText(TextFormat("x %.1f\ny %.1f", mo.bounds.x, mo.bounds.y), mo.bounds.x + mo.texture.width / 2 - 50, mo.bounds.y - 50, 20, WHITE);
}







int main() {

    // ---------------------------------------------
    // Setup
    // ---------------------------------------------
    InitWindow(1024, 768, "Tween");
    SetMousePosition(-1, -1);


    // ---------------------------------------------
    // Initialize
    // ---------------------------------------------
    Vector2 center = { GetScreenWidth() / 2, GetScreenHeight() / 2 };

    // Mo stuff
    Texture2D bunny = LoadTexture("resources/sprites/piece.png");
    Rectangle bounds = { 0, center.y  - bunny.height / 2, bunny.width, bunny.height };
    Tween tween = { 0 };
    tween.currentPosition = (Vector2) { bounds.x, bounds.y };
    tween.targetPosition = (Vector2) { GetScreenWidth() - bunny.width , center.y };
    tween.state = IDLE;
    tween.frameCounter = 0;
    tween.duration = 60 * 2;
    // Designated initializers
    GameObject mo = {
        .texture = bunny,
        .bounds = bounds,
        .tween = tween,

        .shakeDuration = 0.0f,
        .shakeIntensity = 0.0f,
        .noiseTime = 0.0f
     };


     Image perlin = GenImagePerlinNoise(100, 100, 0, 0, 1.0f);
     Texture2D perlinTexture = LoadTextureFromImage(perlin);
     UnloadImage(perlin);

    // Rectangle stuff
    Vector2 startRect = { 20, GetScreenHeight() - RECT_HEIGHT - 20 };
    Vector2 endRect = { GetScreenWidth() - RECT_WIDTH - 20, startRect.y };
    int frameCounter = 0;


    // for (int i = 0; i < 1024; ++i) {
    //     colors[i].x =  perlinNoise(GetTime() * 20);
    //     colors[i].y = GetScreenHeight() / 2 + perlinNoise(GetTime());
    // }

    float spawnInterval = 0.3f; // Every 1 second
    float timeLastSpawn = 0.0;
    int index = 0;

    int frameCount = 0;
    Vector2 pos = { 0, GetScreenHeight() / 2 };

    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        frameCount++;

        // ---------------------------------------------
        // Input
        // ---------------------------------------------
        handleInput(center, &mo);

        // ---------------------------------------------
        // Update
        // ---------------------------------------------
    
        // float currentTime = GetTime();
        // if (currentTime - timeLastSpawn > spawnInterval) {

        //     colors[index].x =  perlinNoise(GetTime());
        //     colors[index].y = GetScreenHeight() / 2 + perlinNoise(GetTime());

        //     printf("index: %.2f\n", perlinNoise(0.5));
        //     index++;
        //     index %= 1024;
        // }

        float currentTime = GetTime();
        float x = 1.5f, y = 0.0f, z = 0.0f;

        if (currentTime - timeLastSpawn > spawnInterval) {
            // x = 100 * perlinNoise(0.005 * frameCount);
            // y = 100 * perlinNoise(0.005 * frameCount + 100);
            float p = stb_perlin_noise3(currentTime, y, z, 0, 0, 100);
            if (p < -1.0f) p = -1.0f;
            if (p > 1.0f) p = 1.0f;
            float np = (p + 1.0f)/2.0f;
            // pos.x = (p + 1)/2 * GetScreenWidth();
            pos.x = np * GetScreenWidth();
            // y = GetScreenHeight() / 2;

            printf("result %.2f\n", np);
            // printf("time %.2f\n", GetTime());

            // timeLastSpawn = GetTime();

            // x *= GetScreenWidth();
            // x += GetScreenWidth() / 2;

        }


        // ApplyPerlinScreenShake(&mo);
        // updateMo(&mo);

        // ---------------------------------------------
        // Draw
        // ---------------------------------------------
        BeginDrawing();
        ClearBackground(SKYBLUE);

        // drawMo(mo);
        // DrawTexture(perlinTexture, 0, 0, WHITE);
        float size = 15;
        DrawCircle(pos.x, pos.y, size, BLACK);
        // DrawCircle(x, y, size, BLACK);
        // for (int i = 0; i < 1024; ++i) {
        //     // DrawPixel(colors[i].x, colors[i].y, BLACK);
        //     // DrawCircle(colors[i].x + (i * size), colors[i].y, size, BLACK);
        // }

        EndDrawing();
    }

    // ---------------------------------------------
    // Destroy
    // ---------------------------------------------
    UnloadTexture(bunny);
    CloseWindow();
}


