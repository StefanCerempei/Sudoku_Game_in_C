#include "generator.h"

/* ─── Fisher-Yates shuffle for int array ───────────────────────── */
static void shuffle(int *arr, int n)
{
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
    }
}

/* ─── Check raw board (no Grid wrapper) ────────────────────────── */
static int raw_valid(int board[GRID_SIZE][GRID_SIZE], int r, int c, int num)
{
    for (int i = 0; i < GRID_SIZE; i++) {
        if (board[r][i] == num) return 0;
        if (board[i][c] == num) return 0;
    }
    int br = (r / BOX_SIZE) * BOX_SIZE;
    int bc = (c / BOX_SIZE) * BOX_SIZE;
    for (int i = br; i < br + BOX_SIZE; i++)
        for (int j = bc; j < bc + BOX_SIZE; j++)
            if (board[i][j] == num) return 0;
    return 1;
}

/* ─── Recursive fill with randomised digit order ───────────────── */
static int fill_recursive(int board[GRID_SIZE][GRID_SIZE], int pos)
{
    if (pos == GRID_SIZE * GRID_SIZE) return 1;

    int r = pos / GRID_SIZE, c = pos % GRID_SIZE;
    if (board[r][c] != EMPTY) return fill_recursive(board, pos + 1);

    int digits[9] = {1,2,3,4,5,6,7,8,9};
    shuffle(digits, 9);

    for (int i = 0; i < 9; i++) {
        if (raw_valid(board, r, c, digits[i])) {
            board[r][c] = digits[i];
            if (fill_recursive(board, pos + 1)) return 1;
            board[r][c] = EMPTY;
        }
    }
    return 0;
}

/* ─── Public: fill board ────────────────────────────────────────── */
void fill_board(int board[GRID_SIZE][GRID_SIZE])
{
    memset(board, 0, GRID_SIZE * GRID_SIZE * sizeof(int));
    fill_recursive(board, 0);
}

/* ─── Solver (counts solutions, stops at 2) ─────────────────────── */
static int solve_count;

static int solve_recursive(int board[GRID_SIZE][GRID_SIZE], int pos)
{
    if (solve_count > 1) return 0;
    if (pos == GRID_SIZE * GRID_SIZE) { solve_count++; return 1; }

    int r = pos / GRID_SIZE, c = pos % GRID_SIZE;
    if (board[r][c] != EMPTY) return solve_recursive(board, pos + 1);

    for (int num = 1; num <= 9; num++) {
        if (raw_valid(board, r, c, num)) {
            board[r][c] = num;
            solve_recursive(board, pos + 1);
            board[r][c] = EMPTY;
            if (solve_count > 1) return 0;
        }
    }
    return 0;
}

/* Returns 1 if board has a unique solution */
static int has_unique_solution(int board[GRID_SIZE][GRID_SIZE])
{
    int tmp[GRID_SIZE][GRID_SIZE];
    memcpy(tmp, board, sizeof(tmp));
    solve_count = 0;
    solve_recursive(tmp, 0);
    return (solve_count == 1);
}

/* Public solve (fills board in-place, returns 1 on success) */
int solve_board(int board[GRID_SIZE][GRID_SIZE])
{
    solve_count = 0;
    return fill_recursive(board, 0);
}

/* ─── Remove cells while maintaining unique solution ────────────── */
void remove_cells(Grid *g, int count)
{
    /* Build a shuffled list of all 81 positions */
    int positions[GRID_SIZE * GRID_SIZE];
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) positions[i] = i;
    shuffle(positions, GRID_SIZE * GRID_SIZE);

    int removed = 0;
    for (int k = 0; k < GRID_SIZE * GRID_SIZE && removed < count; k++) {
        int r = positions[k] / GRID_SIZE;
        int c = positions[k] % GRID_SIZE;

        int backup = g->cells[r][c].value;
        g->cells[r][c].value = EMPTY;
        g->cells[r][c].given = 0;

        /* Build a raw int board to test uniqueness */
        int tmp[GRID_SIZE][GRID_SIZE];
        for (int i = 0; i < GRID_SIZE; i++)
            for (int j = 0; j < GRID_SIZE; j++)
                tmp[i][j] = g->cells[i][j].value;

        if (has_unique_solution(tmp)) {
            removed++;
        } else {
            /* Restore */
            g->cells[r][c].value = backup;
            g->cells[r][c].given = 1;
        }
    }
}

/* ─── Main public entry ─────────────────────────────────────────── */
void generate_puzzle(Grid *g, Difficulty diff)
{
    grid_init(g);
    g->difficulty = diff;

    /* 1. Fill a complete solution */
    fill_board(g->solution);

    /* 2. Copy solution into cells (all given) */
    for (int r = 0; r < GRID_SIZE; r++)
        for (int c = 0; c < GRID_SIZE; c++) {
            g->cells[r][c].value = g->solution[r][c];
            g->cells[r][c].given = 1;
            g->cells[r][c].error = 0;
        }

    /* 3. Remove cells according to difficulty */
    int to_remove = (diff == EASY)   ? EASY_REMOVE   :
                    (diff == MEDIUM)  ? MEDIUM_REMOVE  :
                                        HARD_REMOVE;
    remove_cells(g, to_remove);

    g->start_time = time(NULL);
}