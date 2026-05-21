#ifndef START_MENU_H
#define START_MENU_H

#include "utils.h"
#include "raylib.h"

typedef struct {
    char player_name[NAME_LEN];
    int selected_mode;
    int selected_diff;
    int start_game;
    int name_active;
    float error_timer;
    char error_msg[256];
    int show_leaderboard;
} StartMenu;

void init_start_menu(StartMenu *menu);
void draw_start_menu(StartMenu *menu);
void handle_start_menu_input(StartMenu *menu, Vector2 mouse, int key);

#endif
