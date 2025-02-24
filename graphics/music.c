#include <stdio.h>
#include <stdint.h>
// #include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "raylib.h"

#define ARRAY_LEN(xs) sizeof(xs) / sizeof(xs[0])

typedef struct {
    float left;
    float right;
} Frame;


// Cirular buffer

// ( L + BLANK + R ) * SAMPLE_RATE
Frame global_frames[(314 + 198 + 314) * 1] = { 0 };

// Number of frames added to global frames array
size_t global_frames_count = 0;


void callback(void *bufferData, unsigned int frames) {
    // printf("---\n");
    // printf("ARRAY_LEN(frames) %ld\n", ARRAY_LEN(global_frames));

    // bufferData = 8 bytes
    // L = FIRST CHUNK + EMPTY
    // R = SECOND CHUNK

    size_t capacity = ARRAY_LEN(global_frames);

    if (frames <= capacity - global_frames_count) {
        // There exists space in the array for the section of data
        // Start filling the window (global_frames) with data
        memcpy(global_frames + global_frames_count, bufferData, sizeof(Frame) * frames);
        global_frames_count += frames;
        printf("add\n");
        printf("+%ld\n", global_frames_count);
    } else if (frames <= capacity) {
        // Ran out of capacity
        // Frames overflow
        // Shift array to the front, copy the remaining data

        // Move remaining section to the front of the buffer
        memmove(global_frames, global_frames + frames, sizeof(Frame) * (capacity - frames));
        // Copy the rest of the data to the end
        memcpy(global_frames + (capacity - frames), bufferData, sizeof(Frame) * frames);
        printf("shift\n");
    } else {
        // The frames you are trying to copy over is too much
        // Truncate the end of the buffer data
        // Copy just the full aray
        memcpy(global_frames, bufferData, sizeof(Frame) * capacity);
        global_frames_count = capacity;
        printf("overflow\n");
    }
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
    // Music music = LoadMusicStream("resources/music/Monsters - Conner Youngblood.mp3");
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
    printf("Frame: %ld bytes\n", sizeof(Frame));
    printf("Number of frames: %ld\n", ARRAY_LEN(global_frames));
    printf("global_frames: %ld bytes\n", sizeof(global_frames));

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

        // float cell_width = (float) w / (float) 1000;
        float cell_width = (float) w / (float) global_frames_count;
        // printf("global_frames_count: %ld\n", global_frames_count);

        // for (size_t i  = 0; i < global_frames_count; ++i) {
        // for (size_t i  = 0; i < 314; ++i) {
        //     float t = global_frames[i].left;
        //     if (t > 0) {
        //         DrawRectangle(i * cell_width, h/2 - h/2*t, 1, h/2*t, RED);
        //     } else {
        //         DrawRectangle(i * cell_width, h/2, 1, -h/2*t, YELLOW);
        //     }
        // }
        // for (size_t i  = 314; i < 314 + 198; ++i) {
        //     float t = global_frames[i].left;
        //     DrawCircle(i * cell_width, h/2, 1, BLUE);
        //     // if (t > 0) {
        //     //     DrawRectangle(i * cell_width, h/2 - h/2*t, 1, h/2*t, RED);
        //     // } else {
        //     //     DrawRectangle(i * cell_width, h/2, 1, -h/2*t, YELLOW);
        //     // }
        // }
        // for (size_t i  = 314 + 198; i < global_frames_count; ++i) {
        //     float t = global_frames[i].left;
        //     if (t > 0) {
        //         DrawRectangle(i * cell_width, h/2 - h/2*t, 1, h/2*t, RED);
        //     } else {
        //         DrawRectangle(i * cell_width, h/2, 1, -h/2*t, YELLOW);
        //     }
        // }

        for (size_t i  = 0; i < global_frames_count; ++i) {
            float t = global_frames[i].left;
            if (t > 0) {
                DrawRectangle(i * cell_width, h/2 - h/2*t, 1, h/2*t, RED);
            } else {
                DrawRectangle(i * cell_width, h/2, 1, -h/2*t, YELLOW);
            }
        }

        // for (size_t i  = 0; i < global_frames_count; ++i) {
        //     // Grab 1 channel (L)
        //     float left = global_frames[i].left;
        //     float right = global_frames[i].right;

        //     // printf("cell_w: %f\n", cell_width);
        //     float x = i * cell_width;
        //     float y = h / 2;
        //     float max_height = h * 0.5;
        //     if (left) {
        //         DrawRectangle(x, y - max_height * left, 1, max_height * left, RED);
        //     }
        //     if (right) {
        //         // DrawRectangle(x, y, 1, max_height * right, RED);
        //     }
        // }

        // Draw music rect
        // DrawRectangle(musicPlayer.x, musicPlayer.y, musicPlayer.width, musicPlayer.height, RAYWHITE);
        // DrawRectangle(musicPlayer.x, musicPlayer.y, (int)(timePlayed * musicPlayer.width), musicPlayer.height, MAROON);
        // DrawRectangleLines(musicPlayer.x, musicPlayer.y, musicPlayer.width, musicPlayer.height, GRAY);

        // Draw music progress percentage
        DrawText(TextFormat("%.2f", timePlayed * 100), 20, 20, 30, WHITE);
        DrawText(TextFormat("%d", global_frames_count), GetScreenWidth() - 130, 20, 30, WHITE);

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
