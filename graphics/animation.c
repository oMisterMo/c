#include "raylib.h"

int main() {

    InitWindow(1600, 768, "Animation");
    SetWindowMonitor(2);

    Texture2D jumpbunny = LoadTexture("resources/sprites/jumpbunny_anim1.png");
    Texture2D slime  = LoadTexture("resources/sprites/slime_green.png");
    slime.width *= 8;
    slime.height *= 8;


    // Game Loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(ORANGE);




        DrawTexture(jumpbunny, 0, 0 , WHITE);
        DrawTexture(slime, 0, jumpbunny.height , WHITE);





        EndDrawing();
    }



    UnloadTexture(jumpbunny);
    UnloadTexture(slime);
    CloseWindow();


    return 0;
}
