#include "raylib.h"
#include "reasings.h"
#include "presentation.h"

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
#define NUM_FRAMES_STARS 8

#define MIN_NO_OF_TRAYS 2   // unused
#define MAX_NO_OF_TRAYS 8   // unused

typedef enum {
    IDLE = 0,
    TWEEN,
} TweenState;

typedef struct Spritesheet {
    Rectangle frameRec;         // Draw a part of a texture defined by a rectangle
    int currentFrame;           // The current frame, x-axis. ( frameRect.x * currentFrame )
    int currentLine;            // The current frame, y-axis. ( frameRect.y * currentLine )
    int frameCounter;
    int frameSpeed;
    bool isAnimating;
} Spritesheet;

typedef struct Animation {
    Texture2D texture;
    Vector2 position;
    Spritesheet sheet;
} Animation;

typedef struct Card {
    Rectangle rect;             // Actual position
    Vector2 targetPosition;     // Could be consts
    Color color;
    bool isDragging;
    bool hasTouchedEndZone;
    bool hasScore;

    // tween
    Vector2 currentPosition;    // Tween start position
    int state;                  // IDLE | TWEEN
    int frameCounter;           // Current time in tween
    int duration;               // How long to tween
} Card;


void initCards(Card cards[], int trayStartX, Color colors[]);
void initTrays(Rectangle trays[], int trayStartX, Texture2D *tray);
void reset(int *score);

void handleInput(Rectangle trays[], Card cards[], Color colors[], int *score, int cardStartX, int *counter, Animation *stars);
void updateCards(Card cards[]);
void updateStars(Animation *stars);

void drawBackground(Texture2D clouds[], double increment, int order[]);
void drawTrays(Rectangle trays[], Texture2D tray, Color colors[]);
void drawCards(Card cards[], Texture2D check, Texture2D border);
void drawCursor(Texture2D cursor, Texture2D cursorPressed);
void drawScore(int score);
void drawStars(Animation stars);



int main() {

    // Setup config
    printf("-------------------\n");
    printf("INIT WINDOW\n");
    printf("-------------------\n");
    int screenWidth = 960;
    int screenHeight = 600;
    // SetConfigFlags( FLAG_WINDOW_UNDECORATED );
    InitWindow(screenWidth, screenHeight, "Learn Colors");
    SetWindowMonitor(2);
    SetMousePosition(-10, -10);
    if (isShowCursor) {
        // Cursor stuff not working :(
        // HideCursor();
        // DisableCursor();
        // SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
        SetMouseOffset(-17, -17);
    }

    // Initialization
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
    Texture2D starsTexture = LoadTexture("resources/ui/medal_stars.png");

    border.width /= 4;
    border.height /= 4;

    Spritesheet starsSheet = {
        (Rectangle){ 0, 0, starsTexture.width / NUM_FRAMES_STARS, starsTexture.height },
        0, 0, 0, 10, false
    };
    Animation stars = { starsTexture, (Vector2) { 0 }, starsSheet };


    // Game vars
    double increment = 0.0;
    Color colors[] = { RED, GREEN, BLUE, ORANGE, PINK, PURPLE, SKYBLUE, GRAY };
    Rectangle trays[NO_OF_TRAYS];
    Card cards[NO_OF_CARDS];

    int trayStartX = -(TRAY_WIDTH * NO_OF_TRAYS) / 2;
    int cardStartX = -(CARD_WIDTH * NO_OF_CARDS) / 2;

    initTrays(trays, trayStartX, &tray);
    initCards(cards, cardStartX, colors);

    int frameCounter = 0;
    int counter = 0;
    int score = 0;

    int order[20];      // Random value used to displace the backgrounds velocity
    for (int i = 0; i < 20; i++) {
        order[i] = GetRandomValue(0, 3);
    }


    printf("-------------------\n");
    printf("GAME\n");
    printf("-------------------\n");

    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        increment += 0.02 / 25;

        // Input
        handleInput(trays, cards, colors, &score, cardStartX, &counter, &stars);

        // Update
        updateCards(cards);
        updateStars(&stars);

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        drawBackground(clouds, increment, order);
        drawTrays(trays, tray, colors);
        drawCards(cards, check, border);
        drawCursor(cursor, cursorPressed);
        drawScore(score);
        drawStars(stars);

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
    UnloadTexture(starsTexture);
    CloseWindow();


    return 0;
}



//Init
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

void initCards(Card cards[], int cardStartX, Color colors[]) {
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

        // cards[i].targetPosition = (Vector2) { position.x, position.y };
        cards[i].targetPosition = position;
        cards[i].isDragging = false;
        cards[i].hasTouchedEndZone = false;
        cards[i].hasScore = false;
        cards[i].color = colors[GetRandomValue(0, NO_OF_TRAYS - 1)];
        cards[i].state = IDLE;
        cards[i].frameCounter = 0;
        cards[i].duration = 30;    // Length in frame (30 frame = 500ms)
        cards[i].currentPosition = (Vector2) { position.x, position.y };
    }
}

void reset(int *score) {
    *score = 0;
}

