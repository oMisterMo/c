#include <stdio.h>
#include <raylib.h>
#include <raymath.h>

#define WIDTH 800
#define HEIGHT 800
#define STRENGTH 5.0f   // 1 = Weak, 3+ = Strong

int main(void) {
    InitWindow(WIDTH, HEIGHT, "Lerp");

    int pokemons = 151;
    // Init variables
    Vector2 pos = { WIDTH/2, HEIGHT/2 };
    Texture2D img = LoadTexture("resources/sprites/114.png");
    Texture2D imgs[pokemons];
    int index = 0;
    for (int i = 0; i < pokemons; i++) {
        imgs[i] = LoadTexture(TextFormat("resources/sprites/emerald/%d.png", i+1));
    }
    float scale = 2.0f;
    float t = 0.0f;    // 0 -> 1

    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        // float dt = GetFrameTime();
        // printf("dt: %.2f\n", dt);

        //input
        float mouseX = (float)GetMouseX() / WIDTH;
        // printf("%f\n", mouseX);
        // printf("%d\n", GetMouseX());
        if (IsKeyPressed(KEY_SPACE)) {
            printf("heill\n");
            index = GetRandomValue(0, pokemons);
        }

        //update
        // t += 0.001;

        //draw
        BeginDrawing();
            ClearBackground((Color) {213,240,234,1});
            // Game
            float scale = Lerp(1, 10, mouseX);
            Vector2 atOriginScaled = Vector2Subtract(pos, (Vector2) {(img.width*scale)/2, (img.height*scale)/2});


            // DrawTextureEx(img, atOriginScaled, 0, scale, ColorLerp(BLACK, WHITE, mouseX / STRENGTH));
            DrawTextureEx(imgs[index], atOriginScaled, 0, scale, ColorLerp(BLACK, WHITE, mouseX / STRENGTH));


            // DrawCircleV(pos, 10, ColorAlpha(RED, 0.1f));
            // UI
            DrawText(TextFormat("%.2f", mouseX), 20, 20, 30, BLACK);
        EndDrawing();
    }

    UnloadTexture(img);
    for (int i = 0; i < pokemons; i++) {
        UnloadTexture(imgs[i]);
    }
    CloseWindow();

    return 0;
}
