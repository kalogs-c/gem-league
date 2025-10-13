#include "settings.h"
#include <raylib.h>

static int screen_width;
static int screen_height;
static int target_fps;
static float master_volume;
static bool fullscreen;

void LoadSettings() {
    screen_width = 1280;
    screen_height = 720;
    target_fps = 60;
    master_volume = 1.0f;
    fullscreen = false;
}

void ApplySettings() {
    if (fullscreen) {
        SetWindowState(FLAG_FULLSCREEN_MODE);
    } else {
        ClearWindowState(FLAG_FULLSCREEN_MODE);
    }

    SetTargetFPS(target_fps);
    SetMasterVolume(master_volume);
    SetWindowSize(screen_width, screen_height);
}
