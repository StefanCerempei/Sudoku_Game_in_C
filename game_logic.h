#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "utils.h"

typedef struct {
    int value;
    int given;
    int error;
} Cell;

typedef struct {
    Cell **cells;
    int **solution;
    Difficulty difficulty;
    int mistakes;
    time_t start_time;
    int selected_row;
    int selected_col;
    int game_over;
    int win;
    int game_mode;
} Grid;

void grid_init(Grid *g, int mode);
void grid_free(Grid *g);
int is_valid_placement(const Grid *g, int row, int col, int num);
int place_number(Grid *g, int row, int col, int num);
void erase_cell(Grid *g, int row, int col);
void check_errors(Grid *g);
int is_complete(const Grid *g);
int is_solved(const Grid *g);
int get_elapsed_seconds(const Grid *g);
void reset_game(Grid *g, Difficulty diff, int mode);

#endif
