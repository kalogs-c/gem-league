#include "state_match.h"

#include <raylib.h>
#include <stddef.h>
#include <stdlib.h>

#include "input_handler.h"
#include "state_main_menu.h"
#include "state_stack.h"

#define GRID_COLUMNS 6
#define GRID_ROWS 12

static const int initial_row = GRID_ROWS / 2;
static const int CURSOR_LINE = 3;
static const int GRID_LINE = 5;
static const int TILE_SIZE = 50;
static const int GRID_WIDTH = TILE_SIZE * GRID_COLUMNS;
static const int GRID_HEIGHT = TILE_SIZE * GRID_ROWS;
static const float ROW_SPAWN_INTERVAL = 3.0f;

static float row_spawn_timer = ROW_SPAWN_INTERVAL;

typedef enum {
  GEM_EMPTY,
  GEM_FIRE,
  GEM_WATER,
  GEM_GRASS,
  GEM_ROCK,
  GEM_COUNT,
} Gem;

static Gem RandomGem(const bool allow_empty) {
  if (allow_empty) {
    return rand() % GEM_COUNT;
  } else {
    return 1 + rand() % (GEM_COUNT - 1);
  }
}

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

static void FillGrid(void) {
  for (int row = initial_row; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLUMNS; col++) {
      if (row == initial_row) {
        gem_grid[row][col] = RandomGem(true);
        continue;
      }

      const Gem above = gem_grid[row - 1][col];
      if (above == GEM_EMPTY) {
        gem_grid[row][col] = RandomGem(true);  // Can be any Gem
      } else {
        gem_grid[row][col] = RandomGem(false);  // Cannot be empty
      }
    }
  }
}

static void ClearGrid(void) {
  for (int row = 0; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLUMNS; col++) {
      gem_grid[row][col] = GEM_EMPTY;
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
  for (int row = 0; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLUMNS; col++) {
      const Gem gem = gem_grid[row][col];
      if (gem == GEM_EMPTY) {
        continue;
      }

      DrawRectangle(grid_rect->x + (TILE_SIZE * col) + GRID_LINE,
                    grid_rect->y + (TILE_SIZE * row) + GRID_LINE,
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
  for (int row = 0; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLUMNS - 2; col++) {
      Gem current = gem_grid[row][col];
      if (current == GEM_EMPTY) continue;

      if (gem_grid[row][col + 1] == current && gem_grid[row][col + 2] == current) {
        marked[row][col] = true;
        marked[row][col + 1] = true;
        marked[row][col + 2] = true;
      }
    }
  }

  // --- Vertical ---
  for (int row = 0; row < GRID_ROWS - 2; row++) {
    for (int col = 0; col < GRID_COLUMNS; col++) {
      Gem current = gem_grid[row][col];
      if (current == GEM_EMPTY) continue;

      if (gem_grid[row + 1][col] == current && gem_grid[row + 2][col] == current) {
        marked[row][col] = true;
        marked[row + 1][col] = true;
        marked[row + 2][col] = true;
      }
    }
  }

  // --- Remove ---
  for (int row = 0; row < GRID_ROWS; row++) {
    for (int col = 0; col < GRID_COLUMNS; col++) {
      if (marked[row][col]) {
        gem_grid[row][col] = GEM_EMPTY;
      }
    }
  }
}

void ApplyGravity(void) {
  for (int col = 0; col < GRID_COLUMNS; col++) {
    for (int row = GRID_ROWS - 1; row >= 0; row--) {
      if (gem_grid[row][col] == GEM_EMPTY) {
        for (int below = row - 1; below >= 0; below--) {
          if (gem_grid[below][col] != GEM_EMPTY) {
            gem_grid[row][col] = gem_grid[below][col];
            gem_grid[below][col] = GEM_EMPTY;
            break;
          }
        }
      }
    }
  }
}

void GenerateGems(const float dt) {
  row_spawn_timer -= dt;
  if (row_spawn_timer > 0) {
    return;
  }

  for (int row = 0; row < GRID_ROWS - 1; row++) {
    for (int col = 0; col < GRID_COLUMNS; col++) {
      gem_grid[row][col] = gem_grid[row + 1][col];
    }
  }

  for (int col = 0; col < GRID_COLUMNS; col++) {
    gem_grid[GRID_ROWS - 1][col] = RandomGem(false);
  }

  row_spawn_timer = ROW_SPAWN_INTERVAL;
}

bool CheckGameOver(void) {
  for (int col = 0; col < GRID_COLUMNS; col++) {
    if (gem_grid[0][col] != GEM_EMPTY) {
      return true;
    }
  }

  return false;
}

void match_enter(void* _) {
  row_spawn_timer = ROW_SPAWN_INTERVAL;
  FillGrid();
};

void match_update(const float dt) {
  CheckMatches();
  ApplyGravity();
  MoveCursor();
  SwapGems();
  GenerateGems(dt);

  if (CheckGameOver()) {
    StateStack_Pop();
    StateStack_Push(&MainMenuState, NULL);
  };
};

void match_render(void) {
  ClearBackground(RAYWHITE);
  Rectangle grid_rect = RenderGridBox();
  RenderGridGems(&grid_rect);
  RenderCursor(&grid_rect);
};

void match_exit(void) {
  ClearGrid();
};

State MatchState = {
    .enter = match_enter,
    .update = match_update,
    .render = match_render,
    .exit = match_exit,
};
