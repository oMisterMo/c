#include "raylib.h"
#include "reasings.h"
#include "presentation.h"

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define MAX_SOUNDS 10
Sound soundArray[MAX_SOUNDS] = { 0 };
int currentSound;

#define MAX_TOUCH_POINTS 10
#define MAX_GESTURE_STRINGS 20

#define NO_OF_COLORS 8

#define NO_OF_TRAYS 3
#define NO_OF_CARDS 4
#define NO_OF_CLOUDS 4      // 4 cloud sprites

#define TRAY_WIDTH 250
#define TRAY_HEIGHT 183


#if (defined(isDrawCard) && isDrawCard < 1)
    #define CARD_WIDTH 100      // min size = 150 = size of check texture
    #define CARD_HEIGHT 100
#else
    #define CARD_WIDTH 150
    #define CARD_HEIGHT 150
#endif

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
    Texture2D *texture;
    Vector2 position;
    Spritesheet sheet;
} Animation;

typedef struct Card {
    Rectangle rect;             // Actual position
    Color color;
    bool isDragging;
    bool hasTouchedEndZone;
    bool hasScore;

    // tween
    Vector2 currentPosition;    // Tween start position
    Vector2 targetPosition;     // Could be consts
    int state;                  // IDLE | TWEEN
    int frameCounter;           // Current time in tween
    float duration;             // How long to tween

    // img
    Texture2D texture;
    Rectangle src;
} Card;

typedef struct Tray {
    Texture2D *texture;
    Rectangle rect;
    Color color;
} Tray;

typedef struct Game {
    Card cards[NO_OF_CARDS];
    Tray trays[NO_OF_TRAYS];
    Color *colors;
    Texture2D *colorTextures;
    Animation *stars;
    int frameCounter;
    int score;
    int counter;
} Game;



// ---------------------------
// Function Definition
// ---------------------------
// void initCards(Card cards[], Color colors[]);
// void initTrays(Tray trays[], Color colors[], Texture2D *trayTexture);
// void reset(int *score);

// void handleInput(Game *game, int *score, int *counter, Animation *stars);

// void updateCards(Card cards[]);
// void updateStars(Animation *stars);

// void drawBackground(Texture2D clouds[], double *increment, int order[]);
// void drawTrays(Tray trays[]);
// void drawCards(Card cards[], Texture2D check, Texture2D border);
// void drawCursor(Texture2D cursor, Texture2D cursorPressed);
// void drawScore(int score);
// void drawStars(Animation stars);

// ---------------------------
// Implementation
// ---------------------------

Rectangle GetRandomSource() {
    return (Rectangle) { 32 * GetRandomValue(0, 1), 32 * GetRandomValue(0, 4), 32, 32 };
}

void GetRandomSourceRec(Rectangle *rect) {
    rect->x =  32 * GetRandomValue(0, 1);
    rect->y = 32 * GetRandomValue(0, 4);
}

void initTrays(Tray trays[], Color colors[], Texture2D *trayTexture) {
    int trayStartX = -(TRAY_WIDTH * NO_OF_TRAYS) / 2;
    for (int i = 0; i < NO_OF_TRAYS; ++i) {
        Rectangle rect = {
            trayStartX + GetScreenWidth() / 2 + (TRAY_WIDTH * i) + (i * GAP) - (GAP * (NO_OF_TRAYS - 1)) / 2,
            GetScreenHeight() - TRAY_HEIGHT - PADDING,
            TRAY_WIDTH,
            TRAY_HEIGHT
        };

        trays[i] = (Tray) {
            .texture = trayTexture,
            .rect = rect,
            .color = colors[i]
        };
    }
}
void initCards(Card cards[], Color colors[], Texture2D textures[]) {
    int cardStartX = -(CARD_WIDTH * NO_OF_CARDS) / 2;
    for (int i = 0; i < NO_OF_CARDS; ++i) {
        Vector2 startPosition = {
            cardStartX + GetScreenWidth() / 2 + (CARD_WIDTH * i) + (i * GAP) - (GAP * (NO_OF_CARDS - 1)) / 2,
            PADDING
        };
        int id = GetRandomValue(0, NO_OF_TRAYS - 1);
        float scale = 0.7f;

        cards[i].rect = (Rectangle) {
            startPosition.x,
            startPosition.y,
            CARD_WIDTH,
            CARD_HEIGHT
        };
        cards[i].color = colors[id];

        // flags
        cards[i].isDragging = false;
        cards[i].hasTouchedEndZone = false;
        cards[i].hasScore = false;

        // tween
        cards[i].currentPosition = startPosition;   // This is set to the mousePosition at runtime
        cards[i].targetPosition = startPosition;
        cards[i].state = IDLE;
        cards[i].frameCounter = 0;
        cards[i].duration = 30.0f;                  // Length in frame (30 frame = 500ms)

        // img
        cards[i].texture = textures[id];
        cards[i].src = GetRandomSource();
    }
}
void reset(int *score) {
    *score = 0;
}

