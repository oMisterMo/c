#include "raylib.h"
#include <stdio.h>

int main() {

    InitWindow(600, 600, "Intervals");


    Color colors[] = { RED, GREEN, BLUE, ORANGE, PINK, BLACK };
    float spawnInterval = 1.0f; // Every 1 second
    float timeLastSpawn = 0.0;
    int index = 0;


    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        double currentTime = GetTime();

        // Only run after elapsed interval time
        if (currentTime - timeLastSpawn > spawnInterval) {

            int len = (sizeof(colors) / sizeof(colors[0])) - 1;
            index = GetRandomValue(0, len);


            // Reset last spawn
            timeLastSpawn = GetTime();
        }


        BeginDrawing();
        ClearBackground(SKYBLUE);

        DrawText("HELLO", GetScreenWidth() / 2 - 150, GetScreenHeight() / 2, 50, colors[index]);
        DrawRectangle(0, 0, 100, 100, colors[index]);

        EndDrawing();
    }

    CloseWindow();



    return 0;
}

