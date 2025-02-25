#include <stdio.h>
#include <stdbool.h>
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
typedef struct {
    Music music;
    float timePlayed;
} Game;


// Cirular buffer

// ( L + BLANK + R ) * SAMPLE_RATE
// Frame global_frames[(314 + 198 + 314) * 24] = { 0 };

// 2 seconds worth of data (44100 * 2)
Frame global_frames[44100 * 2] = { 0 };

// Number of frames added to global frames array
size_t global_frames_count = 0;


void update_global_frames_callback(void *buffer, unsigned int frames) {
    // printf("---\n");
    // printf("ARRAY_LEN(frames) %ld\n", ARRAY_LEN(global_frames));

    size_t capacity = ARRAY_LEN(global_frames);

    bool global_frames_not_full = frames <= capacity - global_frames_count;
    bool chunk_to_copy_less_than_global_frames = frames <= capacity;

    if (global_frames_not_full) {

        // There exists space in the array for the section of data
        // Start filling the window (global_frames) with data
        memcpy(global_frames + global_frames_count, buffer, sizeof(Frame) * frames);
        global_frames_count += frames;

        // printf("add\n");

    } else if (chunk_to_copy_less_than_global_frames) {

        // Window is full -> chunk < array.length

        // Ran out of capacity
        // Frames overflow
        // Shift array to the front, copy the remaining data

        // Move remaining section to the front of the buffer
        memmove(global_frames, global_frames + frames, sizeof(Frame) * (capacity - frames));
        // Copy the rest of the data to the end
        memcpy(global_frames + (capacity - frames), buffer, sizeof(Frame) * frames);

        // printf("copy\n");

    } else {

        // Window is full -> chunk > array.length

        // The frames you are trying to copy over is too much
        // Truncate the end of the buffer data

        // Copy just the full aray
        memcpy(global_frames, buffer, sizeof(Frame) * capacity);
        // Cap the capcity
        global_frames_count = capacity;

        // printf("fill\n");

    }
}

void debug_callback(void *buffer, unsigned int frames) {
    // [8, 8, 8, 8, ...]
    float *samples = (float *)buffer;
    // [[4,4], [4,4], ...]
    float l = samples[0];
    float r = samples[1];
    printf("Left %.2f\n", l);
    printf("Right %.2f\n\n", r);
}

void LogMusicData(Music music) {
    const float TOTAL_LENGTH = GetMusicTimeLength(music);   // framesCount / sampleRate

    printf("\n\n");
    printf("Length of song in seconds: %f\n", TOTAL_LENGTH);
    printf("music.frameCount: %u\n", music.frameCount);
    printf("music.looping: %d\n", music.looping);
    printf("music.stream.sampleRate: %d\n", music.stream.sampleRate);
    printf("music.stream.sampleSize: %d\n", music.stream.sampleSize);
    printf("music.stream.channels: %d\n", music.stream.channels);
    assert(music.stream.sampleSize == 32);
    assert(music.stream.channels == 2);
    printf("samepleSize * channels (L + R): %u", music.stream.sampleSize * music.stream.channels);
    printf("\n\n");
}

void MoreLogging() {
    printf("Frame: %ld bytes\n", sizeof(Frame));
    printf("\nglobal_frames\n");
    printf("---------------\n");
    printf("%ld elements\n", ARRAY_LEN(global_frames));
    printf("%ld bytes\n", sizeof(global_frames));
}

void HandleInput(Game *game) {
    Music music = game->music;
    float timePlayed = game->timePlayed;

    if (IsKeyDown(KEY_RIGHT)) {
        UpdateMusicStream(music);   // Update music buffer with new stream data
    }
    if (IsKeyPressed(KEY_R)) {
        printf("restart music\n");
        StopMusicStream(music);
        PlayMusicStream(music);
    }
    if (IsKeyPressed(KEY_S)) {
        printf("stop music\n");
        StopMusicStream(music);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        printf("seek to 30 secs\n");
        timePlayed = 0;
        SeekMusicStream(music, 30);
    }
    if (IsKeyPressed(KEY_P)) {
        if (IsMusicStreamPlaying(music)) {
            printf("pause music\n");
            PauseMusicStream(music);
        } else {
            printf("resume music\n");
            ResumeMusicStream(music);
        }
    }
}

