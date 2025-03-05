#include "raylib.h"
#include "reasings.h"
#include "raymath.h"
#include "presentation.h"

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

int screenWidth = 960;
int screenHeight = 600;

int gameScreenWidth = 960;
int gameScreenHeight = 600;


#define NO_OF_TRAYS 3
#define NO_OF_CARDS 4
#define GAP 70              // Space between cards & trays
#define PADDING 70          // Space above & below




#define MAX_TOUCH_POINTS 10
#define MAX_GESTURE_STRINGS 20
#define NO_OF_COLORS 8
#define NO_OF_CLOUDS 4      // 4 cloud sprites
#define NUM_FRAMES_STARS 8

// https://gcc.gnu.org/onlinedocs/gcc-13.3.0/cpp/Defined.html - simplify
// #if (defined(isDrawTray) && isDrawTray < 1)
#if (isDrawTray < 1)
    // Size of tray without texture
    #define TRAY_WIDTH 150
    #define TRAY_HEIGHT 100
#else
    // Size of trey with texture
    #define TRAY_WIDTH 250
    #define TRAY_HEIGHT 183
#endif


#if (isDrawCard < 1)
    // Size of card without texture
    #define CARD_WIDTH 100
    #define CARD_HEIGHT 100
#else
    // Size of card with texture, should be multiple of 32 to avoid scaling issues
    #define CARD_WIDTH 128
    #define CARD_HEIGHT 128
#endif

#define MIN_NO_OF_TRAYS 2   // unused
#define MAX_NO_OF_TRAYS 8   // unused

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

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
    Rectangle dest;             // Actual position
    Color color;
    bool isDragging;
    bool reachedTarget;
    bool scoredPoints;

    // tween
    Vector2 currentPosition;    // Tween start position
    Vector2 targetPosition;     // Could be consts
    int state;                  // IDLE | TWEEN
    int frameCounter;           // Current time in tween
    float duration;             // How long to tween

    // img
    Texture2D nPatchTexture;
    NPatchInfo nPatchSrc;
    Texture2D imgTexture;
    Rectangle imgSrc;
} Card;

typedef struct Tray {
    Texture2D *texture;
    Rectangle dest;
    Color color;
} Tray;

typedef struct Game {
    Card cards[NO_OF_CARDS];
    Tray trays[NO_OF_TRAYS];
    Color *colors;
    Texture2D *cardTextures;
    Texture2D *trayTexture;
    Animation *stars;
    int frameCounter;
    int score;
    int counter;
    Texture2D nPatchTexture;
    NPatchInfo nPatchSrc;
    Vector2 virtualMouse;
} Game;



// ---------------------------
// Function Declaration
// ---------------------------

// Todo: add declarations
void reset(int *score);

// ---------------------------
// Function definition
// ---------------------------

Rectangle GetRandomSource() {
    return (Rectangle) { 32 * GetRandomValue(0, 1), 32 * GetRandomValue(0, 4), 32, 32 };
}
void SetRandomSourceRec(Rectangle *rect) {
    rect->x =  32 * GetRandomValue(0, 1);
    rect->y = 32 * GetRandomValue(0, 4);
}

void initTrays(Game *game) {
    Tray *trays = game->trays;
    Color *colors = game->colors;
    Texture2D *texture = game->trayTexture;

    int trayStartX = -(TRAY_WIDTH * NO_OF_TRAYS) / 2;
    for (int i = 0; i < NO_OF_TRAYS; ++i) {
        Rectangle dest = {
            trayStartX + gameScreenWidth / 2 + (TRAY_WIDTH * i) + (i * GAP) - (GAP * (NO_OF_TRAYS - 1)) / 2,
            gameScreenHeight - TRAY_HEIGHT - PADDING,
            TRAY_WIDTH,
            TRAY_HEIGHT
        };

        trays[i] = (Tray) {
            .texture = texture,
            .dest = dest,
            .color = colors[i]
        };
    }
}
void initCards(Game *game) {
    Card *cards = game->cards;
    Color *colors = game->colors;
    Texture2D *textures = game->cardTextures;

    int cardStartX = -(CARD_WIDTH * NO_OF_CARDS) / 2;
    for (int i = 0; i < NO_OF_CARDS; ++i) {
        Vector2 startPosition = {
            cardStartX + gameScreenWidth / 2 + (CARD_WIDTH * i) + (i * GAP) - (GAP * (NO_OF_CARDS - 1)) / 2,
            PADDING
        };
        int id = GetRandomValue(0, NO_OF_TRAYS - 1);

        if (isDrawCard && id >= 3) id = GetRandomValue(0, 2);

        cards[i].dest = (Rectangle) {
            startPosition.x,
            startPosition.y,
            CARD_WIDTH,
            CARD_HEIGHT
        };
        cards[i].color = colors[id];

        // flags
        cards[i].isDragging = false;
        cards[i].reachedTarget = false;
        cards[i].scoredPoints = false;

        // tween
        cards[i].currentPosition = startPosition;   // This is set to the mousePosition at runtime
        cards[i].targetPosition = startPosition;
        cards[i].state = IDLE;
        cards[i].frameCounter = 0;
        cards[i].duration = 30.0f;                  // Length in frame (30 frame = 500ms)

        // img
        cards[i].nPatchTexture = game->nPatchTexture;
        cards[i].nPatchSrc = game->nPatchSrc;
        cards[i].imgTexture = textures[id];
        cards[i].imgSrc = GetRandomSource();
    }
}

