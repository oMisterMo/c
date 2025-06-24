#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "reasings.h"

#define NO_OF_ITEMS 6
#define LINE_GAP 100

float easeOutBounce(float x) {
    float n1 = 7.5625;
    float d1 = 2.75;

    if (x < 1 / d1) {
        return n1 * x * x;
    } else if (x < 2 / d1) {
        return n1 * (x -= 1.5 / d1) * x + 0.75;
    } else if (x < 2.5 / d1) {
        return n1 * (x -= 2.25 / d1) * x + 0.9375;
    } else {
        return n1 * (x -= 2.625 / d1) * x + 0.984375;
    }
}

float easeInOutElastic(float x) {
    float c5 = (2 * PI) / 4.5;

    return x == 0
    ? 0
    : x == 1
    ? 1
    : x < 0.5
    ? -(pow(2, 20 * x - 10) * sin((20 * x - 11.125) * c5)) / 2
    : (pow(2, -20 * x + 10) * sin((20 * x - 11.125) * c5)) / 2 + 1;

}

int main(void) {

    InitWindow(800, 600, "Lerp");

    int RADIUS = 30;
    Vector2 A = { 100, 100 };
    Vector2 B = { GetScreenWidth() - 100, 100 };
    float t = 0;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        // input
        // update
        // draw
        BeginDrawing();
            ClearBackground(BLACK);

            DrawCircle(A.x, A.y, RADIUS, WHITE);
            DrawCircle(B.x, B.y, RADIUS, WHITE);
            for (int i = 0; i < NO_OF_ITEMS; ++i) {

                // Lerp

                // 1
                // t = fabs((int)GetTime() - GetTime());   // 0.0 -> 1.0

                // 2
                // t += 0.002;
                // if (t >= 1.0f) t = 0.0f;

                // 3
                t = (sin(GetTime()) + 1 ) / 2;
                
                float x = Lerp(A.x, B.x, (float) i / (NO_OF_ITEMS -1));
                DrawText(TextFormat("t = %.2f", t), 20, 20, 20, WHITE);
                DrawCircle(x, B.y + LINE_GAP, RADIUS, RED);
                DrawCircle(Lerp(A.x, B.x, t), B.y + LINE_GAP * 2, RADIUS, YELLOW);
                DrawCircle(Lerp(A.x, B.x, easeOutBounce(t)), B.y + LINE_GAP * 3, RADIUS, YELLOW);
                DrawCircle(Lerp(A.x, B.x, easeInOutElastic(t)), B.y + LINE_GAP * 4, RADIUS, YELLOW);

            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