// Input
void handleInput(Rectangle trays[], Card cards[], Color colors[], int *score, int cardStartX, int *counter, Animation *stars) {
    if (IsKeyPressed(KEY_F)) ToggleFullscreen();
    if (IsKeyPressed(KEY_R)) {
        reset(score);
        initCards(cards, cardStartX, colors);
    }


    Vector2 touchPosition = GetTouchPosition(0);

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
            if (CheckCollisionPointRec(GetTouchPosition(0), cards[i].rect)) {
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
                        ++(*counter);
                        break;
                    }
                }

                // Did the card enter the correct tray?
                if (hit) {
                    // Well done, but has it already entered the zone?
                    if (cards[i].hasTouchedEndZone) continue;
                    printf("HIT %d\n", *counter);
                    if (!cards[i].hasScore) {
                        ++(*score);
                        stars->position = (Vector2){ GetTouchX() - stars->texture.width / NUM_FRAMES_STARS / 2, GetTouchY() - stars->texture.height / 2 };
                        stars->sheet.isAnimating = true;
                    }
                    cards[i].hasTouchedEndZone = true;
                    cards[i].hasScore = true;
                } else {
                    // No, tween the card back to its original position
                    printf("Reset Card...\n");
                    if (isTweenCard && !isOff) {
                        cards[i].state = TWEEN;
                        cards[i].currentPosition = (Vector2) { touchPosition.x - cards[i].rect.width / 2, touchPosition.y - cards[i].rect.height / 2 };
                    } else {
                        cards[i].rect.x = cards[i].targetPosition.x;
                        cards[i].rect.y = cards[i].targetPosition.y;
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

}

// Update
void updateCards(Card cards[]) {
    if (isTweenCard && !isOff) {
        for (int i = 0; i < NO_OF_CARDS; ++i) {
            if (cards[i].state == TWEEN) {
                cards[i].frameCounter++;
                // printf("cards[%d].frameCounter: %d\n", i,cards[i].frameCounter);


                cards[i].rect.x =
                    EaseSineOut(
                        (float) cards[i].frameCounter,
                        cards[i].currentPosition.x,
                        cards[i].targetPosition.x - cards[i].currentPosition.x,
                        cards[i].duration
                    );
                cards[i].rect.y =
                    EaseSineOut(
                        (float) cards[i].frameCounter,
                        cards[i].currentPosition.y,
                        cards[i].targetPosition.y - cards[i].currentPosition.y,
                        cards[i].duration
                    );
                if (cards[i].frameCounter > cards[i].duration) {
                    cards[i].frameCounter = 0;
                    cards[i].state = IDLE;
                }


            }
        }
    }
}

void updateStars(Animation *stars) {
    if (!(stars->sheet.isAnimating)) return;
    stars->sheet.frameCounter++;

    // Slow down frame speed
    if (stars->sheet.frameCounter >= (GetFPS() / stars->sheet.frameSpeed)) {
        // Time to update current frame index and reset counter
        stars->sheet.frameCounter = 0;
        stars->sheet.currentFrame++;

        // Ensure frame index stays within bounds
        if (stars->sheet.currentFrame > NUM_FRAMES_STARS - 1) {
            stars->sheet.currentFrame = 0;
            stars->sheet.isAnimating = false;
        }

        // Update source rect (index * width)
        stars->sheet.frameRec.x = (float) stars->sheet.currentFrame * (float) stars->texture.width / NUM_FRAMES_STARS;

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

void drawCards(Card cards[], Texture2D check, Texture2D border) {
    for (int i = 0; i < NO_OF_CARDS; ++i) {
        if (isDrawCard && !isOff) {
            DrawRectangleRounded(cards[i].rect, 0.3f, 16, cards[i].color);
            DrawTextureRec(border, (Rectangle) {400, 0, 400 / 4, 400 / 4} , (Vector2) {cards[i].rect.x, cards[i].rect.y}, WHITE);
            if (cards[i].hasTouchedEndZone) {
                int x = (cards[i].targetPosition.x + CARD_WIDTH / 2) - check.width / 2;
                int y = (cards[i].targetPosition.y + CARD_HEIGHT / 2) - check.height / 2;
                DrawRectangleLines(cards[i].targetPosition.x, cards[i].targetPosition.y, CARD_WIDTH, CARD_HEIGHT, ColorAlpha(GRAY, 0.4f));
                DrawTexture(check, x, y, WHITE);
            }
        } else {
            DrawRectangleRoundedLinesEx(cards[i].rect, 0.3f, 16, 2, ColorAlpha(BLACK, 0.3f));
            DrawRectangleRounded(cards[i].rect, 0.3f, 16, cards[i].color);
            if (cards[i].hasTouchedEndZone) {
                int x = (cards[i].targetPosition.x + CARD_WIDTH / 2) - check.width / 2;
                int y = (cards[i].targetPosition.y + CARD_HEIGHT / 2) - check.height / 2;
                DrawRectangleLines(cards[i].targetPosition.x, cards[i].targetPosition.y, CARD_WIDTH, CARD_HEIGHT, ColorAlpha(GRAY, 0.4f));
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

void drawStars(Animation stars) {
    if (stars.sheet.isAnimating && isAnimateStars && !isOff) {
        DrawTextureRec(stars.texture, stars.sheet.frameRec, stars.position , WHITE);
    }
}
