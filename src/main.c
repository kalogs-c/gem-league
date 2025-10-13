#include "raylib.h"
#include "settings.h"
#include "state_main_menu.h"
#include "state_stack.h"

int main(void) {
    InitWindow(0, 0, "Gem League");
    InitAudioDevice();

    LoadSettings();
    ApplySettings();

    StateStack_Push(MainMenuState());

    while (!WindowShouldClose()) {
        const float dt = GetFrameTime();
        StateStack_Update(dt);

        BeginDrawing();
        StateStack_Render();
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
