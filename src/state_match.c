#include "state_match.h"

#include <raylib.h>
#include <stdlib.h>

#define GRID_COLUMNS 6
#define GRID_ROWS 12

static const int GRID_LINE = 5;
static const int TILE_SIZE = 50;
static const int GRID_WIDTH = TILE_SIZE * GRID_COLUMNS;
static const int GRID_HEIGHT = TILE_SIZE * GRID_ROWS;

typedef enum {
    GEM_EMPTY,
    GEM_FIRE,
    GEM_WATER,
    GEM_GRASS,
    GEM_ROCK,
    GEM_COUNT,
} Gem;

static Color GemToColor(const Gem gem) {
    switch (gem) {
        case GEM_FIRE: return RED;
        case GEM_WATER: return BLUE;
        case GEM_GRASS: return GREEN;
        case GEM_ROCK: return BROWN;
        default: return RAYWHITE;
    }
};

static Gem gem_grid[GRID_ROWS][GRID_COLUMNS];

static void FillGrid() {
    const int initial_row = GRID_ROWS / 2;

    for (int i = initial_row; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLUMNS; j++) {
            if (i == initial_row) {
                gem_grid[i][j] = rand() % GEM_COUNT;
                continue;
            }

            const Gem above = gem_grid[i - 1][j];
            if (above == GEM_EMPTY) {
                gem_grid[i][j] = rand() % GEM_COUNT; // Can be any Gem
            } else {
                gem_grid[i][j] = 1 + rand() % (GEM_COUNT - 1); // Cannot be empty
            }
        }
    }
}

void match_enter(void* _) {
    FillGrid();
};

void match_update(const float _) {
};

void match_render(void) {
    ClearBackground(RAYWHITE);

    // Grid box
    const int grid_height = GRID_HEIGHT + GRID_LINE * 2;
    const int grid_width = GRID_WIDTH + GRID_LINE * 2;
    const Rectangle GRID_RECT = {
        .x = GetScreenWidth() / 2 - grid_width / 2,
        .y = GetScreenHeight() / 2 - grid_height / 2,
        .width = grid_width,
        .height = grid_height,
    };
    DrawRectangleLinesEx(GRID_RECT, GRID_LINE, BLACK);

    // Grid Gems
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLUMNS; j++) {
            const Gem gem = gem_grid[i][j];
            if (gem == GEM_EMPTY) {
                continue;
            }

            DrawRectangle(
                GRID_RECT.x + (TILE_SIZE * j) + GRID_LINE,
                GRID_RECT.y + (TILE_SIZE * i) + GRID_LINE,
                TILE_SIZE,
                TILE_SIZE,
                GemToColor(gem)
            );
        }
    }
};

void match_exit(void) {
};

State MatchState = {
    .enter = match_enter,
    .update = match_update,
    .render = match_render,
    .exit = match_exit,
};
