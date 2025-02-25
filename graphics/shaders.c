#include <stdio.h>
#include "raylib.h"

void KeepRectOnScreen(Rectangle *rect) {
    if (rect->x > GetScreenWidth()) {
        rect->x = -rect->width;
    }
    if (rect->y > GetScreenHeight()) {
        rect->y = -rect->height;
    }
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


    // printf("\n\nGetShaderLocation %d\n\n", GetShaderLocation(shader, "/shaders/simple"));
    // SetShaderValue(shader, GetShaderLocation(shader, "screenWidth"), &screenWidth, SHADER_UNIFORM_INT);
    // SetShaderValue(shader, 16, &screenHeight, SHADER_UNIFORM_INT);

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

        // 2D camera stuff
        // BeginTextureMode(target);
        //     ClearBackground(RED);
        // EndTextureMode();

        BeginDrawing();
            ClearBackground(GRAY);

            // Shader stuff
            BeginShaderMode(shader);
                // Texture height must be flipped

                // DrawRectangleRec(rect, RED);
                // DrawTexture(target.texture, 0, 0, WHITE);
                DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)screenWidth, (float)-screenHeight }, (Vector2) { 0, 0 }, WHITE);
                // DrawTexturePro(texture, src, rect, (Vector2){ 0, 0 }, 0, WHITE);
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
