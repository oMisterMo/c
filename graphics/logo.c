#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>

#define MAX_TOUCH_POINTS 10
#define MAX_GESTURE_STRINGS   20

int main() {


    SetTraceLogLevel(LOG_WARNING);

    int screenWidth = 800 * 2;
    int screenHeight = 400 * 2;

    // Before window creation
    // FLAG_VSYNC_HINT | FLAG_FULLSCREEN_MODE | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_UNDECORATED
    printf("---------setcon-------------\n");
    SetConfigFlags( FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_UNDECORATED );
    printf("---------initwin------------\n");
    InitWindow(screenWidth, screenHeight, "Logo");
    printf("---------setwinmin----------\n");
    SetWindowMinSize(320, 240);
    printf("---------rest---------------\n");
    //After window creation

    // Texture2D logo = LoadTexture("resources/logo/raylib_logo_animation.gif");
    int frameCounter = 0;
    Texture2D logo = LoadTexture("resources/logo/raylib_256x256.png");
    Rectangle logoPos = { (GetScreenWidth() - logo.width ) / 2, (GetScreenHeight() - logo.height ) / 2, logo.width, logo.height };
    bool logoTouched = false;

    Vector2 touchPosition = { 0, 0 };
    Rectangle touchArea = { 220, 10, screenWidth - 230.0f, screenHeight - 20.0f };

    int gesturesCount = 0;
    char gestureStrings[MAX_GESTURE_STRINGS][32];

    int currentGesture = GESTURE_NONE;
    int lastGesture = GESTURE_NONE;


    printf("----------------------------\n");
    TraceLog(LOG_DEBUG, "logo texture: w %d, h %d", logo.width, logo.height);
    TraceLog(LOG_DEBUG, "logo texture: w %d, h %d", logo.width, logo.height);
    printf("----------------------------\n");

    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        frameCounter++;


        // Handle input
        if (IsKeyPressed(KEY_F)) ToggleFullscreen();
        if (IsKeyPressed(KEY_S)) TakeScreenshot("img.png");

        lastGesture = currentGesture;
        currentGesture = GetGestureDetected();
        touchPosition = GetTouchPosition(0);

        if (CheckCollisionPointRec(touchPosition, touchArea) && (currentGesture != GESTURE_NONE))
        {
            if (currentGesture != lastGesture)
            {
                // Store gesture string
                switch (currentGesture)
                {
                    case GESTURE_TAP: TextCopy(gestureStrings[gesturesCount], "GESTURE TAP"); break;
                    case GESTURE_DOUBLETAP: TextCopy(gestureStrings[gesturesCount], "GESTURE DOUBLETAP"); break;
                    case GESTURE_HOLD: TextCopy(gestureStrings[gesturesCount], "GESTURE HOLD"); break;
                    case GESTURE_DRAG: TextCopy(gestureStrings[gesturesCount], "GESTURE DRAG"); break;
                    case GESTURE_SWIPE_RIGHT: TextCopy(gestureStrings[gesturesCount], "GESTURE SWIPE RIGHT"); break;
                    case GESTURE_SWIPE_LEFT: TextCopy(gestureStrings[gesturesCount], "GESTURE SWIPE LEFT"); break;
                    case GESTURE_SWIPE_UP: TextCopy(gestureStrings[gesturesCount], "GESTURE SWIPE UP"); break;
                    case GESTURE_SWIPE_DOWN: TextCopy(gestureStrings[gesturesCount], "GESTURE SWIPE DOWN"); break;
                    case GESTURE_PINCH_IN: TextCopy(gestureStrings[gesturesCount], "GESTURE PINCH IN"); break;
                    case GESTURE_PINCH_OUT: TextCopy(gestureStrings[gesturesCount], "GESTURE PINCH OUT"); break;
                    default: break;
                }

                gesturesCount++;

                // Reset gestures strings
                if (gesturesCount >= MAX_GESTURE_STRINGS)
                {
                    for (int i = 0; i < MAX_GESTURE_STRINGS; i++) TextCopy(gestureStrings[i], "\0");

                    gesturesCount = 0;
                }
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(touchPosition, logoPos)) {
                logoTouched = true;
            }
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (logoTouched) {
                logoPos.x = touchPosition.x - logoPos.width / 2;
                logoPos.y = touchPosition.y - logoPos.height / 2;
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            logoTouched = false;

            if (logoPos.x  <= 0) {
                logoPos.x = 0;
            }
            if (logoPos.x + logoPos.width > GetScreenWidth()) {
                logoPos.x =  GetScreenWidth() - logoPos.width;
            }
            if (logoPos.y  <= 0) {
                logoPos.y = 0;
            }
            if (logoPos.y + logoPos.height > GetScreenHeight()) {
                logoPos.y =  GetScreenHeight() - logoPos.height;
            }

        }

        // Update

        // Collision detection




        // Draw
        BeginDrawing();
        ClearBackground(WHITE);

        DrawTexture(logo, logoPos.x, logoPos.y, WHITE);
        DrawFPS(20, 20);


        // if (currentGesture != GESTURE_NONE) DrawCircleV(touchPosition, 60, MAROON);

        EndDrawing();

    }

    UnloadTexture(logo);

    CloseWindow();

    return 0;
}
