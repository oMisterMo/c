#include <stdio.h>
#include <math.h>

#include <raylib.h>

// GRAPH
#define WIDTH 800
#define HEIGHT 800
#define SCALE 160        // wider view
#define THRESHOLD 0.01   // smoother edges

// DRAWING
#define RADIUS 5
#define INCREMENT 5.0f
#define DRAW_PIXEL false

// Function to evaluate the equation at (x, y)
float Equation(float x, float y) {
    return (3.0f * x*x - 2.0f * x*y + y*y - 1.0f);
}

int CountPathPosition() {
    int count = 0;
    int hits = 0;
    
    for (float px = 0; px < WIDTH; px += INCREMENT) {
        for (float py = 0; py < HEIGHT; py += INCREMENT) {
            float x = (px - WIDTH / 2) / SCALE;
            float y = (HEIGHT / 2 - py) / SCALE;
            count++;
            
            if (fabs(Equation(x, y)) < THRESHOLD) {
                hits++;
            }
        }
    }

    printf("==============\n");
    printf("total (iterations): %d\n", count);
    printf("total (hits): %d\n", hits);
    printf("==============\n");
    return hits;
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Implicit Graph: 3x^2 - 2xy + y^2 = 1");
    SetTargetFPS(60);

    int PATH_COUNT = CountPathPosition();
    Vector2 paths[PATH_COUNT];


    printf("size of Vector2: %ld\n", sizeof(Vector2));
    printf("size of paths: %ld\n", sizeof(paths));


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawLine(WIDTH/2, 0, WIDTH/2, HEIGHT, BLACK);  // Y-axis
        DrawLine(0, HEIGHT/2, WIDTH, HEIGHT/2, BLACK);  // X-axis

        // Plot equation (naive)
        for (float px = 0; px < WIDTH; px+=INCREMENT) {
            for (float py = 0; py < HEIGHT; py+=INCREMENT) {
                float x = (px - WIDTH / 2) / SCALE;
                float y = (HEIGHT / 2 - py) / SCALE;

                if (fabs(Equation(x, y)) < THRESHOLD) {
                    if (DRAW_PIXEL) {
                        DrawPixel(px, py, BLUE);
                    } else {
                        DrawCircle(px, py, RADIUS, BLUE);
                    }
                }
            }
        }

        DrawFPS(20, 20);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
