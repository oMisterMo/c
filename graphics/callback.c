#include <stdio.h>
#include "raylib.h"


// Who came first, the chicken or the egg...?
// Define function pointer, Define Box
// Both need each other
typedef struct Box Box;
typedef void (*Box_cb)(Box *b);
typedef struct Box {
    int id;
    Rectangle source;
    Color color;
    Texture2D texture;
    Box_cb callback;
} Box;


// void Hello(Box *box, Box_cb cb) {
//     cb(box);
// }

void CenterBox(Box *box) {
    box->source.x = (GetScreenWidth() - box->source.width) / 2;
    box->source.y = (GetScreenHeight() - box->source.height) / 2;
}
void DoTheThing(Box *box) {
    box->color.r = GetRandomValue(0, 255);
    box->color.g = GetRandomValue(0, 255);
    box->color.b = GetRandomValue(0, 255);
    box->color.a = 120;
}

int main(void) {

    printf("Hello world\n");

    InitWindow(960, 720, "Mo");

    Box box = { 0 };
    box.source = (Rectangle) { 100, 100, 300, 200 };
    box.color = PINK;
    box.callback = DoTheThing;

    // Modify the values set
    CenterBox(&box);
    DoTheThing(&box);


    while(!WindowShouldClose()) {

        // input
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(GetMousePosition(), box.source)) {
                printf("HIT\n");
                box.callback(&box);
            }
        }

        // update
        // draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangleRec(box.source, box.color);
        EndDrawing();

    }


    CloseWindow();

    return 0;
}
