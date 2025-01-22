#include "raylib.h"
#include "reasings.h"

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define MAX_TOUCH_POINTS 10
#define MAX_GESTURE_STRINGS   20

#define NO_OF_TRAYS 3
#define NO_OF_CARDS 4
#define NO_OF_CLOUDS 4      // fixed

#define TRAY_WIDTH 250
#define TRAY_HEIGHT 183
#define CARD_WIDTH 200/2      // min size = 150 = size of check texture
#define CARD_HEIGHT 200/2

#define GAP 70              // Space between cards & trays
#define PADDING 70          // Space above & below





#define MIN_NO_OF_TRAYS 2   // unused
#define MAX_NO_OF_TRAYS 8   // unused




// Presentation
#define isDrawBackground true
#define isPrarallaxBackground true
#define isShowCursor true
#define isDrawTray true
#define isDrawCard false
#define isTweenCard true

#define isOff false   // global flag to turn all examples off


enum State {
    Still,
    Tween,
};

struct Card {
    Vector2 originalPosition;   // End or target position, should be consts
    Rectangle rect;
    Color color;
    bool isDragging;
    bool hasTouchedEndZone;
    bool hasScore;

    // tween
    Vector2 currentPosition;    // Tween start position
    int state;                  // Still | Tween
    int frameCounter;           // Current time in tween
    int duration;               // How long to tween
};

void initCards(struct Card cards[], int trayStartX, Color colors[]);
void initTrays(Rectangle trays[], int trayStartX, Texture2D *tray);
void reset(int *score);

void updateCards(struct Card cards[]);

void drawBackground(Texture2D clouds[], double increment, int order[]);
void drawTrays(Rectangle trays[], Texture2D tray, Color colors[]);
void drawCards(struct Card cards[], Texture2D check, Texture2D border);
void drawCursor(Texture2D cursor, Texture2D cursorPressed);
void drawScore(int score);

int main() {

    // Setup config
    // int screenWidth = 1024 * 1.5;
    // int screenHeight = 768;
    int screenWidth = 960;
    int screenHeight = 600;
    // SetConfigFlags( FLAG_WINDOW_UNDECORATED );
    printf("-------------------\n");
    printf("INIT WINDOW\n");
    printf("-------------------\n");
    InitWindow(screenWidth, screenHeight, "Learn Colors");
    SetWindowMonitor(2);
    SetMousePosition(0, 0);

    if (isShowCursor) {
        // Cursor stuff not working :(
        // HideCursor();
        // DisableCursor();
        // SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
        SetMouseOffset(-17, -17);
    }

    // Initialization

    // Textures
    printf("-------------------\n");
    printf("LOAD TEXTURES\n");
    printf("-------------------\n");
    Texture2D check = LoadTexture("resources/sprites/check.png");
    Texture2D clouds[NO_OF_CLOUDS];
    for (int i = 0; i < NO_OF_CLOUDS; ++i) {
        clouds[i] = LoadTexture(TextFormat("resources/backgrounds/clouds_%d.png", i + 1));
    }
    Texture2D cursor = LoadTexture("resources/ui/icon_hand_1.png");
    Texture2D cursorPressed = LoadTexture("resources/ui/icon_hand_2.png");
    Texture2D tray = LoadTexture("resources/sprites/tray.png");
    Texture2D border = LoadTexture("resources/ui/button_borders_square.png");   // 400 * 3 x 400
    border.width /= 4;
    border.height /= 4;
    // tray.width = TRAY_WIDTH * 1.5;
    // tray.height = TRAY_HEIGHT * 1.5;

    // Game vars
    double increment = 0.0;
    Color colors[] = { RED, GREEN, BLUE, ORANGE, PINK, PURPLE, SKYBLUE, GRAY };
    Rectangle trays[NO_OF_TRAYS];
    struct Card cards[NO_OF_CARDS];

    int trayStartX = -(TRAY_WIDTH * NO_OF_TRAYS) / 2;
    int cardStartX = -(CARD_WIDTH * NO_OF_CARDS) / 2;

    initTrays(trays, trayStartX, &tray);
    initCards(cards, cardStartX, colors);

    // ================================================
    Vector2 touchPosition = { 0, 0 };
    int gesturesCount = 0;
    int currentGesture = GESTURE_NONE;
    char gestureStrings[MAX_GESTURE_STRINGS][32];
    int lastGesture = GESTURE_NONE;
    // ================================================


    int frameCounter = 0;
    int counter = 0;
    int score = 0;

    int order[20];
    for (int i = 0; i < 20; i++) {
        order[i] = GetRandomValue(0, 3);
        // printf("order[i]: %d\n", order[i]);
    }

    SetTargetFPS(60);

    printf("-------------------\n");
    printf("GAME\n");
    printf("-------------------\n");

    while(!WindowShouldClose()) {
        increment += 0.02 / 25;

        if (IsKeyPressed(KEY_F)) ToggleFullscreen();
        if (IsKeyPressed(KEY_R)) {
            reset(&score);
            initCards(cards, cardStartX, colors);
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

                if (cards[i].isDragging) {
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
                        if (isTweenCard) {
                            cards[i].state = Tween;
                            cards[i].currentPosition = (Vector2) { touchPosition.x - cards[i].rect.width / 2, touchPosition.y - cards[i].rect.height / 2 };
                        } else {
                            cards[i].rect.x = cards[i].originalPosition.x;
                            cards[i].rect.y = cards[i].originalPosition.y;
                        }
                    }

                    // Have all cards been moved to the correct zone?
                    for (int j = 0; j < NO_OF_CARDS; ++j) {
                        sum += cards[j].hasTouchedEndZone;
                    }

                    // Yes? Reset cards
                    if (sum >= NO_OF_CARDS) {
                        initCards(cards, cardStartX, colors);
                    }
                }
                    
            }
        }

        // Update
        updateCards(cards);

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        drawBackground(clouds, increment, order);
        drawTrays(trays, tray, colors);
        drawCards(cards, check, border);
        drawCursor(cursor, cursorPressed);
        drawScore(score);

        EndDrawing();

    }

    printf("-------------------\n");
    printf("DESTROY\n");
    printf("-------------------\n");
    UnloadTexture(check);
    for (int i = 0; i < NO_OF_CLOUDS; ++i) {
        UnloadTexture(clouds[i]);
    }
    UnloadTexture(cursor);
    UnloadTexture(cursorPressed);
    UnloadTexture(tray);
    UnloadTexture(border);
    CloseWindow();


    return 0;
}

