#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

#define WIDTH 600
#define HEIGHT 600

// Padle
#define PADDING 50
#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 100
#define BALL_SIZE 20
#define INITIAL_BALL_SPEED 150

// Particles
#define NO_OF_PARTICLES 1 << 4

typedef enum MODE {
    PLAY_SINGLE = 0,
    PLAY_MULTIPLE
} MODE;

typedef enum CameraType {
    CAMERA_SCREEN = 0,
    CAMERA_SCREEN_SHAKE,
} CameraType;

typedef struct Particle {
    int id;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    float size;

    float scale;
    float scaleStart;
    float scaleEnd;
    float scaleVelocity;
    // float rotation;

    float alpha;
    float alphaStart;
    float alphaEnd;
    float alphaVelocity;

    Color color;
    Color colorStart;
    Color colorEnd;

    bool dead;
    float ageSeconds;
    float ageStartSeconds;
    float currentTimeOffsetX;   // For perlin noise
    float currentTimeOffsetY;   // For perlin noise
    // Texture2D img;
} Particle;

typedef struct ScreenShake {
    float shake;
    float maxAngle;
    float maxOffset;
    float angle;
    float offsetX;
    float offsetY;
} ScreenShake;

static Rectangle paddle_player = { PADDING, (HEIGHT - PADDLE_HEIGHT) / 2, PADDLE_WIDTH, PADDLE_HEIGHT };
static Rectangle paddle_other = { WIDTH - PADDLE_WIDTH - PADDING, 200, PADDLE_WIDTH, PADDLE_HEIGHT };
static Rectangle ball = { 130, 160, BALL_SIZE, BALL_SIZE };
static float velX = INITIAL_BALL_SPEED;
static float velY = INITIAL_BALL_SPEED;
static float speed = 450;
float t = 0.0f;

MODE playmode = PLAY_MULTIPLE;

static bool player_hit = false;
static bool other_hit = false;

static Camera2D camera = { 0 };
static Camera2D camera_shake = { 0 };
static ScreenShake screenShake = { 0 };
static CameraType cameraType = 0;


// =========== Particles ===========

int GenerateRandomId() {
    static int id = 0;
    return id++;
}

float GetRandomValueFloat(float min, float max) {
    // Use perlin noise, not random values
    // https://www.youtube.com/watch?v=tu-Qe66AvtY
    return (((float)rand()/(float)(RAND_MAX)) * abs(max - min)) + min;
    // return (((float)rand()/(float)(RAND_MAX)) * 2) - 1; // between -1 and 1
}

void SetPaticleFall(Particle *p) {
    float minSpeedX = -50;
    float maxSpeedX = 50;

    // position
    p->position.x = (float) GetRandomValue(0, WIDTH);
    p->position.y = (float) GetRandomValue(0, HEIGHT);
    p->velocity.x = GetRandomValueFloat(minSpeedX, maxSpeedX);
    p->velocity.y = 50;
    p->acceleration.x = 0;
    p->acceleration.y = 0;

    p->size = GetRandomValue(3, 4);

    // scale
    p->scaleStart = 1.0f;
    p->scaleEnd = 0.0f;
    p->scale = p->scaleStart;
    p->scaleVelocity = GetRandomValueFloat(-0.1, -1); // do I still need this?

    // printf("scale %f\n", p->scale);
    // printf("scale vel %f\n", p->scaleVelocity);

    // alpha
    p->alphaStart = 0.0f;
    p->alphaEnd = 0.0f;
    p->alpha = p->alphaStart;
    p->alphaVelocity = GetRandomValueFloat(-0.1, -1);   // do I still need this?

    // color
    p->colorStart = WHITE;
    p->colorEnd = BROWN;
    p->color = p->colorStart;


    p->dead = false;
    p->ageSeconds = GetRandomValueFloat(1, 15);
    p->ageStartSeconds = p->ageSeconds;
}

void CreateSimpleParticleEffect(Particle *particles) {
    printf("create...\n");
    for (int i = 0; i < NO_OF_PARTICLES; ++i) {
        Particle *p = &particles[i];
        SetPaticleFall(p);
    }
}