void Update(Game *game) {
    Music music = game->music;
    float timePlayed = game->timePlayed;

    // Get normalized time played for current music stream
    timePlayed = GetMusicTimePlayed(music) / GetMusicTimeLength(music);
    if (timePlayed > 1.0f) timePlayed = 1.0f;   // Make sure time played is no longer than music
}

void Draw(Game *game) {
    Music music = game->music;
    float timePlayed = game->timePlayed;

    // Draw main
    int w = GetScreenWidth();
    int h = GetScreenHeight();

    // float cell_width = (float) w / (float) 1000;
    float cell_width = (float) w / (float) global_frames_count;
    // printf("global_frames_count: %ld\n", global_frames_count);

    for (size_t i  = 0; i < global_frames_count; ++i) {
        float left = global_frames[i].left;
        // float right = global_frames[i].right;
        if (left > 0) {
            DrawRectangle(i * cell_width, h/2 - h/2*left, 1, h/2 * left, RED);
        } else {
            DrawRectangle(i * cell_width, h/2, 1, -h/2 * left, YELLOW);
        }
    }

    // Draw music progress percentage
    // DrawRectangle(musicPlayer.x, musicPlayer.y, musicPlayer.width, musicPlayer.height, RAYWHITE);
    // DrawRectangle(musicPlayer.x, musicPlayer.y, (int)(timePlayed * musicPlayer.width), musicPlayer.height, MAROON);
    // DrawRectangleLines(musicPlayer.x, musicPlayer.y, musicPlayer.width, musicPlayer.height, GRAY);

    // Draw help stuff
    // left section
    const float TEXT_SIZE = 30;
    DrawText(TextFormat("%.2f", timePlayed * 100), 20, 20, TEXT_SIZE, WHITE);

    // right section
    const char* FRAME_COUNT = "Global frame count: ";
    const char* TOTAL_FRAMES = "Elements: ";
    DrawText(TextFormat("%s%d", FRAME_COUNT, music.frameCount),
        GetScreenWidth() - MeasureText(FRAME_COUNT, TEXT_SIZE) - MeasureText(TextFormat("%d", music.frameCount), TEXT_SIZE)  - 20,
        20, TEXT_SIZE, WHITE);
    DrawText(TextFormat("%s%d", TOTAL_FRAMES, ARRAY_LEN(global_frames)),
        GetScreenWidth() - MeasureText(TOTAL_FRAMES, TEXT_SIZE) - MeasureText(TextFormat("%d", ARRAY_LEN(global_frames)), TEXT_SIZE) - 20,
        20 + 50,
        TEXT_SIZE,
        WHITE);
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
    // LogMusicData(music);

    PlayMusicStream(music);
    SetMusicVolume(music, 0.2f);
    AttachAudioStreamProcessor(music.stream, update_global_frames_callback);
    // AttachAudioStreamProcessor(music.stream, debug_callback);

    float timePlayed = 0.0f;        // Time played normalized [0.0f..1.0f]
    bool pause = false;             // Music playing paused
    Rectangle musicPlayer = { 0, 0, GetScreenWidth() * .7, 100 };
    musicPlayer.x = (GetScreenWidth() - musicPlayer.width) / 2;
    musicPlayer.y = (GetScreenHeight() - musicPlayer.height) / 2;

    Game game = {
        .music = music,
        .timePlayed = timePlayed
    };

    // Set our game to run at 60 frames-per-second
    SetTargetFPS(60);               
    
    printf("-------------------\n");
    printf("GAME\n");
    printf("-------------------\n");
    // MoreLogging();

    while(!WindowShouldClose()) {

        // input
        HandleInput(&game);

        // update
        Update(&game);

        // draw
        BeginDrawing();
        ClearBackground(BLACK);

        Draw(&game);

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
