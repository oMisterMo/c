/**
 * So I DON'T forget !!!
 *
 * Todo: Actually load the spritesheet data using the example in others/json.c
 * Todo: Remove comments and that
 *
 */

#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

#define WIDTH 800
#define HEIGHT 800
#define STRENGTH 5.0f   // 1 = Weak, 3+ = Strong

int main(void) {
    InitWindow(WIDTH, HEIGHT, "Whos that Pokemon?");

    int pokemons = 151;
    // Init variables
    Vector2 pos = { WIDTH/2.0f, HEIGHT/2.0f };
    Texture2D img = LoadTexture("resources/sprites/114.png");
    Texture2D spritesheet = LoadTexture("resources/sprites/poke.png");
    // Texture2D imgs[pokemons];
    int index = 0;
    // for (int i = 0; i < pokemons; i++) {
    //     imgs[i] = LoadTexture(TextFormat("resources/sprites/emerald/%d.png", i+1));
    // }
    float scale = 2.0f;
    float t = 0.0f;    // 0 -> 1

    // Get image from spritesheet
    Rectangle srcRect = {589,761,62,55};    // random poke
    // Rectangle srcRect = {741,722,35,37};    // 151 = mew
    // Rectangle srcRect = {308,377,59,56};    // 3 = venasaur
    // Location on screen with scale (w and h at the end)
    // Rectangle destRect = {WIDTH/2.0f, HEIGHT/2.0f, srcRect.width, srcRect.height};

    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        // float dt = GetFrameTime();
        // printf("dt: %.2f\n", dt);

        //input
        float mouseX = (float)GetMouseX() / WIDTH;
        // printf("%f\n", mouseX);
        // printf("%d\n", GetMouseX());
        // if (IsKeyPressed(KEY_SPACE)) {
        //     printf("heill\n");
        //     index = GetRandomValue(0, pokemons);
        // }

        //update
        // t += 0.001;

        //draw
        BeginDrawing();
            ClearBackground((Color) {213,240,234,1});
            // Game
            float scale = Lerp(1, 10, mouseX);

            // Old way
            // Vector2 atOriginScaled = Vector2Subtract(pos, (Vector2) {(img.width*scale)/2.0f, (img.height*scale)/2.0f});
            // DrawTextureEx(img, atOriginScaled, 0, scale, ColorLerp(BLACK, WHITE, mouseX / STRENGTH));
            // DrawTextureEx(imgs[index], atOriginScaled, 0, scale, ColorLerp(BLACK, WHITE, mouseX / STRENGTH));

            // New way
            // DrawTexture(spritesheet, 0, 0, WHITE);
            Rectangle destRect = {WIDTH/2.0f, HEIGHT/2.0f, srcRect.width * scale, srcRect.height * scale};
            DrawTexturePro(spritesheet, srcRect, destRect, (Vector2) {destRect.width/2.0f,destRect.height/2.0f}, 0.0f, ColorLerp(BLACK, WHITE, mouseX / STRENGTH));
            // DrawTextureEx(spritesheet, atOriginScaled, 0, scale, ColorLerp(BLACK, WHITE, mouseX / STRENGTH));

            // DrawCircleV(pos, 10, ColorAlpha(RED, 0.1f));
            // UI
            DrawText(TextFormat("%.2f", mouseX), 20, 20, 30, BLACK);
        EndDrawing();
    }

    UnloadTexture(img);
    UnloadTexture(spritesheet);
    // for (int i = 0; i < pokemons; i++) {
    //     UnloadTexture(imgs[i]);
    // }
    CloseWindow();

    return 0;
}
