#include "game_logic.h"

/* ─── Initialization ────────────────────────────────────────────── */
void grid_init(Grid *g)
{
    memset(g, 0, sizeof(Grid));
    g->selected_row = 0;
    g->selected_col = 0;
    g->start_time   = time(NULL);
}

/* ─── Validation ────────────────────────────────────────────────── */
int is_valid_placement(const Grid *g, int row, int col, int num)
{
    if (num < 1 || num > 9) return 0;

    /* Check row */
    for (int c = 0; c < GRID_SIZE; c++)
        if (c != col && g->cells[row][c].value == num)
            return 0;

    /* Check column */
    for (int r = 0; r < GRID_SIZE; r++)
        if (r != row && g->cells[r][col].value == num)
            return 0;

    /* Check 3×3 box */
    int br = (row / BOX_SIZE) * BOX_SIZE;
    int bc = (col / BOX_SIZE) * BOX_SIZE;
    for (int r = br; r < br + BOX_SIZE; r++)
        for (int c = bc; c < bc + BOX_SIZE; c++)
            if ((r != row || c != col) && g->cells[r][c].value == num)
                return 0;

    return 1;
}

/* ─── Place a number (returns 1 = ok, 0 = invalid, -1 = given) ─── */
int place_number(Grid *g, int row, int col, int num)
{
    if (g->cells[row][col].given) return -1;

    g->cells[row][col].value = num;

    /* Compare with solution */
    if (num != 0 && num != g->solution[row][col]) {
        g->cells[row][col].error = 1;
        g->mistakes++;
    } else {
        g->cells[row][col].error = 0;
    }

    check_errors(g);
    return is_valid_placement(g, row, col, num) ? 1 : 0;
}

/* ─── Erase ─────────────────────────────────────────────────────── */
void erase_cell(Grid *g, int row, int col)
{
    if (g->cells[row][col].given) return;
    g->cells[row][col].value = EMPTY;
    g->cells[row][col].error = 0;
    check_errors(g);
}

/* ─── Re-evaluate all error flags ──────────────────────────────── */
void check_errors(Grid *g)
{
    for (int r = 0; r < GRID_SIZE; r++)
        for (int c = 0; c < GRID_SIZE; c++)
            if (!g->cells[r][c].given && g->cells[r][c].value != EMPTY)
                g->cells[r][c].error =
                    (g->cells[r][c].value != g->solution[r][c]) ? 1 : 0;
}

/* ─── Completion check ──────────────────────────────────────────── */
int is_complete(const Grid *g)
{
    for (int r = 0; r < GRID_SIZE; r++)
        for (int c = 0; c < GRID_SIZE; c++)
            if (g->cells[r][c].value == EMPTY) return 0;
    return 1;
}

int is_solved(const Grid *g)
{
    for (int r = 0; r < GRID_SIZE; r++)
        for (int c = 0; c < GRID_SIZE; c++)
            if (g->cells[r][c].value != g->solution[r][c]) return 0;
    return 1;
}

/* ─── Timer ─────────────────────────────────────────────────────── */
int get_elapsed_seconds(const Grid *g)
{
    return (int)(time(NULL) - g->start_time);
}

/* ─── Copy solution into cells (for "reveal" / new game) ────────── */
void grid_copy_solution(Grid *g)
{
    for (int r = 0; r < GRID_SIZE; r++)
        for (int c = 0; c < GRID_SIZE; c++) {
            g->cells[r][c].value = g->solution[r][c];
            g->cells[r][c].error = 0;
        }
}