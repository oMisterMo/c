#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <raylib.h>

#define WIDTH 800
#define HEIGHT 800

int main() {
    InitWindow(WIDTH, HEIGHT, "Bezier");
    SetTargetFPS(60);

    float offsetY = 200.0f;

    Vector2 linear[] = {
        (Vector2) {0,0 + offsetY},
        (Vector2) {WIDTH,HEIGHT + offsetY},
    };

    Vector2 cp = { WIDTH/2, HEIGHT/2};
    bool dragging = false;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // DrawLine(WIDTH/2, 0, WIDTH/2, HEIGHT, BLACK);  // Y-axis
        // DrawLine(0, HEIGHT/2, WIDTH, HEIGHT/2, BLACK);  // X-axis

        if (CheckCollisionPointCircle(GetMousePosition(), cp, 10)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                dragging = true;
            }
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                dragging = true;
            }
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                dragging = false;
            }
        } else {
            // dragging = false;
        }

        // Quick reset position
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            cp = GetMousePosition();
        }

        if (dragging) {
            cp = GetMousePosition();
        }


        DrawLineBezier((Vector2) {0,0}, (Vector2) {WIDTH,HEIGHT}, 10, RED);
        DrawSplineLinear(linear, sizeof(linear) / sizeof(Vector2), 10, BLUE);
        DrawSplineSegmentBezierQuadratic(
            (Vector2) {0,0}, cp, (Vector2) {WIDTH,HEIGHT}, 10, ORANGE);

        if (dragging) {
            DrawCircleV(cp, 10, RED);
        } else {
            DrawCircleV(cp, 10, BLACK);
        }

        DrawFPS(20, 20);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
