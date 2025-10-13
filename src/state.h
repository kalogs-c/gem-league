#pragma once

typedef struct {
    void (*enter)(void);
    void (*update)(float dt);
    void (*render)(void);
    void (*exit)(void);
} State;