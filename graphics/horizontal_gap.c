#include "raylib.h"

#define NO_OF_RECTS 3
#define NO_OF_SPACES (NO_OF_RECTS + 1)
#define RECT_WIDTH 100
#define RECT_HEIGHT 100

#define TOTAL_WIDTH RECT_WIDTH * NO_OF_RECTS

int main() {
    
    
    InitWindow(800, 600, "Gaps");

    Vector2 center = { GetScreenWidth() / 2, GetScreenHeight() / 2 };
    const int GAP = (GetScreenWidth() - (TOTAL_WIDTH)) / NO_OF_SPACES;

    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        // Handle Input

        // Update

        // Collision

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        int y = center.y - RECT_HEIGHT / 2;
        for (int i = 0; i < NO_OF_RECTS; ++i) {

            int x = i * RECT_WIDTH + GAP * (i + 1);
            // DrawCircle(radius + i * radius * 2, center.y, radius, YELLOW);
            DrawRectangle(x, y, RECT_WIDTH, RECT_HEIGHT, BLUE);
            DrawRectangleLines(x, y, RECT_WIDTH, RECT_HEIGHT, WHITE);

        }

        EndDrawing();
    }


    CloseWindow();
}
