#pragma once

typedef struct {
    void (*enter)(void* context);
    void (*update)(float dt);
    void (*render)(void);
    void (*exit)(void);
} State;