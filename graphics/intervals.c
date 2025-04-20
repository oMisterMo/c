#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>

// typedef void (*Function)(void *any, int name, Vector2 position);
typedef void (*Function)();

typedef struct Timer {
    // Public members
    float duration;
    bool repeat;
    bool autostart;
    Function callback;

    // Private members, please no edit
    float _start_time;
    bool _active;
} Timer;

void ActivateTimer(Timer *t) {
    t->_active = true;
    t->_start_time = GetTime();
}

void DeactivateTimer(Timer *t) {
    t->_active = false;
    t->_start_time = 0.0f;
    if (t->repeat) {
        ActivateTimer(t);
    }
}

void InitTimer(Timer *t, float duration, bool repeat, bool autostart, Function callback) {
    t->duration = duration;
    t->repeat = repeat;
    t->callback = callback;

    t-> _start_time = 0.0f;
    t->_active = false;

    if (autostart) {
        printf("autostart timer...\n");
        ActivateTimer(t);
    }
}

void UpdateTimer(Timer *t) {
    if (t->_active) {
        if (GetTime() - t->_start_time >= t->duration) {
            // Reached the time

            printf("\n\n");
            printf("start: %.2f\n", t->_start_time);
            printf(":) %.2f seconds\n", t->duration);
            if (t->callback != NULL) {
                // RUN THE CALLBACK, I DONT KNOW HOW YET.
                t->callback();
            }
            DeactivateTimer(t);
        }
    }
}

void callback() {
    printf("HELLO MO: \n");
}

int main() {

    printf("-------------------\n");
    printf("Init Window\n");
    printf("-------------------\n");
    InitWindow(600, 600, "Intervals");


    printf("-------------------\n");
    printf("Init Variables\n");
    printf("-------------------\n");
    // ATTEMPT 1
    Color colors[] = { RED, GREEN, BLUE, ORANGE, PINK, BLACK };
    float spawnInterval = 1.0f; // Every 1 second
    float timeLastSpawn = 0.0;
    int index = 0;

    // ATTEMPT 2
    printf("sizeof(Timer): %ld\n", sizeof(Timer));
    Timer t = { 0 };
    InitTimer(&t, 1.5f, true, true, callback);

    SetTargetFPS(60);
    printf("-------------------\n");
    printf("GAME\n");
    printf("-------------------\n");

    while (!WindowShouldClose()) {

        // ATTEMPT 1 - Only run after elapsed interval time
        double currentTime = GetTime();
        if (currentTime - timeLastSpawn > spawnInterval) {
            int len = sizeof(colors) / sizeof(colors[0]);
            index = GetRandomValue(0, len - 1);
            // Reset last spawn
            timeLastSpawn = GetTime();
        }

        // ATTEMPT 2
        UpdateTimer(&t);


        BeginDrawing();
        ClearBackground(SKYBLUE);

        DrawText("HELLO", GetScreenWidth() / 2 - 150, GetScreenHeight() / 2, 50, colors[index]);
        DrawRectangle(0, 0, 100, 100, colors[index]);

        EndDrawing();
    }

    CloseWindow();



    return 0;
}

