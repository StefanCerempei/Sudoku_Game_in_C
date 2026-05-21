#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Moduri de joc
#define MODE_CLASSIC    0
#define MODE_SCALE_UP   1

// Dimensiuni pentru modul clasic (9x9)
#define CLASSIC_SIZE    9
#define CLASSIC_BOX     3
#define CLASSIC_REMOVE_EASY   36
#define CLASSIC_REMOVE_MEDIUM 46
#define CLASSIC_REMOVE_HARD   54

// Dimensiuni pentru modul scale-up (16x16)
#define SCALE_SIZE      16
#define SCALE_BOX       4
#define SCALE_REMOVE_EASY     96
#define SCALE_REMOVE_MEDIUM   128
#define SCALE_REMOVE_HARD     160

// Variabile globale
extern int GRID_SIZE;
extern int BOX_SIZE;
extern int MAX_DIGIT;
extern int EASY_REMOVE;
extern int MEDIUM_REMOVE;
extern int HARD_REMOVE;
extern int CELL_SIZE;
extern int GRID_OFFSET_X;
extern int GRID_OFFSET_Y;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

#define EMPTY           0
#define LB_FILE         "leaderboard.csv"
#define LB_MAX_ENTRIES  10
#define NAME_LEN        32

typedef enum { EASY = 0, MEDIUM, HARD } Difficulty;

static inline char digit_to_char(int d) {
    if (d == 0) return ' ';
    if (d <= 9) return '0' + d;
    return 'A' + (d - 10);
}

static inline int char_to_digit(char c) {
    if (c >= '1' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'G') return c - 'A' + 10;
    if (c >= 'a' && c <= 'g') return c - 'a' + 10;
    return 0;
}

#endif

// Limite de timp pentru fiecare dificultate (în secunde)
#define TIME_LIMIT_EASY     600    // 10 minute
#define TIME_LIMIT_MEDIUM   900    // 15 minute
#define TIME_LIMIT_HARD     1200   // 20 minute

