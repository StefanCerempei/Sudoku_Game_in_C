#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "utils.h"

/* ─── Cell ──────────────────────────────────────────────────────── */
typedef struct {
    int  value;       /* 0 = empty, 1-9 = digit          */
    int  given;       /* 1 = fixed clue, 0 = user cell   */
    int  error;       /* 1 = conflicts with rules        */
} Cell;

/* ─── Grid ──────────────────────────────────────────────────────── */
typedef struct {
    Cell      cells[GRID_SIZE][GRID_SIZE];
    int       solution[GRID_SIZE][GRID_SIZE]; /* full solved grid   */
    Difficulty difficulty;
    int       mistakes;   /* total wrong placements         */
    time_t    start_time; /* for elapsed timer              */
    int       selected_row;
    int       selected_col;
} Grid;

/* ─── Prototypes ────────────────────────────────────────────────── */
void grid_init          (Grid *g);
int  is_valid_placement (const Grid *g, int row, int col, int num);
int  is_complete        (const Grid *g);
int  place_number       (Grid *g, int row, int col, int num);
void erase_cell         (Grid *g, int row, int col);
void check_errors       (Grid *g);
int  is_solved          (const Grid *g);
int  get_elapsed_seconds(const Grid *g);
void grid_copy_solution (Grid *g); /* copy solution[] → cells[]  */

#endif /* GAME_LOGIC_H */