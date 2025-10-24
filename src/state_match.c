#include "state_match.h"

#include <raylib.h>
#include <stddef.h>
#include <stdlib.h>

#include "input_handler.h"

#define GRID_COLUMNS 6
#define GRID_ROWS 12

static const int CURSOR_LINE = 3;
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
    case GEM_FIRE:
      return RED;
    case GEM_WATER:
      return BLUE;
    case GEM_GRASS:
      return GREEN;
    case GEM_ROCK:
      return BROWN;
    default:
      return RAYWHITE;
  }
};

static Gem gem_grid[GRID_ROWS][GRID_COLUMNS];

static struct {
  int row;
  int column;
} cursor = {GRID_ROWS / 2, GRID_COLUMNS / 2};

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
        gem_grid[i][j] = rand() % GEM_COUNT;  // Can be any Gem
      } else {
        gem_grid[i][j] = 1 + rand() % (GEM_COUNT - 1);  // Cannot be empty
      }
    }
  }
}

Rectangle RenderGridBox(void) {
  const int grid_height = GRID_HEIGHT + GRID_LINE * 2;
  const int grid_width = GRID_WIDTH + GRID_LINE * 2;
  const Rectangle rect = {
      .x = GetScreenWidth() / 2 - grid_width / 2,
      .y = GetScreenHeight() / 2 - grid_height / 2,
      .width = grid_width,
      .height = grid_height,
  };

  DrawRectangleLinesEx(rect, GRID_LINE, BLACK);

  return rect;
}

void RenderGridGems(const Rectangle* grid_rect) {
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLUMNS; j++) {
      const Gem gem = gem_grid[i][j];
      if (gem == GEM_EMPTY) {
        continue;
      }

      DrawRectangle(grid_rect->x + (TILE_SIZE * j) + GRID_LINE,
                    grid_rect->y + (TILE_SIZE * i) + GRID_LINE,
                    TILE_SIZE,
                    TILE_SIZE,
                    GemToColor(gem));
    }
  }
}

void RenderCursor(const Rectangle* grid_rect) {
  const float x = grid_rect->x + GRID_LINE + (cursor.column * TILE_SIZE);
  const float y = grid_rect->y + GRID_LINE + (cursor.row * TILE_SIZE);

  const Rectangle cursor_rect = {
      .x = x,
      .y = y,
      .width = TILE_SIZE,
      .height = TILE_SIZE,
  };

  const Rectangle swap_rect = {
      .x = x + TILE_SIZE,
      .y = y,
      .width = TILE_SIZE,
      .height = TILE_SIZE,
  };

  DrawRectangleLinesEx(cursor_rect, CURSOR_LINE, BLACK);
  DrawRectangleLinesEx(swap_rect, CURSOR_LINE, BLACK);
}

void MoveCursor(void) {
  if (Input_Pressed(ACTION_UP) && cursor.row > 0) {
    cursor.row--;
  }
  if (Input_Pressed(ACTION_DOWN) && cursor.row < GRID_ROWS - 1) {
    cursor.row++;
  }
  if (Input_Pressed(ACTION_LEFT) && cursor.column > 0) {
    cursor.column--;
  }
  if (Input_Pressed(ACTION_RIGHT) && cursor.column < GRID_COLUMNS - 2) {
    cursor.column++;
  }
}

void SwapGems(void) {
  if (!Input_Pressed(ACTION_SWAP)) {
    return;
  }

  const Gem gem = gem_grid[cursor.row][cursor.column];
  gem_grid[cursor.row][cursor.column] = gem_grid[cursor.row][cursor.column + 1];
  gem_grid[cursor.row][cursor.column + 1] = gem;
}

void CheckMatches(void) {
  bool marked[GRID_ROWS][GRID_COLUMNS] = {false};

  // --- Horizontal ---
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLUMNS - 2; j++) {
      Gem current = gem_grid[i][j];
      if (current == GEM_EMPTY) continue;

      if (gem_grid[i][j + 1] == current && gem_grid[i][j + 2] == current) {
        marked[i][j] = true;
        marked[i][j + 1] = true;
        marked[i][j + 2] = true;
      }
    }
  }

  // --- Vertical ---
  for (int i = 0; i < GRID_ROWS - 2; i++) {
    for (int j = 0; j < GRID_COLUMNS; j++) {
      Gem current = gem_grid[i][j];
      if (current == GEM_EMPTY) continue;

      if (gem_grid[i + 1][j] == current && gem_grid[i + 2][j] == current) {
        marked[i][j] = true;
        marked[i + 1][j] = true;
        marked[i + 2][j] = true;
      }
    }
  }

  // --- Remove ---
  for (int i = 0; i < GRID_ROWS; i++) {
    for (int j = 0; j < GRID_COLUMNS; j++) {
      if (marked[i][j]) {
        gem_grid[i][j] = GEM_EMPTY;
      }
    }
  }
}

void ApplyGravity(void) {
  for (int j = 0; j < GRID_COLUMNS; j++) {
    for (int i = GRID_ROWS - 1; i >= 0; i--) {
      if (gem_grid[i][j] == GEM_EMPTY) {
        for (int k = i - 1; k >= 0; k--) {
          if (gem_grid[k][j] != GEM_EMPTY) {
            gem_grid[i][j] = gem_grid[k][j];
            gem_grid[k][j] = GEM_EMPTY;
            break;
          }
        }
      }
    }
  }
}

void match_enter(void* _) {
  FillGrid();
};

void match_update(const float _) {
  CheckMatches();
  ApplyGravity();
  MoveCursor();
  SwapGems();
};

void match_render(void) {
  ClearBackground(RAYWHITE);
  Rectangle grid_rect = RenderGridBox();
  RenderGridGems(&grid_rect);
  RenderCursor(&grid_rect);
};

void match_exit(void) {};

State MatchState = {
    .enter = match_enter,
    .update = match_update,
    .render = match_render,
    .exit = match_exit,
};