void handleInput(Game *game) {
    Tray *trays = game->trays;
    Card *cards = game->cards;
    Color *colors = game->colors;
    Texture2D *colorTextures = game->colorTextures;
    Animation *stars = game->stars;

    if (IsKeyPressed(KEY_F)) ToggleFullscreen();
    if (IsKeyPressed(KEY_R)) {
        reset(&game->score);
        initCards(cards, colors, colorTextures);
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
                    if (CheckCollisionRecs(cards[i].rect, trays[j].rect) && ColorIsEqual(cards[i].color, colors[j])) {
                        hit = true;
                        ++(game->counter);
                        break;
                    }
                }

                // Did the card enter the correct tray?
                if (hit) {
                    // Well done, but has it already entered the zone?
                    if (cards[i].hasTouchedEndZone) continue;
                    printf("HIT %d\n", game->counter);
                    if (!cards[i].hasScore) {
                        ++(game->score);
                        stars->position = (Vector2){ GetTouchX() - (*stars->texture).width / NUM_FRAMES_STARS / 2, GetTouchY() - (*stars->texture).height / 2 };
                        stars->sheet.isAnimating = true;

                        if (isAudio && !isOff) PlaySound(soundArray[0]);  // CLICK
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

                    if (isAudio && !isOff) PlaySound(soundArray[2]);  // STOP
                }

                // Have all cards been moved to the correct zone?
                for (int j = 0; j < NO_OF_CARDS; ++j) {
                    sum += cards[j].hasTouchedEndZone;
                }

                // Yes? Reset cards
                if (sum >= NO_OF_CARDS) {
                    initCards(cards, colors, colorTextures);

                    if (isAudio && !isOff) PlaySound(soundArray[3]);  // CHIME
                }
            }

        }
    }

}

void updateCards(Card cards[]) {
    if (isTweenCard && !isOff) {
        for (int i = 0; i < NO_OF_CARDS; ++i) {
            if (cards[i].state == TWEEN) {
                cards[i].frameCounter++;
                // printf("cards[%d].frameCounter: %d\n", i,cards[i].frameCounter);


                float x = EaseBackOut(
                        (float) cards[i].frameCounter,
                        cards[i].currentPosition.x,
                        cards[i].targetPosition.x - cards[i].currentPosition.x,
                        cards[i].duration
                    );
                float y = EaseBackOut(
                        (float) cards[i].frameCounter,
                        cards[i].currentPosition.y,
                        cards[i].targetPosition.y - cards[i].currentPosition.y,
                        cards[i].duration
                    );

                cards[i].rect.x = x;
                cards[i].rect.y = y;

                if (cards[i].frameCounter >= cards[i].duration) {
                    cards[i].frameCounter = 0;
                    cards[i].state = IDLE;
                    cards[i].rect.x = cards[i].targetPosition.x;
                    cards[i].rect.y = cards[i].targetPosition.y;
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
        stars->sheet.frameRec.x = (float) stars->sheet.currentFrame * (float) (*stars->texture).width / NUM_FRAMES_STARS;

    }
}

void drawBackground(Texture2D layers[], double *increment, int order[]) {
    if (isDrawBackground && !isOff) {
        (*increment) += 0.02 / 25;

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
                speed = (sin(*increment * index) * clampedW) + clampedW; // 0 < speed < 900
            }

            int xPos = startX - speed;

            DrawTexture(layers[index], xPos, startY, WHITE);

            startY += height / 1.6;
            row++;

        }
    }
}
void drawTrays(Tray trays[]) {
    for (int i = 0; i < NO_OF_TRAYS; ++i) {
        Tray tray = trays[i];
        if (isDrawTray && !isOff) {
            // DrawRectangleRounded(trays[i], 0.3f, 16, colors[i]);    // Show bounds
            DrawTextureV(*trays->texture, (Vector2){tray.rect.x - 7, tray.rect.y + 7}, BLACK);
            DrawTextureV(*trays->texture, (Vector2){tray.rect.x, tray.rect.y}, tray.color);
        } else {
            DrawRectangleRounded(trays[i].rect, 0.3f, 16, tray.color);
        }
    }
}
void drawCards(Card cards[], Texture2D check, Texture2D border) {
    for (int i = 0; i < NO_OF_CARDS; ++i) {
        Card card = cards[i];
        if (isDrawCard && !isOff) {
            DrawTexturePro(card.texture,
            card.src, card.rect,
            (Vector2) { 0 }, 0, WHITE);
            DrawRectangleRoundedLinesEx(card.rect, 0.3f, 16, 6, ColorAlpha(PINK, 0.5f));
        } else {
            DrawRectangleRoundedLinesEx(card.rect, 0.3f, 16, 2, ColorAlpha(BLACK, 0.3f));
            DrawRectangleRounded(card.rect, 0.3f, 16, card.color);
        }

        // Draw empty square
        if (cards[i].hasTouchedEndZone) {
            int x = (cards[i].targetPosition.x + CARD_WIDTH / 2) - check.width / 2;
            int y = (cards[i].targetPosition.y + CARD_HEIGHT / 2) - check.height / 2;
            DrawRectangleLines(cards[i].targetPosition.x, cards[i].targetPosition.y, CARD_WIDTH, CARD_HEIGHT, ColorAlpha(GRAY, 0.4f));
            DrawTexture(check, x, y, WHITE);
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
        DrawTextureRec(*stars.texture, stars.sheet.frameRec, stars.position , WHITE);
    }
}
