#include <stdio.h>
#include <math.h>
#include "raylib.h"

// #define PI atan2f(1, 1) * 4



int main(void) {

    printf("-------------------\n");
    printf("Init Window\n");
    printf("-------------------\n");
    int screenWidth = 900;
    int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Graph");


    // vars
    size_t samples = 8;
    float in[samples];

    for (size_t i = 0; i < samples; ++i) {
        float t = (float) i / samples;
        printf("t = %.2f, ", t);
        in[i] = sinf(2 * PI * t) ;
    }

    RenderTexture target = LoadRenderTexture(screenWidth, screenHeight);

    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0 };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 2.5f;

    SetTargetFPS(60);

    printf("-------------------\n");
    printf("GAME\n");
    printf("-------------------\n");

    while (!WindowShouldClose()) {

        // input

        // Camera zoom controls
        camera.zoom += ((float)GetMouseWheelMove()*0.4f);

        if (camera.zoom > 6.0f) camera.zoom = 6.0f;
        else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        // Camera reset (zoom and rotation)
        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 2.5f;
            camera.rotation = 0.0f;
        }


        // update


        // draw
        const float RADIUS = 3;
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                // Draw y-axis
                DrawLine(0, -100, 0, 100, BLACK);
                // Draw x-axis
                DrawLine(-100, 0, 100, 0, BLACK);
                for (size_t i = 0; i < samples; ++i) {
                    float t = (float) i / samples;
                    in[i] = sinf(2 * PI * t) ;
                    DrawCircle(t * RAD2DEG, in[i] * RAD2DEG, RADIUS, RED);
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

    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
