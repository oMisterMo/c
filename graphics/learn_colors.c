#include "learn_colors.h"





int main() {

    // Setup config
    printf("-------------------\n");
    printf("INIT WINDOW\n");
    printf("-------------------\n");
    int screenWidth = 960;
    int screenHeight = 600;
    // SetConfigFlags( FLAG_WINDOW_UNDECORATED );
    InitWindow(screenWidth, screenHeight, "Learn Colors");
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
    Texture2D checkTexture = LoadTexture("resources/sprites/check.png");
    Texture2D cloudsTexture[NO_OF_CLOUDS];
    for (int i = 0; i < NO_OF_CLOUDS; ++i) {
        cloudsTexture[i] = LoadTexture(TextFormat("resources/backgrounds/clouds_%d.png", i + 1));
    }
    Texture2D cursorTexture = LoadTexture("resources/ui/icon_hand_1.png");
    Texture2D cursorPressedTexture = LoadTexture("resources/ui/icon_hand_2.png");
    Texture2D trayTexture = LoadTexture("resources/sprites/tray.png");
    Texture2D borderTexture = LoadTexture("resources/ui/button_borders_square.png");   // 400 * 3 x 400
    Texture2D starsTexture = LoadTexture("resources/ui/medal_stars.png");
    Texture2D redTexture = LoadTexture("resources/sprites/red.png");
    Texture2D greenTexture = LoadTexture("resources/sprites/green.png");
    Texture2D blueTexture = LoadTexture("resources/sprites/blue.png");
    Rectangle greenSrc = { 0, 0, 32, 32 };
    Rectangle redSrc = { 0, 0, 32, 32 };
    Rectangle blueSrc = { 0, 0, 32, 32 };

    GetRandomSourceRec(&greenSrc);
    GetRandomSourceRec(&redSrc);
    GetRandomSourceRec(&blueSrc);

    borderTexture.width /= 4;
    borderTexture.height /= 4;
    trayTexture.width = TRAY_WIDTH;
    trayTexture.height = TRAY_HEIGHT;

    Spritesheet starsSheet = { 0 };
    starsSheet.frameRec = (Rectangle) { 0, 0, starsTexture.width / NUM_FRAMES_STARS, starsTexture.height };
    starsSheet.currentFrame = 0;
    starsSheet.currentLine = 0;
    starsSheet.frameCounter = 0;
    starsSheet.frameSpeed = 10;
    starsSheet.isAnimating = false;

    Animation stars = { 0 };
    stars.texture = &starsTexture;
    stars.position = (Vector2) { 0 };
    stars.sheet = starsSheet;


    // Game vars
    Game game = { 0 };

    game.colors = (Color[]) { RED, GREEN, BLUE, ORANGE, PINK, PURPLE, SKYBLUE, GRAY };
    game.colorTextures = (Texture2D[]) { redTexture, greenTexture, blueTexture };
    game.frameCounter = 0;
    game.score = 0;
    game.counter = 0;
    game.stars = &stars;

    // Rectangle trays[NO_OF_TRAYS];
    initTrays(game.trays, game.colors, &trayTexture);
    initCards(game.cards, game.colors, game.colorTextures);

    double increment = 0.0;

    int order[20];      // Random value used to displace the backgrounds velocity
    for (int i = 0; i < 20; i++) {
        order[i] = GetRandomValue(0, 3);
    }

    // Audio
    if (isAudio && !isOff) {
        printf("-------------------\n");
        printf("LOAD AUDIO\n");
        printf("-------------------\n");
        InitAudioDevice();
        soundArray[0] = LoadSound("resources/sfx/button_click.wav");
        soundArray[1] = LoadSound("resources/sfx/piece_select.wav");
        soundArray[2] = LoadSound("resources/sfx/piece_stop.wav");
        soundArray[3] = LoadSound("resources/sfx/popup.wav");
        currentSound = 0;
    }


    printf("-------------------\n");
    printf("GAME\n");
    printf("-------------------\n");

    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        // Input
        handleInput(&game);

        // Update
        updateCards(game.cards);
        updateStars(game.stars);

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        drawBackground(cloudsTexture, &increment, order);
        drawTrays(game.trays);
        drawCards(game.cards, checkTexture, borderTexture);
        drawCursor(cursorTexture, cursorPressedTexture);
        drawScore(game.score);
        drawStars(stars);

        EndDrawing();

    }

    printf("-------------------\n");
    printf("DESTROY\n");
    printf("-------------------\n");
    // Textures
    UnloadTexture(checkTexture);
    for (int i = 0; i < NO_OF_CLOUDS; ++i) {
        UnloadTexture(cloudsTexture[i]);
    }
    UnloadTexture(cursorTexture);
    UnloadTexture(cursorPressedTexture);
    UnloadTexture(trayTexture);
    UnloadTexture(borderTexture);
    UnloadTexture(starsTexture);
    UnloadTexture(redTexture);
    UnloadTexture(greenTexture);
    UnloadTexture(blueTexture);

    // Audio
    if (isAudio && !isOff) {
        for (int i = 0; i < MAX_SOUNDS; ++i) {
            UnloadSound(soundArray[i]);
        }
        CloseAudioDevice();
    }

    CloseWindow();


    return 0;
}
