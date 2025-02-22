#include <stdio.h>
#include "raylib.h"

void callback(void *bufferData, unsigned int frames) {
    printf("frames: %u\n", frames);
}

int main(void) {


    printf("-------------------\n");
    printf("Init Window\n");
    printf("-------------------\n");
    InitWindow(900, 600, "Music");

    printf("-------------------\n");
    printf("Init Audio\n");
    printf("-------------------\n");
    InitAudioDevice();              // Initialize audio device
    Music music = LoadMusicStream("resources/music/country.mp3");
    PlayMusicStream(music);
    SetMusicVolume(music, 0.2f);
    AttachAudioStreamProcessor(music.stream, callback);


    float timePlayed = 0.0f;        // Time played normalized [0.0f..1.0f]
    bool pause = false;             // Music playing paused
    const float TOTAL_LENGTH = GetMusicTimeLength(music);
    // float timePlayed = GetMusicTimePlayed(music);



    // Set our game to run at 60 frames-per-second
    SetTargetFPS(60);               
    
    printf("-------------------\n");
    printf("GAME\n");
    printf("-------------------\n");
    printf("Length of song in seconds: %f\n", TOTAL_LENGTH);

    while(!WindowShouldClose()) {

    
        // input
        if (IsKeyDown(KEY_RIGHT)) {
            UpdateMusicStream(music);   // Update music buffer with new stream data
        }
        if (IsKeyDown(KEY_R)) {
            StopMusicStream(music);
            PlayMusicStream(music);
        }
        if (IsKeyDown(KEY_S)) {
            StopMusicStream(music);
        }
        if (IsKeyPressed(KEY_SPACE)) {
            timePlayed = 0;
            SeekMusicStream(music, 30);
        }
        
        
        // update
        // Get normalized time played for current music stream
        timePlayed = GetMusicTimePlayed(music)/GetMusicTimeLength(music);
        if (timePlayed > 1.0f) timePlayed = 1.0f;   // Make sure time played is no longer than music


        
        
        
          // draw
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw music rect
        DrawRectangle(200, 200, 400, 12, RAYWHITE);
        DrawRectangle(200, 200, (int)(timePlayed*400.0f), 12, MAROON);
        DrawRectangleLines(200, 200, 400, 12, GRAY);

        // Draw music progress percentage
        DrawText(TextFormat("%.2f", timePlayed * 100), 20, 20, 30, WHITE);

        // Draw help

        EndDrawing();

    }


    printf("-------------------\n");
    printf("Destroy\n");
    printf("-------------------\n");

    UnloadMusicStream(music);   // Unload music stream buffers from RAM
    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)
    CloseWindow();


    return 0;
}
