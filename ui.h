#ifndef UI_H
#define UI_H

#include "game_logic.h"
#include "raylib.h"

typedef struct {
    char player_name[NAME_LEN];
    int show_menu;
    int show_leaderboard;
    char message[256];
    float message_timer;
} UIState;

void init_ui(UIState *ui);
void draw_grid(const Grid *g);
void draw_buttons(UIState *ui);
void draw_status(const Grid *g, UIState *ui);
void draw_leaderboard(UIState *ui);
void draw_message(UIState *ui);
void show_message(UIState *ui, const char *msg);
void draw_exit_button(void);
int is_exit_button_clicked(Vector2 mouse);

#endif
