#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "utils.h"

typedef struct {
    char name[NAME_LEN];
    int time_seconds;
    int mistakes;
    Difficulty difficulty;
} ScoreEntry;

void lb_add_score(const char *name, int time_sec, int mistakes, Difficulty diff);
int lb_load(ScoreEntry entries[], int *count);
void lb_save(ScoreEntry entries[], int count);

#endif