void initTrays(Rectangle trays[], int trayStartX, Texture2D *tray) {
    for (int i = 0; i < NO_OF_TRAYS; ++i) {
        trays[i] = (Rectangle) {
            trayStartX + GetScreenWidth() / 2 + (TRAY_WIDTH * i) + (i * GAP) - (GAP * (NO_OF_TRAYS - 1)) / 2,
            GetScreenHeight() - TRAY_HEIGHT - PADDING,
            TRAY_WIDTH,
            TRAY_HEIGHT
        };
        tray->width = TRAY_WIDTH;
        tray->height = TRAY_HEIGHT;
    }
}

void initCards(struct Card cards[], int cardStartX, Color colors[]) {
    for (int i = 0; i < NO_OF_CARDS; ++i) {
        Vector2 position = {
            cardStartX + GetScreenWidth() / 2 + (CARD_WIDTH * i) + (i * GAP) - (GAP * (NO_OF_CARDS - 1)) / 2,
            PADDING
        };
        cards[i].rect = (Rectangle) {
            position.x,
            position.y, 
            CARD_WIDTH,
            CARD_HEIGHT
        };

        // cards[i].originalPosition = (Vector2) { position.x, position.y };
        cards[i].originalPosition = position;
        cards[i].isDragging = false;
        cards[i].hasTouchedEndZone = false;
        cards[i].hasScore = false;
        cards[i].color = colors[GetRandomValue(0, NO_OF_TRAYS - 1)];
        cards[i].state = Still;
        cards[i].frameCounter = 0;
        cards[i].duration = 30;    // Length in frame
        cards[i].currentPosition = (Vector2) { position.x, position.y };
    }
}

void reset(int *score) {
    *score = 0;
}


