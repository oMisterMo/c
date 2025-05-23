#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

typedef struct {
    Rectangle rect;
    RenderTexture2D target;
    Texture2D texture;
    Rectangle src;
    Rectangle dest;
    int screenWidth;
    int screenHeight;
    Shader shader_0;
    Shader shader_1;
    Shader shader_2;
    Shader shader_3;
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
    BeginShaderMode(game->shader_0);
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
        // DrawTexture(target.texture, 0, 0, WHITE);    //[incorrect flip]
        DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)screenWidth, (float)-screenHeight }, (Vector2) { 0, 0 }, WHITE);
    EndShaderMode();
}

void IntermediateShader(Game *game) {
    BeginShaderMode(game->shader_1);
        DrawTextureRec(game->target.texture, (Rectangle) { 0, 0, (float)game->screenWidth, (float)-game->screenHeight }, (Vector2) { 0, 0 }, WHITE);
    EndShaderMode();
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
    Shader shader_0 = LoadShader(NULL, "./shaders/shader_0.fs");
    Shader shader_1 = LoadShader(NULL, "./shaders/shader_1.fs");
    Shader shader_2 = LoadShader(NULL, "./shaders/shader_2.fs");
    Shader shader_3 = LoadShader(NULL, "./shaders/shader_3.fs");
    // Shader shader = LoadShader(NULL, "./shaders/fog.fs");
    // Shader shader = LoadShader(NULL, "./shaders/pixelizer.fs");
    // Shader shader = LoadShader(NULL, "./shaders/grayscale.fs");
    // Shader shader = LoadShader(NULL, NULL);

    // ==== SET UNIFORM VALUE ====

    // Variable that are passed into the shader
    Vector2 resolution = { GetScreenWidth(), GetScreenHeight() };
    float seconds = 2.0f;
    Vector2 mouse = { 0 };

    // The shader uniform location
    int resolutionLoc = GetShaderLocation(shader_0, "resolution");
    int secondsLoc = GetShaderLocation(shader_0, "seconds");
    int mouseLoc = GetShaderLocation(shader_0, "mouse");
    int bandsLoc = GetShaderLocation(shader_0, "bands");
    printf("\n\n");
    printf("resolution shader location: %d\n", resolutionLoc);
    printf("seconds shader location: %d\n", secondsLoc);
    printf("mouse shader location: %d\n", mouseLoc);
    printf("bands shader location: %d\n", bandsLoc);
    printf("\n\n");
    // ============================

    Game game = {
        .rect = rect,
        .target = target,
        .texture = texture,
        .src = src,
        .screenWidth = screenWidth,
        .screenHeight = screenHeight,
        .shader_0 = shader_0,
        .shader_1 = shader_1
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
        seconds += GetFrameTime();
        mouse = GetMousePosition();
        printf("%.2f secs, sinf() =%.2f\n", seconds, sinf(seconds));
        SetShaderValue(shader_0, resolutionLoc, &resolution, SHADER_UNIFORM_VEC2);  // Can I set this once?
        SetShaderValue(shader_0, secondsLoc, &seconds, SHADER_UNIFORM_FLOAT);
        SetShaderValue(shader_0, mouseLoc, &mouse, SHADER_UNIFORM_VEC2);
        // SetShaderValue(shader_0, secondsLoc, &seconds, SHADER_UNIFORM_FLOAT);

        // collision detection
        KeepRectOnScreen(&rect);
        
        
        // draw
        BeginDrawing();
            ClearBackground(WHITE);

            // Shader stuff - Texture height must be flipped
            BasicShader(&game);
            // IntermediateShader(&game);

            // Normal drawing
            DrawTexturePro(texture, src, rect, (Vector2){ 0, 0 }, 0, WHITE);
            DrawFPS(20, 20);
        EndDrawing();
    }

    printf("-------------------\n");
    printf("Destroy\n");
    printf("-------------------\n");

    UnloadTexture(texture);
    UnloadShader(shader_0);
    UnloadShader(shader_1);
    UnloadShader(shader_2);
    UnloadShader(shader_3);
    UnloadRenderTexture(target);
    CloseWindow();



    return 0;
}
