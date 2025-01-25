typedef enum {
    LOGO = 0,
    MENU,
    LEVEL,
    GAME,
    GAMEOVER,
    TRANSITION_START,
    TRANSITION_END
} GameScreen;

typedef enum {
    UI_BACK = 0,
    UI_RETRY,
    UI_RANDOM,
    UI_SKIP,
    UI_LEFT,
    UI_RIGHT
} UIButton;

typedef enum {
    IDLE = 0,
    TWEENING
} TweenState;

typedef struct Tween {
    Vector2 startPosition;      // Tween start
    Vector2 targetPosition;     // Tween end [could be consts]
    int state;                  // IDLE | TWEENING
    int frameCounter;           // Current time in tween
    int duration;               // How long to tween
} Tween;

typedef struct UIButtons {
    Texture2D texture;
    Rectangle src;
    Rectangle dest;
} UIButtons;