void UpdateParticles(Particle *particles) {
    float dt = GetFrameTime();
    for (int i = 0; i < NO_OF_PARTICLES; ++i) {
        Particle *p = &particles[i];
        if (p->ageSeconds <= 0) {
            SetPaticleFall(p);
            continue;
        }
            
        p->ageSeconds -= dt;
        // Update position
        p->velocity.x += p->acceleration.x * dt;
        p->velocity.y += p->acceleration.y * dt;
        p->position.x += p->velocity.x * dt;
        p->position.y += p->velocity.y * dt;
        // p->position.x += 0.2;
        // p->position.y += 0.2;

        // If I want to mess around with it, not got it moving nicely
        // UpdateParticlePerlin(p);


        // Update rotation

        // Update scale
        float d = p->ageSeconds / p->ageStartSeconds;
        float t = 1.0f - d; // progresses from 0 to 1 over the lifetime

        // p->scale += p->scaleVelocity * dt;
        // p->scale = Clamp(p->scale, 0, 30);
        p->scale = p->scaleStart * d;

        // Update alpha
        // 1 - mo
        // p->alpha += p->alphaVelocity * dt;
        // p->alpha = Clamp(p->alpha, 0, 1);

        // 2 - creates a triangle fade: 0 → 1 → 0
        p->alpha = 1.0f - fabsf(2.0f * t - 1.0f);

        // 3 - prettier easing: smooth in and out
        // p->alpha = sinf(t * PI);

        // 4 - sharper fade
        // p->alpha = powf(1.0f - fabsf(2.0f * t - 1.0f), 2.0f);

        // Update color
        p->color = ColorLerp(p->colorStart, p->colorEnd, d);

        // printf("t: %.2f\n", t);
        // printf("ageSeconds %.2f\n", p->ageSeconds);
        // printf("scale %.2f\n", p->scale);
        // printf("alpha %.2f\n", p->alpha);
        // printf("---------------\n");
    }

}

void DrawParticles(Particle *particles) {
    for (int i = 0; i < NO_OF_PARTICLES; ++i) {
        Particle p = particles[i];
        if (p.ageSeconds <= 0) {
            continue;
        }

        DrawRectangleRec((Rectangle){p.position.x,p.position.y,p.size*p.scale,p.size*p.scale},ColorAlpha(p.color, p.alpha));
    }

}

// =========== Screen shake ===========

void ApplyShake(float trauma) {
    printf("Shake...\n");
    cameraType = CAMERA_SCREEN_SHAKE;
    screenShake.shake += trauma;
    if (screenShake.shake > 1.0f) screenShake.shake = 1.0f;
}

void UpdateScreenShake() {
    if (screenShake.shake > 0) {
        // TODO: FRAME RATE INDEPENDENT
        screenShake.shake -= 0.01;

        // Update shake
        screenShake.angle = screenShake.shake * GetRandomValueFloat(-1, 1);
        screenShake.offsetX = screenShake.maxOffset * screenShake.shake * GetRandomValueFloat(-1, 1);
        screenShake.offsetY = screenShake.maxOffset * screenShake.shake * GetRandomValueFloat(-1, 1);

        // Apply rotation and translation
        camera_shake.rotation = camera.rotation + screenShake.angle;
        camera_shake.target = Vector2Add(camera.target,  (Vector2){screenShake.offsetX, screenShake.offsetX});

    } else {
        screenShake.shake = 0;
        cameraType = CAMERA_SCREEN;
    }
}

// =========== Game ===========

void UpdateBall() {
    float dt = GetFrameTime();

    ball.x += velX * dt;
    ball.y += velY * dt;

    if (ball.x < 0) {
        ball.x = 0;
        velX *= -1.0f;
        ApplyShake(0.1f);
    }
    if (ball.x + BALL_SIZE >= WIDTH) {
        ball.x = WIDTH - BALL_SIZE;
        velX *= -1.1;
        ApplyShake(0.1f);
    }
    if (ball.y < 0) {
        ball.y = 0;
        velY *= -1.0f;
        ApplyShake(0.1f);
    }
    if (ball.y + BALL_SIZE >= HEIGHT) {
        ball.y = HEIGHT - BALL_SIZE;
        velY *= -1.0f;
        ApplyShake(0.1f);
    }
    // printf("%f\n", velX);
    if (velX >= 1000) velX = 1000;
}

void Collisions() {
    // // DEBUGGIN...
    // Vector2 mouse = GetMousePosition();
    // ball.x = mouse.x - BALL_SIZE / 2;
    // ball.y = mouse.y - BALL_SIZE / 2;
    // // ...END DEBUGGING

    if (CheckCollisionRecs(ball, paddle_player)) {
        player_hit = true;
        ApplyShake(0.2f);

        // Find the amount collided
        Rectangle rec = GetCollisionRec(ball, paddle_player);
        // printf("%.2f, %.2f, %.2f, %.2f\n", rec.x, rec.y, rec.width, rec.height);

        // Check where the ball hit (top? sides?)
        if (rec.height < 20) {
            // printf("top\n");
            if (velY < 0) {
                ball.y += rec.height;
            } else {
                ball.y -= rec.height;
            }
            // Reverse direction or Get normal based on hit location
            velY *= -1.0f;
        }
        if (rec.width < 20) {
            // printf("sides\n");
            // Push ball out
            // add to ball
            if (velX < 0) {
                ball.x += rec.width;
            } else {
                ball.x -= rec.width;
            }
            // Reverse direction or Get normal based on hit location
            velX *= -1.0f;
        }

    } else {
        player_hit = false;
    }
    if (CheckCollisionRecs(ball, paddle_other)) {
        other_hit = true;
        ApplyShake(0.2f);
        Rectangle rec = GetCollisionRec(ball, paddle_other);
        if (velX > 0) {
            ball.x -= rec.width;
        } else {
            ball.x += rec.width;
        }
        velX *= -1.0f;
    } else {
        other_hit = false;
    }

}

