#include "raylib.h"

// --------------------
#define MAX_SOUNDS 10
Sound soundArray[MAX_SOUNDS] = { 0 };
int currentSound;
// --------------------

Sound click, select, stop, popup;

void LoadSFX() {
    InitAudioDevice();
    click = LoadSound("resources/sfx/button_click.wav");
    select = LoadSound("resources/sfx/piece_select.wav");
    stop = LoadSound("resources/sfx/piece_stop.wav");
    popup = LoadSound("resources/sfx/popup.wav");

    currentSound = 0;
}

void PlaySFX(Sound sound) {
    PlaySound(sound);
}

void UnloadSFX() {
    UnloadSound(click);
    UnloadSound(select);
    UnloadSound(stop);
    UnloadSound(popup);
    CloseAudioDevice();
}
