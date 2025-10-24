#include "input_handler.h"

#include <raylib.h>

static int actions_key[INPUT_ACTION_COUNT] = {
    [ACTION_UP] = KEY_UP,
    [ACTION_DOWN] = KEY_DOWN,
    [ACTION_LEFT] = KEY_LEFT,
    [ACTION_RIGHT] = KEY_RIGHT,
    [ACTION_SWAP] = KEY_SPACE,
    [ACTION_SELECT] = KEY_ENTER,
    [ACTION_BACK] = KEY_ESCAPE,
};

bool Input_Pressed(const InputAction action) {
  return IsKeyPressed(actions_key[action]);
};

bool Input_Down(const InputAction action) {
  return IsKeyDown(actions_key[action]);
};
