#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"

// #define PI atan2f(1, 1) * 4



int main(void) {

    printf("-------------------\n");
    printf("Init Window\n");
    printf("-------------------\n");
    int screenWidth = 900;
    int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Graph");


    // vars

    const float RADIUS = 2;
    size_t samples = 16;
    float in[samples];

    printf("\n=====================\n");
    for (size_t i = 0; i < samples; ++i) {
        float t = (float) i / samples;
        printf("t = %.2f, ", t);
        in[i] = sinf(2 * PI * t) ;
    }
    printf("\n=====================\n\n");

    RenderTexture target = LoadRenderTexture(screenWidth, screenHeight);

    // Vector2 initialMousePosition = {};
    // float scale = 2.5f;

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

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }

        // Camera zoom center of screen
        camera.zoom += ((float)GetMouseWheelMove()*0.4f);

        if (camera.zoom > 6.0f) camera.zoom = 6.0f;
        else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        // // Zoom based on mouse wheel
        // float wheel = GetMouseWheelMove();
        // if (wheel != 0)
        // {
        //     // Get the world point that is under the mouse
        //     Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

        //     // Set the offset to where the mouse is
        //     camera.offset = GetMousePosition();

        //     // Set the target to match, so that the camera maps the world space point
        //     // under the cursor to the screen space point under the cursor at any zoom
        //     camera.target = mouseWorldPos;

        //     // Zoom increment
        //     float scaleFactor = 1.0f + (0.25f*fabsf(wheel));
        //     if (wheel < 0) scaleFactor = 1.0f/scaleFactor;
        //     camera.zoom = Clamp(camera.zoom*scaleFactor, 0.125f, 64.0f);
        // }

        // Camera reset (zoom and rotation)
        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 2.5f;
            camera.rotation = 0.0f;
        }


        // update


        // draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                // Draw y-axis
                DrawLine(0, -100, 0, 100, BLACK);
                // Draw x-axis
                DrawLine(-100, 0, 100, 0, BLACK);
                // x
                DrawText("1", 1 * RAD2DEG - MeasureText("1", 5), 0, 5, BLACK);
                // y
                DrawText("1", -5, (1 * RAD2DEG - MeasureText("1", 5)) *  1, 5, BLACK);
                DrawText("-1", -5, (1 * RAD2DEG - MeasureText("1", 5)) * -1, 5, BLACK);

                // Draw Points
                for (size_t i = 0; i < samples; ++i) {
                    float t = (float) i / samples;
                    // in[i] = sinf(2 * PI * t) ;
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