void handleInput(Game *game, float scale) {
    Tray *trays = game->trays;
    Card *cards = game->cards;
    Color *colors = game->colors;
    Animation *stars = game->stars;

    if (IsKeyPressed(KEY_F)) ToggleFullscreen();
    if (IsKeyPressed(KEY_R)) {
        reset(&game->score);
        initCards(game);
    }


    // Update virtual mouse (clamped mouse value behind game screen)
    // Vector2 mouse = GetMousePosition();
    // Vector2 touchPosition = GetTouchPosition(0);

    Vector2 mouse = GetTouchPosition(0);
    Vector2 virtualMouse = { 0 };
    virtualMouse.x = (mouse.x - (GetScreenWidth() - (gameScreenWidth*scale))*0.5f)/scale;
    virtualMouse.y = (mouse.y - (GetScreenHeight() - (gameScreenHeight*scale))*0.5f)/scale;
    virtualMouse = Vector2Clamp(virtualMouse, (Vector2){ 0, 0 }, (Vector2){ (float)gameScreenWidth, (float)gameScreenHeight });

    game->virtualMouse = virtualMouse;

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
        // Pointer arithmetic to get the next card
        Card *card = (cards + i);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(virtualMouse, card->dest)) {
                card->isDragging = true;
            }
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (card->isDragging) {
                card->dest.x = virtualMouse.x - card->dest.width / 2;
                card->dest.y = virtualMouse.y - card->dest.height / 2;
            }
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

            if (card->isDragging) {
                card->isDragging = false;

                bool hit = false;
                int sum = 0;

                for (int j = 0; j < NO_OF_TRAYS; ++j) {
                    if (CheckCollisionRecs(card->dest, trays[j].dest) && ColorIsEqual(card->color, colors[j])) {
                        hit = true;
                        ++(game->counter);
                        break;
                    }
                }

                // Did the card enter the correct tray?
                if (hit) {
                    // Well done, but has it already entered the zone?
                    if (card->reachedTarget) continue;
                    printf("HIT %d\n", game->counter);
                    if (!card->scoredPoints) {
                        ++(game->score);



                        // Get free starts object
                        // Set the properties
                        // Add to list
                        stars->position = (Vector2) { virtualMouse.x - stars->texture->width / NUM_FRAMES_STARS / 2, virtualMouse.y - stars->texture->height / 2 };
                        stars->sheet.isAnimating = true;




                        if (isAudio && !isOff) PlaySFX(sfx.click);
                    }
                    card->reachedTarget = true;
                    card->scoredPoints = true;
                } else {
                    // No, tween the card back to its original position
                    printf("Reset Card...\n");
                    if (isTweenCard && !isOff) {
                        card->state = TWEEN;
                        card->currentPosition = (Vector2) { virtualMouse.x - card->dest.width / 2, virtualMouse.y - card->dest.height / 2 };
                    } else {
                        card->dest.x = card->targetPosition.x;
                        card->dest.y = card->targetPosition.y;
                    }

                    if (isAudio && !isOff) PlaySFX(sfx.stop);
                }

                // Have all cards been moved to the correct zone?
                for (int j = 0; j < NO_OF_CARDS; ++j) {
                    sum += cards[j].reachedTarget;
                }

                // Yes? Reset cards
                if (sum >= NO_OF_CARDS) {
                    initCards(game);

                    if (isAudio && !isOff) PlaySFX(sfx.popup);
                }
            }

        }
    }

}

