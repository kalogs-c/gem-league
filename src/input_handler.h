#pragma once
#include <stdbool.h>

typedef enum {
  ACTION_UP,
  ACTION_DOWN,
  ACTION_LEFT,
  ACTION_RIGHT,
  ACTION_SWAP,
  ACTION_SELECT,
  ACTION_BACK,
  INPUT_ACTION_COUNT,
} InputAction;

bool Input_Pressed(InputAction action);

bool Input_Down(InputAction action);
