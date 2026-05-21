#include "save_game.h"
#include <stdio.h>

void save_game(const Grid *g, const char *player_name) {
    FILE *f = fopen(SAVE_FILE, "w");
    if (!f) return;
    
    // Scrie header
    fprintf(f, "SUDOKU_SAVE_V1\n");
    fprintf(f, "%d,%d,%d,%d,%d,%d\n", 
            g->game_mode, 
            g->difficulty,
            g->mistakes,
            get_elapsed_seconds(g),
            g->selected_row,
            g->selected_col);
    
    // Scrie numele jucătorului
    fprintf(f, "%s\n", player_name);
    
    // Scrie gridul și soluția
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            fprintf(f, "%d,%d,%d", 
                    g->cells[r][c].value,
                    g->cells[r][c].given ? 1 : 0,
                    g->solution[r][c]);
            if (c < GRID_SIZE - 1) fprintf(f, ",");
        }
        fprintf(f, "\n");
    }
    
    fclose(f);
    printf("Game saved to %s\n", SAVE_FILE);
}

int load_game(Grid *g, char *player_name) {
    FILE *f = fopen(SAVE_FILE, "r");
    if (!f) return 0;
    
    char header[20];
    fscanf(f, "%s\n", header);
    if (strcmp(header, "SUDOKU_SAVE_V1") != 0) {
        fclose(f);
        return 0;
    }
    
    int game_mode, difficulty, mistakes, elapsed, selected_row, selected_col;
    fscanf(f, "%d,%d,%d,%d,%d,%d\n", 
           &game_mode, &difficulty, &mistakes, &elapsed, &selected_row, &selected_col);
    
    fscanf(f, "%s\n", player_name);
    
    // Setează modul de joc înainte să aloce gridul
    g->game_mode = game_mode;
    
    // Inițializează gridul cu dimensiunile corecte
    grid_init(g, game_mode);
    g->difficulty = (Difficulty)difficulty;
    g->mistakes = mistakes;
    g->selected_row = selected_row;
    g->selected_col = selected_col;
    g->start_time = time(NULL) - elapsed;
    g->game_over = 0;
    g->win = 0;
    
    // Citește gridul
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            int val, given, sol;
            fscanf(f, "%d,%d,%d", &val, &given, &sol);
            g->cells[r][c].value = val;
            g->cells[r][c].given = given;
            g->solution[r][c] = sol;
            if (c < GRID_SIZE - 1) fscanf(f, ",");
        }
        fscanf(f, "\n");
    }
    
    fclose(f);
    return 1;
}

int save_exists(void) {
    FILE *f = fopen(SAVE_FILE, "r");
    if (f) {
        fclose(f);
        return 1;
    }
    return 0;
}

void delete_save(void) {
    remove(SAVE_FILE);
}
