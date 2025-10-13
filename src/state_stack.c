#include "state_stack.h"

#include <assert.h>
#include <stddef.h>


#define STACK_SIZE 8

struct {
    State* stack[STACK_SIZE];
    int top;
} StateStack = {
    .top = -1,
};

void StateStack_Push(State* state) {
    assert(StateStack.top < STACK_SIZE && "state stack overflow");
    StateStack.top++;
    StateStack.stack[StateStack.top] = state;
}

void StateStack_Pop(void) {
    assert(StateStack.top >= 0 && "state state_stack underflow");

    const State* state = StateStack.stack[StateStack.top];
    if (state->exit) {
        state->exit();
    }

    StateStack.stack[StateStack.top] = NULL;
    StateStack.top--;
}

void StateStack_Update(const float dt) {
    assert(StateStack.top >= 0 && "state stack underflow");
    assert(StateStack.stack[StateStack.top] != NULL);
    const State* state = StateStack.stack[StateStack.top];
    state->update(dt);
}

void StateStack_Render(void) {
    assert(StateStack.top >= 0 && "state stack underflow");

    for (int i = 0; i <= StateStack.top; i++) {
        assert(StateStack.stack[i] != NULL && "trying to draw empty state");
        const State* state = StateStack.stack[i];
        state->render();
    }
}