void DrawGUI() {
    int fontsize = 20;
    int textW = MeasureText("1 Player", fontsize);
    int x = WIDTH - textW - 20;
    int y = 20;

    switch (playmode) {
        case PLAY_SINGLE:
            DrawText(TextFormat("1 Player"), x, y, fontsize, WHITE);
            break;
            case PLAY_MULTIPLE:
            DrawText(TextFormat("2 Player"), x, y, fontsize, WHITE);
            break;
    }
}


void Input() {
    float dt = GetFrameTime();

    if (IsKeyPressed(KEY_P)) {
        printf("p\n");
        if (playmode == PLAY_SINGLE) {
            playmode = PLAY_MULTIPLE;
        } else {
            playmode = PLAY_SINGLE;
        }
    }

    if (IsKeyPressed(KEY_FOUR)) {
        ApplyShake(0.2f);
    }

    if (IsKeyPressed(KEY_R)) {
        camera.rotation += 45;
        camera_shake.rotation = camera.rotation;
    }


    switch (playmode) {
        case PLAY_SINGLE:

            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
                paddle_player.y -= speed * dt;
            }
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
                paddle_player.y += speed * dt;
            }

            break;
        case PLAY_MULTIPLE:

            if (IsKeyDown(KEY_UP)) {
                paddle_other.y -= speed * dt;
            }
            if (IsKeyDown(KEY_DOWN)) {
                paddle_other.y += speed * dt;
            }
            
            if (IsKeyDown(KEY_W)) {
                paddle_player.y -= speed * dt;
            }
            if (IsKeyDown(KEY_S)) {
                paddle_player.y += speed * dt;
            }

            break;
    }
}

void Update(Particle *particles) {
    UpdateBall();
    Collisions();
    UpdateParticles(particles);
    UpdateScreenShake();
}

void Draw() {
    BeginMode2D(screenShake.shake > 0 ? camera_shake : camera);

        // Draw padal
        DrawRectangleRec(paddle_player, player_hit ? RED : WHITE);
        DrawRectangleRec(paddle_other, other_hit ? RED : WHITE);
        // Draw ball
        DrawRectangleRec(ball, WHITE);

        // Draw seperator
        // DrawRectangleRec((Rectangle) {(WIDTH - PADDLE_WIDTH) / 2, 0, PADDLE_WIDTH, HEIGHT}, WHITE);

        t += 0.01;
        if (t > 1.0f) t = 1.0f;
        int ITESM = 16;
        for (int i = 0; i < ITESM; ++i) {
            // DrawRectangle((WIDTH - BALL_SIZE) / 2, 0 + i * BALL_SIZE * 2, BALL_SIZE, BALL_SIZE, WHITE);
            DrawRectangle(
                (WIDTH - BALL_SIZE) / 2,
                Lerp(0, HEIGHT -  BALL_SIZE, (float) i / (ITESM - 1)),
                BALL_SIZE, BALL_SIZE, WHITE);
        }
        // Draw text

    EndMode2D();
}

// =========== Main ===========

int main(void) {

    InitWindow(WIDTH, HEIGHT, "pong");

    size_t PARTICLES_SIZE_BYTE = sizeof(Particle) * NO_OF_PARTICLES;
    Particle *particles = RL_MALLOC(PARTICLES_SIZE_BYTE);

    printf("size of particles %lu bytes\n", PARTICLES_SIZE_BYTE);
    printf("No of particles %d\n", NO_OF_PARTICLES);

    for (int i = 0; i < NO_OF_PARTICLES; ++i) {
        Particle *p = &particles[i];
        p->id = GenerateRandomId();
    }
    CreateSimpleParticleEffect(particles);

    // =========== Camera ===========
    camera.offset = (Vector2) { WIDTH / 2, HEIGHT / 2 };
    camera.target = (Vector2) { WIDTH / 2, HEIGHT / 2 };
    camera.zoom = 1.0f;
    // camera.rotation = 45.0f;
    camera.rotation = 0.0f;

    camera_shake.offset = (Vector2) { WIDTH / 2, HEIGHT / 2 };
    camera_shake.target = (Vector2) { WIDTH / 2, HEIGHT / 2 };
    camera_shake.zoom = 1.0f;
    camera_shake.rotation = 0.0f;

    // =========== Screen shake ===========
    screenShake.shake = 0.0f;
    screenShake.maxAngle = 10.0f;
    screenShake.maxOffset = 10.0f;
    screenShake.angle   = 0.0f;
    screenShake.offsetX = 0.0f;
    screenShake.offsetY = 0.0f;


    SetTargetFPS(60);
    while (!WindowShouldClose()) {

        Input();

        Update(particles);

        BeginDrawing();
            ClearBackground(BLACK);

            Draw();
            DrawGUI();
            DrawParticles(particles);

            DrawFPS(20,20);
        EndDrawing();

    }

    RL_FREE(particles);

    CloseWindow();

    return 0;
}
