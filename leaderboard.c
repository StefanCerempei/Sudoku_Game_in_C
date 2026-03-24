#include "leaderboard.h"
#include "ui_display.h"

/* ─── Score calculation (lower is better) ──────────────────────── */
static int score_value(const ScoreEntry *e)
{
    /* Penalty: 30 s per mistake */
    return e->time_seconds + e->mistakes * 30;
}

static int cmp_entries(const void *a, const void *b)
{
    return score_value((const ScoreEntry *)a)
         - score_value((const ScoreEntry *)b);
}

/* ─── Load from CSV ─────────────────────────────────────────────── */
int lb_load(ScoreEntry entries[], int *count)
{
    *count = 0;
    FILE *f = fopen(LB_FILE, "r");
    if (!f) return 0;

    char line[256];
    while (*count < LB_MAX_ENTRIES && fgets(line, sizeof(line), f)) {
        ScoreEntry *e = &entries[*count];
        int diff;
        if (sscanf(line, "%31[^,],%d,%d,%d",
                   e->name, &e->time_seconds, &e->mistakes, &diff) == 4) {
            e->difficulty = (Difficulty)diff;
            (*count)++;
        }
    }
    fclose(f);
    return 1;
}

/* ─── Save to CSV ───────────────────────────────────────────────── */
void lb_save(ScoreEntry entries[], int count)
{
    FILE *f = fopen(LB_FILE, "w");
    if (!f) return;

    for (int i = 0; i < count; i++)
        fprintf(f, "%s,%d,%d,%d\n",
                entries[i].name,
                entries[i].time_seconds,
                entries[i].mistakes,
                (int)entries[i].difficulty);
    fclose(f);
}

/* ─── Add a new score (keeps top LB_MAX_ENTRIES) ───────────────── */
void lb_add_score(const char *name, int time_sec, int mistakes, Difficulty diff)
{
    ScoreEntry entries[LB_MAX_ENTRIES + 1];
    int count = 0;
    lb_load(entries, &count);

    /* Append new entry */
    if (count < LB_MAX_ENTRIES + 1) {
        strncpy(entries[count].name, name, NAME_LEN - 1);
        entries[count].name[NAME_LEN - 1] = '\0';
        entries[count].time_seconds = time_sec;
        entries[count].mistakes     = mistakes;
        entries[count].difficulty   = diff;
        count++;
    }

    /* Sort and trim */
    qsort(entries, count, sizeof(ScoreEntry), cmp_entries);
    if (count > LB_MAX_ENTRIES) count = LB_MAX_ENTRIES;

    lb_save(entries, count);
}

/* ─── Display leaderboard ───────────────────────────────────────── */
void lb_display(void)
{
    ScoreEntry entries[LB_MAX_ENTRIES];
    int count = 0;
    lb_load(entries, &count);

    CLEAR_SCREEN();
    printf("\n  %s╔══════════════════════════════════════════════════╗%s\n",
           COL_YELLOW COL_BOLD, COL_RESET);
    printf("  %s║              🏆  LEADERBOARD  🏆                ║%s\n",
           COL_YELLOW COL_BOLD, COL_RESET);
    printf("  %s╚══════════════════════════════════════════════════╝%s\n\n",
           COL_YELLOW COL_BOLD, COL_RESET);

    if (count == 0) {
        printf("  %sNo scores yet. Be the first!%s\n\n",
               COL_CYAN, COL_RESET);
    } else {
        printf("  %-4s %-16s %-8s %-8s %-8s\n",
               "Rank", "Name", "Time", "Mistakes", "Difficulty");
        printf("  %-4s %-16s %-8s %-8s %-8s\n",
               "────", "────────────────", "────────", "────────", "──────────");

        const char *medals[] = {"🥇", "🥈", "🥉"};
        const char *diff_names[] = {"Easy", "Medium", "Hard"};
        const char *diff_cols[]  = {COL_GREEN, COL_YELLOW, COL_RED};

        for (int i = 0; i < count; i++) {
            const ScoreEntry *e = &entries[i];
            int mins = e->time_seconds / 60;
            int secs = e->time_seconds % 60;

            const char *rank_str;
            char rank_buf[8];
            if (i < 3) {
                rank_str = medals[i];
            } else {
                snprintf(rank_buf, sizeof(rank_buf), " %d.", i + 1);
                rank_str = rank_buf;
            }

            printf("  %-4s %s%-16s%s %s%02d:%02d%s    %s%-8d%s %s%s%s\n",
                   rank_str,
                   COL_WHITE, e->name, COL_RESET,
                   COL_CYAN, mins, secs, COL_RESET,
                   e->mistakes > 0 ? COL_RED : COL_GREEN,
                   e->mistakes, COL_RESET,
                   diff_cols[e->difficulty],
                   diff_names[e->difficulty], COL_RESET);
        }
    }

    printf("\n  %s[Enter]%s  Back to menu\n\n", COL_GREEN, COL_RESET);
    getchar();
}