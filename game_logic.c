#include "game_logic.h"
#include "generator.h"

void grid_init(Grid *g, int mode) {
    memset(g, 0, sizeof(Grid));
    g->selected_row = 0;
    g->selected_col = 0;
    g->start_time = time(NULL);
    g->game_over = 0;
    g->win = 0;
    g->game_mode = mode;
    
    // Alocare memorie pentru cells și solution
    g->cells = (Cell**)malloc(GRID_SIZE * sizeof(Cell*));
    g->solution = (int**)malloc(GRID_SIZE * sizeof(int*));
    for (int i = 0; i < GRID_SIZE; i++) {
        g->cells[i] = (Cell*)calloc(GRID_SIZE, sizeof(Cell));
        g->solution[i] = (int*)calloc(GRID_SIZE, sizeof(int));
    }
}

void grid_free(Grid *g) {
    if (g->cells) {
        for (int i = 0; i < GRID_SIZE; i++) {
            free(g->cells[i]);
            free(g->solution[i]);
        }
        free(g->cells);
        free(g->solution);
    }
}

int is_valid_placement(const Grid *g, int row, int col, int num) {
    if (num < 1 || num > MAX_DIGIT) return 0;

    for (int c = 0; c < GRID_SIZE; c++)
        if (c != col && g->cells[row][c].value == num)
            return 0;

    for (int r = 0; r < GRID_SIZE; r++)
        if (r != row && g->cells[r][col].value == num)
            return 0;

    int br = (row / BOX_SIZE) * BOX_SIZE;
    int bc = (col / BOX_SIZE) * BOX_SIZE;
    for (int r = br; r < br + BOX_SIZE; r++)
        for (int c = bc; c < bc + BOX_SIZE; c++)
            if ((r != row || c != col) && g->cells[r][c].value == num)
                return 0;

    return 1;
}

int place_number(Grid *g, int row, int col, int num) {
    if (g->game_over || g->win) return -1;
    if (g->cells[row][col].given) return -1;

    g->cells[row][col].value = num;

    if (num != 0 && num != g->solution[row][col]) {
        g->cells[row][col].error = 1;
        g->mistakes++;
    } else {
        g->cells[row][col].error = 0;
    }

    check_errors(g);
    if (is_solved(g)) g->win = 1;
    return 1;
}

void erase_cell(Grid *g, int row, int col) {
    if (g->game_over || g->win) return;
    if (g->cells[row][col].given) return;
    g->cells[row][col].value = EMPTY;
    g->cells[row][col].error = 0;
    check_errors(g);
}

void check_errors(Grid *g) {
    for (int r = 0; r < GRID_SIZE; r++)
        for (int c = 0; c < GRID_SIZE; c++)
            if (!g->cells[r][c].given && g->cells[r][c].value != EMPTY)
                g->cells[r][c].error = (g->cells[r][c].value != g->solution[r][c]);
}

int is_complete(const Grid *g) {
    for (int r = 0; r < GRID_SIZE; r++)
        for (int c = 0; c < GRID_SIZE; c++)
            if (g->cells[r][c].value == EMPTY) return 0;
    return 1;
}

int is_solved(const Grid *g) {
    for (int r = 0; r < GRID_SIZE; r++)
        for (int c = 0; c < GRID_SIZE; c++)
            if (g->cells[r][c].value != g->solution[r][c]) return 0;
    return 1;
}

int get_elapsed_seconds(const Grid *g) {
    return (int)(time(NULL) - g->start_time);
}

void reset_game(Grid *g, Difficulty diff, int mode) {
    grid_free(g);
    grid_init(g, mode);
    generate_puzzle(g, diff);
    g->difficulty = diff;
    g->mistakes = 0;
    g->game_over = 0;
    g->win = 0;
    g->start_time = time(NULL);
    g->selected_row = 0;
    g->selected_col = 0;
}
