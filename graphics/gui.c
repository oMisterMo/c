#include <stdio.h>
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

typedef struct GUI {
    bool showMessageBox;
    bool graphics_checkbox;
} GUI;

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

void DrawGUI(GUI *gui) {
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    static int PAD = 24;
    static int GAP = 50;
    float w = 20;float w_small = 20;float w_long = 120;
    float h = 20;
    float x = PAD;
    float y = GetScreenHeight() - h - PAD;

    Rectangle r = { x, y, w, h };
    if (GuiCheckBox(r, "Graphics", &gui->graphics_checkbox));
    r.y -= GAP;
    if (GuiCheckBox(r, "Graphics", &gui->graphics_checkbox));
    r.y -= GAP;
    if (GuiCheckBox(r, "Graphics", &gui->graphics_checkbox));
    r.y -= GAP;
    if (GuiCheckBox(r, "Graphics", &gui->graphics_checkbox));
    r.y -= GAP;
    if (GuiCheckBox(r, "Graphics", &gui->graphics_checkbox));
    r.y -= GAP;
    if (GuiCheckBox(r, "Graphics", &gui->graphics_checkbox));
    r.y -= GAP;



    r.width = w_long;
    if (GuiButton(r, "#191#Show Message")) gui->showMessageBox = true;


    // When opened
    // if (gui->showMessageBox) {
    //     int result = GuiMessageBox((Rectangle){ 85, 70, 250, 100 },
    //         "#191#Message Box", "Hi! This is a message!", "Nice;Cool");
    //     if (result >= 0) gui->showMessageBox = false;
    // }

}

int main(void) {

    InitWindow(900, 600, "GUI");

    Rectangle rect = { 0, 0, 100, 100 };

    // Rectangle, Text title, bool state
    GUI gui = {
        .showMessageBox = false
    };

    SetTargetFPS(60);
    while(!WindowShouldClose()) {

        // handle input
        HandleInput(&rect);

        // update
        rect.x++;

        // draw
        BeginDrawing();
            // ClearBackground(BLACK);
            // DrawGame(rect);
            DrawGUI(&gui);
            DrawFPS(20, 20);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
