#ifndef GENERATOR_H
#define GENERATOR_H

#include "game_logic.h"

void generate_puzzle(Grid *g, Difficulty diff);
int solve_board(int **board);
void fill_board(int **board);

#endif
