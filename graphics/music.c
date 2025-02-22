#include <stdio.h>
#include <stdint.h>
// #include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "raylib.h"

#define ARRAY_LEN(xs) sizeof(xs) / sizeof(xs[0])

uint64_t global_frames[4410] = { 0 };
size_t global_frames_count = 0;

void callback(void *bufferData, unsigned int frames) {
    printf("frames: %u\n", frames);
    // printf("ARRAY_LEN(frames) %ld\n", ARRAY_LEN(global_frames));

    size_t capacity = ARRAY_LEN(global_frames); // 1024

    // Can fit chunk into remaining space...
    if (frames <= capacity - global_frames_count) {
        memcpy(global_frames + global_frames_count, bufferData, sizeof(uint64_t) * frames);
        global_frames_count += frames;
    } else if (frames <= capacity) {
        // frames overflow
        memmove(global_frames, global_frames + sizeof(uint64_t) * frames, sizeof(uint64_t) * (capacity - frames));
        memcmp(global_frames + (capacity - frames), bufferData, sizeof(uint64_t) * frames);
    } else {
        // everything is full
        // you can't really do stuff
        memcpy(global_frames, bufferData, sizeof(uint64_t) * capacity);
        global_frames_count = capacity;
    }

    // ===========
    // if (frames > ARRAY_LEN(global_frames)) {
    //     frames = ARRAY_LEN(global_frames);
    // }

    // memcpy(global_frames, bufferData, sizeof(uint64_t) * frames);
    // global_frames_count = frames;
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
    printf("\n\n");
    printf("music.frameCount: %u\n", music.frameCount);
    printf("music.looping: %d\n", music.looping);
    printf("music.stream.sampleRate: %d\n", music.stream.sampleRate);
    printf("music.stream.sampleSize: %d\n", music.stream.sampleSize);
    printf("music.stream.channels: %d\n", music.stream.channels);
    assert(music.stream.sampleSize == 32);
    assert(music.stream.channels == 2);
    printf("samepleSize * channels (L + R): %u", music.stream.sampleSize * music.stream.channels);
    printf("\n\n");
    PlayMusicStream(music);
    SetMusicVolume(music, 0.2f);
    AttachAudioStreamProcessor(music.stream, callback);


    float timePlayed = 0.0f;        // Time played normalized [0.0f..1.0f]
    bool pause = false;             // Music playing paused
    const float TOTAL_LENGTH = GetMusicTimeLength(music);
    // float timePlayed = GetMusicTimePlayed(music);
    Rectangle musicPlayer = { 0, 0, GetScreenWidth() * .7, 100 };
    musicPlayer.x = (GetScreenWidth() - musicPlayer.width) / 2;
    musicPlayer.y = (GetScreenHeight() - musicPlayer.height) / 2;



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

        // Draw main
        int w = GetScreenWidth();
        int h = GetScreenHeight();
        float cell_width = (float) w / global_frames_count;
        printf("global_frames_count: %ld\n", global_frames_count);

        for (size_t i  = 0; i < global_frames_count; ++i) {
            // Grab 1 channel (L)
            int32_t sample = *(int32_t*) &global_frames[i];

            printf("cell_w: %f\n", cell_width);
            float x = i * cell_width;
            float y = h / 2;
            float max_height = h * 0.8;
            if (sample > 0) {
                float t = (float) sample / INT32_MAX;
                DrawRectangle(x, y - max_height * t, cell_width, max_height * t, RED);
            } else {
                float t = (float) sample / INT32_MIN;
                DrawRectangle(x, y, cell_width, max_height * t, RED);
            }
        }

        // Draw music rect
        // DrawRectangle(musicPlayer.x, musicPlayer.y, musicPlayer.width, musicPlayer.height, RAYWHITE);
        // DrawRectangle(musicPlayer.x, musicPlayer.y, (int)(timePlayed * musicPlayer.width), musicPlayer.height, MAROON);
        // DrawRectangleLines(musicPlayer.x, musicPlayer.y, musicPlayer.width, musicPlayer.height, GRAY);

        // Draw music progress percentage
        DrawText(TextFormat("%.2f", timePlayed * 100), 20, 20, 30, WHITE);
        DrawText(TextFormat("%d", global_frames_count), GetScreenWidth() - 80, 20, 30, WHITE);

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
