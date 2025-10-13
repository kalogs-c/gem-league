#include <raylib.h>

#include "state.h"

#include "input_handler.h"

typedef enum {
    MENU_PLAY,
    MENU_SETTINGS,
    MENU_EXIT,
    MENU_COUNT,
} MenuOption;

static int selected_item = MENU_PLAY;
static const char* menu_items[MENU_COUNT] = {
    "Play",
    "Settings",
    "Exit"
};

static Color item_color = RAYWHITE;
static Color selected_item_color = YELLOW;

static int font_size = 40;
static int spacing = 40;

void main_menu_enter(void) {
    selected_item = MENU_PLAY;
};

void main_menu_update(const float _) {
    if (Input_Pressed(ACTION_UP)) {
        selected_item--;
        if (selected_item < 0) {
            selected_item = MENU_COUNT - 1;
        }
    }

    if (Input_Pressed(ACTION_DOWN)) {
        selected_item++;
        if (selected_item >= MENU_COUNT) {
            selected_item = MENU_PLAY;
        }
    }
};

void main_menu_render(void) {
    ClearBackground(DARKGREEN);

    const int screen_width = GetScreenWidth();
    const int screen_height = GetScreenHeight();

    const int start_y = screen_height / 2;

    for (int i = 0; i < MENU_COUNT; i++) {
        const Color color = i == selected_item ? selected_item_color : item_color;
        const int text_width = MeasureText(menu_items[i], font_size);
        const int x = (screen_width - text_width) / 2;
        const int y = start_y + i * (spacing + font_size);

        DrawText(menu_items[i], x, y, font_size, color);
    }
};

void main_menu_exit(void) {
    selected_item = MENU_PLAY;
};

State main_menu = {
    .enter = main_menu_enter,
    .update = main_menu_update,
    .render = main_menu_render,
    .exit = main_menu_exit,
};

State* MainMenuState() {
    return &main_menu;
}
