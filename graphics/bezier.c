#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <raylib.h>

#define WIDTH 800
#define HEIGHT 800

typedef struct ControlPoint {
    Vector2 pos;
    bool isDragging;
} ControlPoint;

int main() {
    InitWindow(WIDTH, HEIGHT, "Bezier");
    SetTargetFPS(60);

    float offsetY = 200.0f;

    Vector2 linear[] = {
        (Vector2) {0,0 + offsetY},
        (Vector2) {WIDTH,HEIGHT + offsetY},
    };

    ControlPoint cp1 = {
        .pos = { WIDTH/2, HEIGHT/2},
        .isDragging = false,
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // DrawLine(WIDTH/2, 0, WIDTH/2, HEIGHT, BLACK);  // Y-axis
        // DrawLine(0, HEIGHT/2, WIDTH, HEIGHT/2, BLACK);  // X-axis

        if (CheckCollisionPointCircle(GetMousePosition(), cp1.pos, 10)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                cp1.isDragging = true;
            }
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                cp1.isDragging = true;
            }
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                cp1.isDragging = false;
            }
        } else {
            // cp1.isDragging = false;
        }

        // Quick reset position
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            cp1.pos = GetMousePosition();
        }

        if (cp1.isDragging) {
            cp1.pos = GetMousePosition();
        }


        DrawLineBezier((Vector2) {0,0}, (Vector2) {WIDTH,HEIGHT}, 10, RED);
        DrawSplineLinear(linear, sizeof(linear) / sizeof(Vector2), 10, BLUE);
        DrawSplineSegmentBezierQuadratic(
            (Vector2) {0,0}, cp1.pos, (Vector2) {WIDTH,HEIGHT}, 10, ORANGE);

        if (cp1.isDragging) {
            DrawCircleV(cp1.pos, 10, RED);
        } else {
            DrawCircleV(cp1.pos, 10, BLACK);
        }

        DrawFPS(20, 20);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
