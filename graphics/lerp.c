#include <stdio.h>
#include <raylib.h>
#include <raymath.h>

#define WIDTH 800
#define HEIGHT 800

int main(void) {
    InitWindow(WIDTH, HEIGHT, "TEST");

    // Init variables
    Vector2 pos = { WIDTH/2, HEIGHT/2 };
    Texture2D img = LoadTexture("resources/sprites/114.png");
    float scale = 2.0f;
    float t = 0.0f;    // 0 -> 1

    SetTargetFPS(60);
    while(!WindowShouldClose()) {

        //input
        float mouseX = (float)GetMouseX() / WIDTH;
        printf("%f\n", mouseX);
        // printf("%d\n", GetMouseX());
        //update
        // t += 0.001;
        //draw
        BeginDrawing();
            ClearBackground(WHITE);
            // Vector2 atOrigin = Vector2Subtract(pos, (Vector2) {(img.width)/2, (img.height)/2});

            float scale = Lerp(1, 10, mouseX);

            Vector2 atOriginScaled = Vector2Subtract(pos, (Vector2) {(img.width*scale)/2, (img.height*scale)/2});
            DrawTextureEx(img, atOriginScaled, 0, scale, ColorLerp(BLACK, WHITE, mouseX));
            DrawCircleV(pos, 10, ColorAlpha(RED, 0.1f));
        EndDrawing();

    }

    UnloadTexture(img);
    CloseWindow();

    return 0;
}
