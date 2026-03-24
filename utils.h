#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ─── Grid dimensions ───────────────────────────────────────────── */
#define GRID_SIZE       9
#define BOX_SIZE        3
#define EMPTY           0

/* ─── Difficulty cell-removal counts ───────────────────────────── */
#define EASY_REMOVE     36
#define MEDIUM_REMOVE   46
#define HARD_REMOVE     54

/* ─── Leaderboard ───────────────────────────────────────────────── */
#define LB_FILE         "leaderboard.csv"
#define LB_MAX_ENTRIES  10
#define NAME_LEN        32

/* ─── ANSI color codes ──────────────────────────────────────────── */
#define COL_RESET       "\033[0m"
#define COL_BOLD        "\033[1m"
#define COL_RED         "\033[31m"
#define COL_GREEN       "\033[32m"
#define COL_YELLOW      "\033[33m"
#define COL_BLUE        "\033[34m"
#define COL_MAGENTA     "\033[35m"
#define COL_CYAN        "\033[36m"
#define COL_WHITE       "\033[37m"
#define COL_BG_DARK     "\033[48;5;235m"
#define COL_GIVEN       "\033[1;37m"   /* given cells  – bold white  */
#define COL_USER        "\033[1;33m"   /* user input   – bold yellow */
#define COL_ERROR       "\033[1;31m"   /* errors       – bold red    */
#define COL_HIGHLIGHT   "\033[48;5;24m"/* selected cell highlight    */

/* ─── Misc macros ───────────────────────────────────────────────── */
#define CLEAR_SCREEN()  printf("\033[2J\033[H")
#define MIN(a,b)        ((a)<(b)?(a):(b))
#define MAX(a,b)        ((a)>(b)?(a):(b))
#define UNUSED(x)       (void)(x)

typedef enum { EASY = 0, MEDIUM, HARD } Difficulty;

#endif /* UTILS_H */