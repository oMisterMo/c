/**
 * Must do:
 *          1. Modularize methods
 *          2. Use raygui to modify values easily
 *              - Color
 *              - Radius
 *              - sin(x)
 *              - Grid (on/off)
 *              -
 *              -
 *          3. Hot reload C
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"

// #define PI atan2f(1, 1) * 4
enum SAMPLE_SIZE {
    SAMEPLE_LOW = 256,
    SAMEPLE_MEDIUM = 512,
    SAMEPLE_HIGH = 1024,
    SAMEPLE_HIGHER = 2048,
    SAMEPLE_HIGHEST = 4096,
};

Texture2D GenerateCheckedTexture() {
    // Generate a checked texture by code
    int width = GetMonitorWidth(1);
    int height = GetMonitorHeight(1);
    // Dynamic memory allocation to store pixels data (Color type)
    const float FADE = 0.08f;
    Color *pixels = (Color *)malloc(width*height*sizeof(Color));
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (((x/32+y/32)/1)%2 == 0) pixels[y*width + x] = Fade(WHITE, FADE);
            else pixels[y*width + x] = Fade(GRAY, FADE);
        }
    }
    // Load pixels data into an image structure and create texture
    Image checkedIm = {
        .data = pixels,             // We can assign pixels directly to data
        .width = width,
        .height = height,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
        .mipmaps = 1
    };

    Texture2D checked = LoadTextureFromImage(checkedIm);
    UnloadImage(checkedIm);
    return checked;
}

int main(void) {

    printf("-------------------\n");
    printf("Init Window\n");
    printf("-------------------\n");
    int screenWidth = 900;
    int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Graph");


    printf("-------------------\n");
    printf("Load Textures\n");
    printf("-------------------\n");
    Texture2D checked = GenerateCheckedTexture();

    printf("-------------------\n");
    printf("Init Variables\n");
    printf("-------------------\n");
    const float RADIUS = 1;
    size_t samples = 8;
    float in[samples];
    for (size_t i = 0; i < samples; ++i) {
        float t = (float) i / samples;
        in[i] = sinf(2 * PI * t);

        printf("t = %.2f, ", t);
    }

    // Camera
    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0 };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 2.5f;

    // ========================================



    SetTargetFPS(60);

    printf("-------------------\n");
    printf("GAME\n");
    printf("-------------------\n");

    while (!WindowShouldClose()) {

        // input

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }

        // Camera zoom center of screen
        camera.zoom += ((float)GetMouseWheelMove()*0.4f);

        // if (camera.zoom > 6.0f) camera.zoom = 6.0f;
        // else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        // Zoom based on mouse wheel
        float wheel = GetMouseWheelMove();
        if (wheel != 0){
            // Get the world point that is under the mouse
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

            // Set the offset to where the mouse is
            camera.offset = GetMousePosition();

            // Set the target to match, so that the camera maps the world space point
            // under the cursor to the screen space point under the cursor at any zoom
            camera.target = mouseWorldPos;

            // Zoom increment
            float scaleFactor = 1.0f + (0.25f*fabsf(wheel));
            if (wheel < 0) scaleFactor = 1.0f/scaleFactor;
            camera.zoom = Clamp(camera.zoom*scaleFactor, 1.0f, 64.0f);
        }

        // Camera reset (zoom and rotation)
        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 2.5f;
            camera.rotation = 0.0f;
        }
        if (IsKeyPressed(KEY_F)) {
            ToggleFullscreen();
            camera.target.x = 0;
            camera.target.y = 0;
        }


        // update


        // draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(checked, 0, 0, Fade(WHITE, 0.5f));
            BeginMode2D(camera);
                // Draw y-axis
                DrawLine(0, -100, 0, 100, BLACK);
                // Draw x-axis
                DrawLine(-100, 0, 100, 0, BLACK);
                // x
                DrawText("1", 1 * RAD2DEG - MeasureText("1", 5), 0, 5, BLACK);
                DrawLine(-100, 1 * RAD2DEG, 100, 1 * RAD2DEG, Fade(GRAY, 0.2f));
                DrawLine(-100, -1 * RAD2DEG, 100, -1 * RAD2DEG, Fade(GRAY, 0.2f));
                // y
                DrawText("1",
                    -5 - MeasureText("1", 5),   // x
                    (1 * RAD2DEG) * 1 - 3,      // y
                    5, BLACK);
                DrawText("-1",
                    -5 - MeasureText("-1", 5),  // x
                    (1 * RAD2DEG) * -1 - 3,     // y
                    5, BLACK);

                // Draw Points
                for (size_t i = 0; i < samples; ++i) {
                    float t = (float) i / samples;
                    // in[i] = sinf(2 * PI * t) ;
                    float x = t * RAD2DEG;
                    float y = (in[i] * RAD2DEG);
                    DrawCircle(x, -y, RADIUS, RED);
                }

                // Draw sample lines
                for (size_t i = 0; i < samples; ++i) {
                    float t = (float) i / samples;
                    DrawLine(t * RAD2DEG, -100, t * RAD2DEG, 100, Fade(GRAY, 0.2f));
                }
            EndMode2D();
        EndDrawing();
        
    }
    
    printf("-------------------\n");
    printf("Destroy\n");
    printf("-------------------\n");

    UnloadTexture(checked);
    CloseWindow();

    return 0;
}
