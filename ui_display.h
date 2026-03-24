#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H

#include "game_logic.h"

/* ─── Grid rendering ────────────────────────────────────────────── */
void display_grid        (const Grid *g);
void display_status_bar  (const Grid *g);
void display_help        (void);

/* ─── Menus ─────────────────────────────────────────────────────── */
void display_main_menu   (void);
void display_win_screen  (const Grid *g, const char *player_name);
void display_game_over   (void);

/* ─── Input helpers ─────────────────────────────────────────────── */
char get_keypress        (void);
int  prompt_difficulty   (void);
void prompt_player_name  (char *buf, int max_len);

#endif /* UI_DISPLAY_H */