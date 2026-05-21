#include "leaderboard.h"
#include <string.h>

static int score_value(const ScoreEntry *e) {
    return e->time_seconds + e->mistakes * 30;
}

static int cmp_entries(const void *a, const void *b) {
    return score_value((const ScoreEntry *)a) - score_value((const ScoreEntry *)b);
}

int lb_load(ScoreEntry entries[], int *count) {
    *count = 0;
    FILE *f = fopen(LB_FILE, "r");
    if (!f) {
        // Crează fișierul dacă nu există
        f = fopen(LB_FILE, "w");
        if (f) {
            fprintf(f, "# Sudoku Leaderboard\n");
            fclose(f);
        }
        return 0;
    }
    
    char line[256];
    while (*count < LB_MAX_ENTRIES && fgets(line, sizeof(line), f)) {
        // Ignoră liniile goale și comentariile
        if (line[0] == '#' || line[0] == '\n') continue;
        
        ScoreEntry *e = &entries[*count];
        int diff;
        if (sscanf(line, "%31[^,],%d,%d,%d", e->name, &e->time_seconds, &e->mistakes, &diff) == 4) {
            e->difficulty = (Difficulty)diff;
            (*count)++;
        }
    }
    fclose(f);
    
    // Sortează după scor
    if (*count > 0) {
        qsort(entries, *count, sizeof(ScoreEntry), cmp_entries);
    }
    
    return 1;
}

void lb_save(ScoreEntry entries[], int count) {
    FILE *f = fopen(LB_FILE, "w");
    if (!f) return;
    
    fprintf(f, "# Sudoku Leaderboard\n");
    fprintf(f, "# Format: name,time_seconds,mistakes,difficulty(0=Easy,1=Medium,2=Hard)\n");
    for (int i = 0; i < count; i++) {
        fprintf(f, "%s,%d,%d,%d\n", entries[i].name, entries[i].time_seconds, 
                entries[i].mistakes, (int)entries[i].difficulty);
    }
    fclose(f);
}

void lb_add_score(const char *name, int time_sec, int mistakes, Difficulty diff) {
    ScoreEntry entries[LB_MAX_ENTRIES + 1];
    int count = 0;
    lb_load(entries, &count);
    
    // Adaugă scorul nou
    if (count < LB_MAX_ENTRIES + 1) {
        strncpy(entries[count].name, name, NAME_LEN - 1);
        entries[count].name[NAME_LEN - 1] = '\0';
        entries[count].time_seconds = time_sec;
        entries[count].mistakes = mistakes;
        entries[count].difficulty = diff;
        count++;
    }
    
    // Sortează
    qsort(entries, count, sizeof(ScoreEntry), cmp_entries);
    
    // Păstrează doar top 10
    if (count > LB_MAX_ENTRIES) count = LB_MAX_ENTRIES;
    
    // Salvează
    lb_save(entries, count);
}

void lb_get_top_scores(ScoreEntry entries[], int *count, Difficulty diff) {
    ScoreEntry all[LB_MAX_ENTRIES];
    int all_count = 0;
    lb_load(all, &all_count);
    
    *count = 0;
    for (int i = 0; i < all_count && *count < 5; i++) {
        if (all[i].difficulty == diff) {
            entries[*count] = all[i];
            (*count)++;
        }
    }
}
