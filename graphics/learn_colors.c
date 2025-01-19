#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>

#define MAX_TOUCH_POINTS 10
#define MAX_GESTURE_STRINGS   20

#define NO_OF_TRAYS 3
#define TRAY_WIDTH 300
#define TRAY_HEIGHT 150
#define MIN_NO_OF_TRAYS 3
#define MAX_NO_OF_TRAYS 8

#define NO_OF_CARDS 4
#define CARD_WIDTH 200
#define CARD_HEIGHT 270

#define GAP 100

struct Card {
    Vector2 originalPosition;
    Rectangle rect;
    Color color;
    bool isDragging;
    bool hasTouchedEndZone;
    bool hasScore;
};

void initCards(struct Card cards[], int startX, int centerX, Color colors[]);
void initTrays(Rectangle trays[], int startX, int centerX);
void reset(int *score);

int main() {

    // Setup config
    int screenWidth = 800 * 2;
    int screenHeight = 400 * 2;
    SetConfigFlags( FLAG_WINDOW_UNDECORATED );
    InitWindow(screenWidth, screenHeight, "Learn Colors");
    SetWindowMonitor(2);


    // Initialization
    Color colors[] = { RED, GREEN, BLUE, ORANGE, PINK, PURPLE, SKYBLUE, GRAY };
    Color original[] = { RED, GREEN, BLUE, ORANGE, PINK, PURPLE, SKYBLUE, GRAY };
    Rectangle trays[NO_OF_TRAYS];
    struct Card cards[NO_OF_CARDS];

    int centerX = GetScreenWidth() / 2;
    int centerY = GetScreenHeight() / 2;
    int startX = -(TRAY_WIDTH * NO_OF_TRAYS) / 2;
    int startY = -(TRAY_HEIGHT) / 2;


    initTrays(trays, startX, centerX);
    initCards(cards, startX, centerX, colors);

    // ================================================
    Vector2 touchPosition = { 0, 0 };
    int gesturesCount = 0;
    int currentGesture = GESTURE_NONE;
    char gestureStrings[MAX_GESTURE_STRINGS][32];
    int lastGesture = GESTURE_NONE;
    // ================================================


    int counter = 0;
    int score = 0;


    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        if (IsKeyPressed(KEY_F)) ToggleFullscreen();
        if (IsKeyPressed(KEY_R)) {
            reset(&score);
            initCards(cards, startX, centerX, colors);
        }

        lastGesture = currentGesture;
        currentGesture = GetGestureDetected();
        touchPosition = GetTouchPosition(0);

        // Handle Tray
        for (int i = 0; i < NO_OF_TRAYS; ++i) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                // colors[i] = original[i];
            }
        }

        // Handle Cards
        for (int i = 0; i < NO_OF_CARDS; ++i) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(touchPosition, cards[i].rect)) {
                    cards[i].isDragging = true;
                }
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (cards[i].isDragging) {
                    cards[i].rect.x = touchPosition.x - cards[i].rect.width / 2;
                    cards[i].rect.y = touchPosition.y - cards[i].rect.height / 2;
                }
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                cards[i].isDragging = false;

                bool hit = false;
                int sum = 0;

                for (int j = 0; j < NO_OF_TRAYS; ++j) {
                    if (CheckCollisionRecs(cards[i].rect, trays[j]) && ColorIsEqual(cards[i].color, colors[j])) {
                        hit = true;
                        ++counter;
                        break;
                    }
                }

                // Increment score or reset card position
                if (hit) {
                    if (cards[i].hasTouchedEndZone) continue;
                    printf("HIT %d\n", counter);
                    if (!cards[i].hasScore) {
                        ++score;
                    }
                    cards[i].hasTouchedEndZone = true;
                    cards[i].hasScore = true;
                } else {
                    printf("Reset Card...\n");
                    cards[i].rect.x = cards[i].originalPosition.x;
                    cards[i].rect.y = cards[i].originalPosition.y; 
                }

                // Have all cards been moved to the correct zone?
                for (int j = 0; j < NO_OF_CARDS; ++j) {
                    sum += cards[j].hasTouchedEndZone;
                }

                // Yes? Reset cards
                if (sum >= NO_OF_CARDS) {
                    initCards(cards, startX, centerX, colors);
                }
                    
            }
        }

        // Collisions
        // for (int i = 0; i < NO_OF_CARDS; ++i) {
        //     for (int j = 0; j < NO_OF_TRAYS; ++j) {
        //         if (CheckCollisionRecs(cards[i].rect, trays[j]) && ColorIsEqual(cards[i].color, colors[j])) {
        //             printf("HIT %d\n", counter);
        //             break;
        //         }
        //     }
        //     counter++;
        // }

        BeginDrawing();
        ClearBackground(RAYWHITE);


        // DrawRectangle(trays[0].rect.x, trays[0].rect.y, trays[0].rect.width, trays[0].rect.height, RED);
        for (int i = 0; i < NO_OF_TRAYS; ++i) {
            DrawRectangleRounded(trays[i], 0.3f, 16, colors[i]);
        }
        for (int i = 0; i < NO_OF_CARDS; ++i) {
            if (cards[i].hasTouchedEndZone) {
                DrawRectangleLines(cards[i].originalPosition.x, cards[i].originalPosition.y, CARD_WIDTH, CARD_HEIGHT, ColorAlpha(GRAY, 0.4f));
            }
            DrawRectangleRoundedLinesEx(cards[i].rect, 0.3f, 16, 5, ColorAlpha(GRAY, 0.4f));
            DrawRectangleRounded(cards[i].rect, 0.3f, 16, cards[i].color);
        }
        DrawText((TextFormat("Score: %d", score)), 20, 20, 30, GRAY);


        EndDrawing();

    }

    CloseWindow();


    return 0;
}

void initTrays(Rectangle trays[], int startX, int centerX) {
    for (int i = 0; i < NO_OF_TRAYS; ++i) {
        trays[i] = (Rectangle) {
            -GAP * (NO_OF_TRAYS - 1) + centerX + startX  + ((i + 1) * GAP) + (TRAY_WIDTH * i),
            GetScreenHeight() - TRAY_HEIGHT - 20,
            TRAY_WIDTH,
            TRAY_HEIGHT
        };
    }
}

void initCards(struct Card cards[], int centerX, int startX, Color colors[]) {
    for (int i = 0; i < NO_OF_CARDS; ++i) {
        Vector2 position = {
            -GAP * (NO_OF_TRAYS - 1) + centerX + startX  + ((i + 1) * GAP) + (CARD_WIDTH * i),
            20
        };
        cards[i].rect = (Rectangle) {
            position.x,
            position.y, 
            CARD_WIDTH,
            CARD_HEIGHT
        };

        cards[i].originalPosition = position;
        cards[i].isDragging = false;
        cards[i].hasTouchedEndZone = false;
        cards[i].hasScore = false;
        cards[i].color = colors[GetRandomValue(0, NO_OF_TRAYS - 1)];
    }
}

void reset(int *score) {
    *score = 0;
}
