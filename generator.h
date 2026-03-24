#ifndef GENERATOR_H
#define GENERATOR_H

#include "game_logic.h"

/* Generate a complete, valid Sudoku puzzle at the given difficulty.
   Fills g->solution[] and g->cells[] (with clues marked as given). */
void generate_puzzle(Grid *g, Difficulty diff);

/* Internal helpers (exposed for testing) */
int  solve_board   (int board[GRID_SIZE][GRID_SIZE]);
void fill_board    (int board[GRID_SIZE][GRID_SIZE]);
void remove_cells  (Grid *g, int count);

#endif /* GENERATOR_H */