// Update
void updateCards(struct Card cards[]) {
    if (isTweenCard) {
        for (int i = 0; i < NO_OF_CARDS; ++i) {
            if (cards[i].state == Tween) {
                cards[i].frameCounter++;

                // printf("cards[%d].frameCounter: %d\n", i,cards[i].frameCounter);

                cards[i].rect.x =
                    EaseSineOut(
                        (float) cards[i].frameCounter,
                        cards[i].currentPosition.x,
                        cards[i].originalPosition.x - cards[i].currentPosition.x,
                        cards[i].duration
                    );
                cards[i].rect.y =
                    EaseSineOut(
                        (float) cards[i].frameCounter,
                        cards[i].currentPosition.y,
                        cards[i].originalPosition.y - cards[i].currentPosition.y,
                        cards[i].duration
                    );
                if (cards[i].frameCounter > cards[i].duration) {
                    cards[i].frameCounter = 0;
                    cards[i].state = Still;
                }
            }
        }
    }
}

// Draw
void drawBackground(Texture2D layers[], double increment, int order[]) {
    if (isDrawBackground && !isOff) {
        int width = layers[0].width;
        int height = layers[0].height;
        int X_REAPEAT = ceil((float) GetScreenWidth() / (float) width);
        int startX = 0;
        int startY = -height / 2 ;

        int row = 0;

        // For each layer on the y axis
        while (startY < GetScreenHeight() + height * 2) {

            int index = order[row % 4];
            int clampedW = (width - GetScreenWidth()) / 2;
            int speed = 0;

            if (isPrarallaxBackground && !isOff) {
                speed = (sin(increment * index) * clampedW) + clampedW; // 0 < speed < 900
            }

            int xPos = startX - speed;

            DrawTexture(layers[index], xPos, startY, WHITE);

            startY += height / 1.6;
            row++;

        }
    }
}

void drawTrays(Rectangle trays[], Texture2D tray, Color colors[]) {
    for (int i = 0; i < NO_OF_TRAYS; ++i) {
        if (isDrawTray && !isOff) {
            // DrawRectangleRounded(trays[i], 0.3f, 16, colors[i]);    // Show bounds
            Rectangle rect = trays[i];
            DrawTextureV(tray, (Vector2){rect.x - 7, rect.y + 7}, BLACK);
            DrawTextureV(tray, (Vector2){rect.x, rect.y}, colors[i]);
        } else {
            DrawRectangleRounded(trays[i], 0.3f, 16, colors[i]);
        }
    }
}

void drawCards(struct Card cards[], Texture2D check, Texture2D border) {
    for (int i = 0; i < NO_OF_CARDS; ++i) {
        if (isDrawCard) {
            DrawRectangleRounded(cards[i].rect, 0.3f, 16, cards[i].color);
            DrawTextureRec(border, (Rectangle) {400, 0, 400 / 4, 400 / 4} , (Vector2) {cards[i].rect.x, cards[i].rect.y}, WHITE);
            if (cards[i].hasTouchedEndZone) {
                int x = (cards[i].originalPosition.x + CARD_WIDTH / 2) - check.width / 2;
                int y = (cards[i].originalPosition.y + CARD_HEIGHT / 2) - check.height / 2;
                DrawRectangleLines(cards[i].originalPosition.x, cards[i].originalPosition.y, CARD_WIDTH, CARD_HEIGHT, ColorAlpha(GRAY, 0.4f));
                DrawTexture(check, x, y, WHITE);
            }
        } else {
            DrawRectangleRoundedLinesEx(cards[i].rect, 0.3f, 16, 2, ColorAlpha(BLACK, 0.3f));
            DrawRectangleRounded(cards[i].rect, 0.3f, 16, cards[i].color);
            if (cards[i].hasTouchedEndZone) {
                int x = (cards[i].originalPosition.x + CARD_WIDTH / 2) - check.width / 2;
                int y = (cards[i].originalPosition.y + CARD_HEIGHT / 2) - check.height / 2;
                DrawRectangleLines(cards[i].originalPosition.x, cards[i].originalPosition.y, CARD_WIDTH, CARD_HEIGHT, ColorAlpha(GRAY, 0.4f));
                DrawTexture(check, x, y, WHITE);
            }
        }
    }
}

void drawCursor(Texture2D cursor, Texture2D cursorPressed) {
    if (isShowCursor && IsCursorOnScreen() && !isOff) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            DrawTexture(cursorPressed, GetMouseX(), GetMouseY(), WHITE);
        } else {
            DrawTexture(cursor, GetMouseX(), GetMouseY(), WHITE);
        }
    }
}

void drawScore(int score) {
    DrawText((TextFormat("Score: %d", score)), 20, 20, 30, GRAY);
}
