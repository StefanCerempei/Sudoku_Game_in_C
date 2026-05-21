#include "generator.h"

static void shuffle(int *arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
    }
}

static int raw_valid(int **board, int r, int c, int num) {
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

static int fill_recursive(int **board, int pos) {
    if (pos == GRID_SIZE * GRID_SIZE) return 1;
    int r = pos / GRID_SIZE, c = pos % GRID_SIZE;
    if (board[r][c] != EMPTY) return fill_recursive(board, pos + 1);

    int *digits = malloc(MAX_DIGIT * sizeof(int));
    for (int i = 0; i < MAX_DIGIT; i++) digits[i] = i + 1;
    shuffle(digits, MAX_DIGIT);
    
    for (int i = 0; i < MAX_DIGIT; i++) {
        if (raw_valid(board, r, c, digits[i])) {
            board[r][c] = digits[i];
            if (fill_recursive(board, pos + 1)) {
                free(digits);
                return 1;
            }
            board[r][c] = EMPTY;
        }
    }
    free(digits);
    return 0;
}

void fill_board(int **board) {
    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            board[i][j] = EMPTY;
    fill_recursive(board, 0);
}

static int solve_count;
static void solve_recursive(int **board, int pos) {
    if (solve_count > 1) return;
    if (pos == GRID_SIZE * GRID_SIZE) { solve_count++; return; }
    int r = pos / GRID_SIZE, c = pos % GRID_SIZE;
    if (board[r][c] != EMPTY) { solve_recursive(board, pos + 1); return; }
    for (int num = 1; num <= MAX_DIGIT; num++) {
        if (raw_valid(board, r, c, num)) {
            board[r][c] = num;
            solve_recursive(board, pos + 1);
            board[r][c] = EMPTY;
            if (solve_count > 1) return;
        }
    }
}

static int has_unique_solution(Grid *g) {
    int **tmp = malloc(GRID_SIZE * sizeof(int*));
    for (int i = 0; i < GRID_SIZE; i++) {
        tmp[i] = malloc(GRID_SIZE * sizeof(int));
        for (int j = 0; j < GRID_SIZE; j++)
            tmp[i][j] = g->cells[i][j].value;
    }
    solve_count = 0;
    solve_recursive(tmp, 0);
    for (int i = 0; i < GRID_SIZE; i++) free(tmp[i]);
    free(tmp);
    return solve_count == 1;
}

int solve_board(int **board) {
    return fill_recursive(board, 0);
}

void generate_puzzle(Grid *g, Difficulty diff) {
    fill_board(g->solution);
    for (int r = 0; r < GRID_SIZE; r++)
        for (int c = 0; c < GRID_SIZE; c++) {
            g->cells[r][c].value = g->solution[r][c];
            g->cells[r][c].given = 1;
        }
    int to_remove = (diff == EASY) ? EASY_REMOVE : (diff == MEDIUM) ? MEDIUM_REMOVE : HARD_REMOVE;
    int total_cells = GRID_SIZE * GRID_SIZE;
    int *positions = malloc(total_cells * sizeof(int));
    for (int i = 0; i < total_cells; i++) positions[i] = i;
    shuffle(positions, total_cells);
    int removed = 0;
    for (int k = 0; k < total_cells && removed < to_remove; k++) {
        int r = positions[k] / GRID_SIZE, c = positions[k] % GRID_SIZE;
        int backup = g->cells[r][c].value;
        g->cells[r][c].value = EMPTY;
        g->cells[r][c].given = 0;
        if (has_unique_solution(g)) removed++;
        else {
            g->cells[r][c].value = backup;
            g->cells[r][c].given = 1;
        }
    }
    free(positions);
    g->start_time = time(NULL);
}