void updateCards(Card cards[]) {
    if (isTweenCard && !isOff) {
        for (int i = 0; i < NO_OF_CARDS; ++i) {
            Card *card = (cards + i);
            if (card->state == TWEEN) {
                card->frameCounter++;
                // printf("cards[%d].frameCounter: %d\n", i,card->frameCounter);


                float x = EaseBackOut(
                        (float) card->frameCounter,
                        card->currentPosition.x,
                        card->targetPosition.x - card->currentPosition.x,
                        card->duration
                    );
                float y = EaseBackOut(
                        (float) card->frameCounter,
                        card->currentPosition.y,
                        card->targetPosition.y - card->currentPosition.y,
                        card->duration
                    );

                card->dest.x = x;
                card->dest.y = y;

                if (card->frameCounter >= card->duration) {
                    card->frameCounter = 0;
                    card->state = IDLE;
                    card->dest.x = card->targetPosition.x;
                    card->dest.y = card->targetPosition.y;
                }


            }
        }
    }
}
void updateStars(Animation *stars) {
    // Loop through all stars, and animate otherwise do nothing to empty array
    if (!(stars->sheet.isAnimating)) return;
    stars->sheet.frameCounter++;

    // Slow down frame speed
    if (stars->sheet.frameCounter >= (GetFPS() / stars->sheet.frameSpeed)) {
        // Time to update current frame index and reset counter
        stars->sheet.frameCounter = 0;
        stars->sheet.currentFrame++;

        // Ensure frame index stays within bounds
        if (stars->sheet.currentFrame > NUM_FRAMES_STARS - 1) {



            // Star animation is complete
            // Reset it members
            // Remove current reference from the list
            stars->sheet.currentFrame = 0;
            stars->sheet.isAnimating = false;



        }

        // Update source rect (index * width)
        stars->sheet.frameRec.x = (float) stars->sheet.currentFrame * (float) stars->texture->width / NUM_FRAMES_STARS;

    }
}

void drawBackground(Texture2D layers[], double *increment, int order[]) {
    if (isDrawBackground && !isOff) {
        (*increment) += 0.02 / 25;

        int width = layers[0].width;
        int height = layers[0].height;
        int startX = 0;
        int startY = -height / 2 ;

        int row = 0;

        // For each layer on the y axis
        while (startY < gameScreenHeight + height * 2) {

            int index = order[row % 4];
            int clampedW = (width - gameScreenWidth) / 2;
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
            DrawTextureV(*trays->texture, (Vector2){tray.dest.x - 7, tray.dest.y + 7}, BLACK);
            DrawTextureV(*trays->texture, (Vector2){tray.dest.x, tray.dest.y}, tray.color);
        } else {
            DrawRectangleRounded(trays[i].dest, 0.3f, 16, tray.color);
        }
    }
}
void drawCards(Card cards[], Texture2D check) {
    for (int i = 0; i < NO_OF_CARDS; ++i) {
        Card card = cards[i];
        if (isDrawCard && !isOff) {
            DrawTextureNPatch(card.nPatchTexture, card.nPatchSrc, card.dest, (Vector2) { 0 }, 0, WHITE);
            DrawTexturePro(card.imgTexture, card.imgSrc, card.dest, (Vector2) { 0 }, 0, WHITE);
            // DrawRectangleRoundedLinesEx(card.dest, 0.3f, 16, 6, ColorAlpha(PINK, 0.5f));
        } else {
            DrawRectangleRoundedLinesEx(card.dest, 0.3f, 16, 2, ColorAlpha(BLACK, 0.3f));
            DrawRectangleRounded(card.dest, 0.3f, 16, card.color);
        }

        // Draw empty square
        if (card.reachedTarget) {
            int x = (card.targetPosition.x + CARD_WIDTH / 2) - check.width / 2;
            int y = (card.targetPosition.y + CARD_HEIGHT / 2) - check.height / 2;
            DrawRectangleLines(card.targetPosition.x, card.targetPosition.y, CARD_WIDTH, CARD_HEIGHT, ColorAlpha(GRAY, 0.4f));
            DrawTexture(check, x, y, WHITE);
        }
    }
}
void drawCursor(Vector2 virtualMouse, Texture2D cursor, Texture2D cursorPressed, float scale) {
    if (isShowCursor && IsCursorOnScreen() && !isOff) {
        // Subtract the offset of cursor tip
        virtualMouse = Vector2SubtractValue(virtualMouse, 17);
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            DrawTexture(cursorPressed, virtualMouse.x, virtualMouse.y, WHITE);
        } else {
            DrawTexture(cursor, virtualMouse.x, virtualMouse.y, WHITE);
        }
    }
}
void drawScore(int score) {
    DrawText((TextFormat("Score: %d", score)), 20, 20, 30, GRAY);
}
void drawStars(Animation stars) {
    // Loop through all stars, and draw otherwise do nothing to empty array
    if (stars.sheet.isAnimating && isAnimateStars && !isOff) {
        DrawTextureRec(*stars.texture, stars.sheet.frameRec, stars.position , WHITE);
    }
}

void reset(int *score) {
    *score = 0;
}
