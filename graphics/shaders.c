#include <stdio.h>
#include "raylib.h"

typedef struct {
    Rectangle rect;
    RenderTexture2D target;
    Texture2D texture;
    Rectangle src;
    Rectangle dest;
    int screenWidth;
    int screenHeight;
} Game;

void KeepRectOnScreen(Rectangle *rect) {
    if (rect->x > GetScreenWidth()) {
        rect->x = -rect->width;
    }
    if (rect->y > GetScreenHeight()) {
        rect->y = -rect->height;
    }
}

void BasicShader(Game *game) {
    Rectangle rect = game->rect;
    RenderTexture2D target = game->target;
    int screenWidth = game->screenWidth;
    int screenHeight = game->screenHeight;
    Texture2D texture = game->texture;
    Rectangle src = game->src;

    // Shader on shape? [doesn't work on shapes]
    // DrawRectangleRec(rect, RED);

    // Reular texture - Overlayed with shader fragments [incorrect flip]
    // DrawTexturePro(texture, src, rect, (Vector2){ 0, 0 }, 0, WHITE);

    // Empty texture spanning the full screen
    // DrawTexture(target.texture, 0, 0, WHITE);    [incorrect flip]
    DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)screenWidth, (float)-screenHeight }, (Vector2) { 0, 0 }, WHITE);
}

int main(void) {

    printf("-------------------\n");
    printf("Init Window\n");
    printf("-------------------\n");
    int screenWidth = 800 * 2;
    int screenHeight = 400 * 2;
    InitWindow(screenWidth, screenHeight, "Shaders");

    printf("-------------------\n");
    printf("Init Textures\n");
    printf("-------------------\n");
    Texture2D texture = LoadTexture("resources/sprites/mo.png");
    Rectangle src = { 0, 0, texture.width, texture.height };
    Rectangle rect = { 0, 0, texture.width, texture.height };
    rect.x = (GetScreenWidth() - rect.width) / 2;
    rect.y = (GetScreenHeight() - rect.height) / 2;

    RenderTexture2D target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    printf("-------------------\n");
    printf("Init Shaders\n");
    printf("-------------------\n");
    // Load shader to be used on some parts drawing
    // NOTE 1: Using GLSL 330 shader version, on OpenGL ES 2.0 use GLSL 100 shader version
    // NOTE 2: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
    Shader shader = LoadShader(NULL, "./shaders/simple.fs");
    // Shader shader = LoadShader(NULL, "./shaders/fog.fs");
    // Shader shader = LoadShader(NULL, "./shaders/pixelizer.fs");
    // Shader shader = LoadShader(NULL, "./shaders/grayscale.fs");
    // Shader shader = LoadShader(NULL, NULL);

    Game game = {
        .rect = rect,
        .target = target,
        .texture = texture,
        .src = src,
        .screenWidth = screenWidth,
        .screenHeight = screenHeight,
    };

    SetTargetFPS(60);

    printf("-------------------\n");
    printf("GAME\n");
    printf("-------------------\n");

    while (!WindowShouldClose()) {
        // input
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(GetMousePosition(), rect)) {
                printf("clicked\n");
                rect.x = GetRandomValue(0, GetScreenWidth() - rect.width);
                rect.y = GetRandomValue(0, GetScreenHeight() - rect.height);
            }
        }

        // update
        // rect.x += 2;
        // rect.y += 2;

        KeepRectOnScreen(&rect);
        
        
        // draw
        BeginDrawing();
            ClearBackground(GRAY);

            // Shader stuff - Texture height must be flipped
            BeginShaderMode(shader);
                BasicShader(&game);
            EndShaderMode();

            // Normal drawing
            DrawTexturePro(texture, src, rect, (Vector2){ 0, 0 }, 0, WHITE);
            DrawFPS(20, 20);
        EndDrawing();
    }

    printf("-------------------\n");
    printf("Destroy\n");
    printf("-------------------\n");

    UnloadTexture(texture);
    UnloadShader(shader);
    UnloadRenderTexture(target);
    CloseWindow();



    return 0;
}
