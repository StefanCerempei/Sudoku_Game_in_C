#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "utils.h"

/* ─── Score entry ────────────────────────────────────────────────── */
typedef struct {
    char name[NAME_LEN];
    int  time_seconds;   /* lower = better   */
    int  mistakes;
    Difficulty difficulty;
} ScoreEntry;

/* ─── Prototypes ────────────────────────────────────────────────── */
void lb_add_score    (const char *name, int time_sec,
                      int mistakes, Difficulty diff);
void lb_display      (void);
int  lb_load         (ScoreEntry entries[], int *count);
void lb_save         (ScoreEntry entries[], int count);

#endif /* LEADERBOARD_H */