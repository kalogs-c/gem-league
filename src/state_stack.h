#pragma once

#include "state.h"

void StateStack_Push(State* state);

void StateStack_Pop(void);

void StateStack_Update(float dt);

void StateStack_Render(void);
