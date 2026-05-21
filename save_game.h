#ifndef SAVE_GAME_H
#define SAVE_GAME_H

#include "game_logic.h"

#define SAVE_FILE "sudoku_save.csv"

typedef struct {
    int game_mode;      // MODE_CLASSIC sau MODE_SCALE_UP
    int difficulty;     // EASY, MEDIUM, HARD
    int mistakes;
    int elapsed_time;
    int selected_row;
    int selected_col;
    int grid_values[16][16];   // Valori maxim 16x16
    int grid_given[16][16];    // Dacă e given sau nu
    int solution[16][16];      // Soluția completă
    char player_name[NAME_LEN];
    int has_save;
} SaveData;

void save_game(const Grid *g, const char *player_name);
int load_game(Grid *g, char *player_name);
int save_exists(void);
void delete_save(void);

#endif
