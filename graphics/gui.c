#include <stdio.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

void HandleInput(Rectangle *rect) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), *rect)){
            printf("touched\n");
            rect->width *= 1.2;
            rect->height *= 1.2;
        }
    }
}
void DrawGame(Rectangle rect) {
    ClearBackground(YELLOW);
    DrawRectangleRec(rect, BLACK);
}

void DrawGUI(bool *showMessageBox) {
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
    if (GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Show Message")) *showMessageBox = true;
    if (*showMessageBox) {
        int result = GuiMessageBox((Rectangle){ 85, 70, 250, 100 },
            "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

        if (result >= 0) *showMessageBox = false;
    }

}

int main(void) {


    InitWindow(900, 600, "GUI");

    Rectangle rect = { 0, 0, 100, 100 };
    bool showMessageBox = false;

    SetTargetFPS(60);
    while(!WindowShouldClose()) {

        // handle input
        HandleInput(&rect);

        // update
        rect.x++;

        // draw
        BeginDrawing();

            DrawGame(rect);
            DrawGUI(&showMessageBox);
            DrawFPS(20, 20);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
