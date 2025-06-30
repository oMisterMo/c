#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "raylib.h"

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

    // Bunch of random lines
    Vector2 linear[] = {
        (Vector2) {0, offsetY},
        (Vector2) {WIDTH/2,HEIGHT},
        (Vector2) {WIDTH, offsetY},
    };

    // Line start / end position and control point
    Vector2 start = { 0,0 };
    Vector2 end = { WIDTH,HEIGHT };
    ControlPoint controlPoint = {
        .pos = { WIDTH/2, HEIGHT/2},
        .isDragging = false,
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // DrawLine(WIDTH/2, 0, WIDTH/2, HEIGHT, BLACK);  // Y-axis
        // DrawLine(0, HEIGHT/2, WIDTH, HEIGHT/2, BLACK);  // X-axis

        if (CheckCollisionPointCircle(GetMousePosition(), controlPoint.pos, 10)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                controlPoint.isDragging = true;
            }
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                controlPoint.isDragging = true;
            }
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                controlPoint.isDragging = false;
            }
        }

        // Quick reset position
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            controlPoint.pos = GetMousePosition();
        }

        if (controlPoint.isDragging) {
            controlPoint.pos = GetMousePosition();
        }

        // Draw contol point - including hover color if dragging
        if (controlPoint.isDragging) {
            DrawCircleV(controlPoint.pos, 10, RED);
        } else {
            DrawCircleV(controlPoint.pos, 10, BLACK);
        }
        // DrawCircleV(controlPoint.pos, 10, controlPoint.isDragging ? RED : BLACK);


        // Default curve
        DrawLineBezier((Vector2) {0,0}, (Vector2) {WIDTH,HEIGHT}, 10, RED);
        // Multiple lines
        DrawSplineLinear(linear, sizeof(linear) / sizeof(Vector2), 10, BLUE);
        // Actual bezier
        DrawSplineSegmentBezierQuadratic(start, controlPoint.pos, end, 10, ORANGE);

        // DrawSplineSegmentBezierQuadratic(Vector2 p1, Vector2 c2, Vector2 p3, float thick, Color color);              // 1 control point
        // DrawSplineSegmentBezierCubic(Vector2 p1, Vector2 c2, Vector2 c3, Vector2 p4, float thick, Color color);      // 2 control points



        DrawFPS(20, 20);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
