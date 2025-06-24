#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

#define NO_OF_RECTS 3
#define NO_OF_SPACES (NO_OF_RECTS + 1)
#define RECT_WIDTH 100
#define RECT_HEIGHT 100

#define TOTAL_WIDTH RECT_WIDTH * NO_OF_RECTS

int main() {

    InitWindow(800, 600, "Gaps");

    // Position based
    Vector2 center = { GetScreenWidth() / 2, GetScreenHeight() / 2 };
    const int GAP = (GetScreenWidth() - (TOTAL_WIDTH)) / NO_OF_SPACES;

    // Lerp based
    int RADIUS = 30;
    Vector2 A = { 100, 100 };
    Vector2 B = { GetScreenWidth() - 100, 100 };
    float t = 0;

    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        // Handle Input

        // Update

        // Collision

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw using gaps
        int y = center.y - RECT_HEIGHT / 2;
        for (int i = 0; i < NO_OF_RECTS; ++i) {
            int x = i * RECT_WIDTH + GAP * (i + 1);
            // DrawCircle(radius + i * radius * 2, center.y, radius, YELLOW);
            DrawRectangle(x, y, RECT_WIDTH, RECT_HEIGHT, BLUE);
            DrawRectangleLines(x, y, RECT_WIDTH, RECT_HEIGHT, WHITE);
        }

        // Draw using lerp
        DrawCircle(A.x, A.y, RADIUS, WHITE);
        DrawCircle(B.x, B.y, RADIUS, WHITE);
        for (int i = 0; i < NO_OF_RECTS; ++i) {

            // Lerp

            // 1
            // t = fabs((int)GetTime() - GetTime());   // 0.0 -> 1.0

            // 2
            // t += 0.002;
            // if (t >= 1) t = 0;

            // 3
            t = (sin(GetTime()) + 1 ) / 2;
            float x = Lerp(A.x, B.x, (float) i / (NO_OF_RECTS-1));

            DrawText(TextFormat("t = %.2f", t), 20, 20, 20, WHITE);
            DrawCircle(x, B.y, RADIUS, RED);
            DrawCircle(Lerp(A.x, B.x, t), B.y, RADIUS, YELLOW);

        }

        EndDrawing();
    }


    CloseWindow();
}
