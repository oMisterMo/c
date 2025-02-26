#include <stdio.h>
#include <math.h>
#include "raylib.h"

// #define PI atan2f(1, 1) * 4



int main(void) {


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
    float scale = 2;
    target.texture.width * scale;
    target.texture.height * scale;
    float xOff = 100;
    float yOff = 100;

    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0 };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {


        // BeginTextureMode(target);

        //     DrawLine(xOff + 0, yOff + -100, xOff + 0,   yOff + 100, BLACK);
        //     DrawLine(xOff + -100, yOff + 0, xOff + 100, yOff + 0, BLACK);


        //     for (size_t i = 0; i < samples; ++i) {
        //         float t = (float) i / samples;
        
        //         in[i] = sinf(2 * PI * t) ;

        //         DrawCircle(xOff + t * RAD2DEG, yOff + in[i] * RAD2DEG, 5, RED);
        //     }

        // EndTextureMode();



        const float scale = 4;
        BeginDrawing();
        ClearBackground(RAYWHITE);

            DrawTexture(target.texture, 0, 0, WHITE);
            // DrawTextureRec(
            //     target.texture,
            //     (Rectangle) { 0, 0, (float)screenWidth, (float)-screenHeight },
            //     (Vector2) { GetScreenWidth() / 2 - target.texture.width / 8, GetScreenHeight() / 2 - target.texture.height / 8 },
            //     WHITE
            // );
            // Rectangle src = { 0, 0, (float)screenWidth, (float)-screenHeight };
            // Rectangle dest = (Rectangle) { GetScreenWidth() / 2 - target.texture.width / 8, GetScreenHeight() / 2 - target.texture.height / 8, target.texture.width * scale, target.texture.height * scale };
            // DrawTexturePro(
            //     target.texture,
            //     src,
            //     dest,
            //     (Vector2) {0}, 0,
            //     WHITE
            // );

            // DrawTextureEx(target.texture, (Vector2) { - 200, - 100}, 0, 2, WHITE);

            BeginMode2D(camera);
                DrawLine(xOff + 0, yOff + -100, xOff + 0,   yOff + 100, BLACK);
                DrawLine(xOff + -100, yOff + 0, xOff + 100, yOff + 0, BLACK);

                for (size_t i = 0; i < samples; ++i) {
                    float t = (float) i / samples;
            
                    in[i] = sinf(2 * PI * t) ;

                    DrawCircle(xOff + t * RAD2DEG, yOff + in[i] * RAD2DEG, 5, RED);
                }
            EndMode2D();

        EndDrawing();
        
    }
    
    
    UnloadRenderTexture(target);
    CloseWindow();

    return 0;
}
