#include "ui_display.h"
#include "leaderboard.h"
#include <ctype.h>
#include <termios.h>
#include <unistd.h>

/* ══════════════════════════════════════════════════════════════════
 *  GRID DISPLAY
 * ══════════════════════════════════════════════════════════════════ */

static void print_h_separator(int thick)
{
    const char *left  = thick ? "╠" : "├";
    const char *mid   = thick ? "╬" : "┼";
    const char *right = thick ? "╣" : "┤";
    const char *bar   = thick ? "═══════" : "───────";
    printf("%s", left);
    for (int b = 0; b < 3; b++) {
        printf("%s", bar);
        if (b < 2) printf("%s", mid);
    }
    printf("%s\n", right);
}

void display_grid(const Grid *g)
{
    CLEAR_SCREEN();

    /* ── Title banner ── */
    printf("\n");
    printf("  %s╔══════════════════════════════╗%s\n",
           COL_CYAN COL_BOLD, COL_RESET);
    printf("  %s║      S U D O K U  9×9        ║%s\n",
           COL_CYAN COL_BOLD, COL_RESET);
    printf("  %s╚══════════════════════════════╝%s\n\n",
           COL_CYAN COL_BOLD, COL_RESET);

    /* Column index guide */
    printf("     ");
    for (int c = 0; c < GRID_SIZE; c++) {
        if (c > 0 && c % BOX_SIZE == 0) printf("   ");
        printf(" %s%d%s  ", COL_MAGENTA, c + 1, COL_RESET);
    }
    printf("\n");

    /* Top border */
    printf("    ╔═════════════════╦═════════════════╦═════════════════╗\n");

    for (int r = 0; r < GRID_SIZE; r++) {
        /* Thick separator between 3-row blocks (except top) */
        if (r > 0) {
            if (r % BOX_SIZE == 0)
                printf("    ╠═════════════════╬═════════════════╬═════════════════╣\n");
            else
                printf("    ╟─────────────────╫─────────────────╫─────────────────╢\n");
        }

        /* Row index */
        printf(" %s%d%s  ║", COL_MAGENTA, r + 1, COL_RESET);

        for (int c = 0; c < GRID_SIZE; c++) {
            if (c > 0 && c % BOX_SIZE == 0) printf("║");
            else if (c > 0)                  printf("│");

            const Cell *cell = &g->cells[r][c];
            int selected = (r == g->selected_row && c == g->selected_col);

            if (selected) printf("%s", COL_HIGHLIGHT);

            if (cell->value == EMPTY) {
                printf("  %s %s  ", COL_BLUE, COL_RESET);
            } else if (cell->given) {
                printf("  %s%d%s  ", COL_GIVEN, cell->value, COL_RESET);
            } else if (cell->error) {
                printf("  %s%d%s  ", COL_ERROR, cell->value, COL_RESET);
            } else {
                printf("  %s%d%s  ", COL_USER, cell->value, COL_RESET);
            }

            if (selected) printf("%s", COL_RESET);
        }
        printf("║\n");
    }

    /* Bottom border */
    printf("    ╚═════════════════╩═════════════════╩═════════════════╝\n\n");
}

/* ══════════════════════════════════════════════════════════════════
 *  STATUS BAR
 * ══════════════════════════════════════════════════════════════════ */

void display_status_bar(const Grid *g)
{
    int elapsed = get_elapsed_seconds(g);
    int mins    = elapsed / 60;
    int secs    = elapsed % 60;

    const char *diff_str = (g->difficulty == EASY)   ? "Easy"   :
                           (g->difficulty == MEDIUM)  ? "Medium" : "Hard";
    const char *diff_col = (g->difficulty == EASY)   ? COL_GREEN   :
                           (g->difficulty == MEDIUM)  ? COL_YELLOW  : COL_RED;

    printf("  Difficulty: %s%s%s   ", diff_col, diff_str, COL_RESET);
    printf("Time: %s%02d:%02d%s   ", COL_CYAN, mins, secs, COL_RESET);
    printf("Mistakes: %s%d%s\n\n",
           g->mistakes > 0 ? COL_RED : COL_GREEN,
           g->mistakes, COL_RESET);

    /* Legend */
    printf("  %s●%s Given  %s●%s Yours  %s●%s Error  "
           "%s●%s Selected\n\n",
           COL_GIVEN, COL_RESET,
           COL_USER,  COL_RESET,
           COL_ERROR, COL_RESET,
           COL_HIGHLIGHT, COL_RESET);
}

/* ══════════════════════════════════════════════════════════════════
 *  HELP OVERLAY
 * ══════════════════════════════════════════════════════════════════ */

void display_help(void)
{
    printf("  %s┌─── Controls ───────────────────────────┐%s\n",
           COL_CYAN, COL_RESET);
    printf("  %s│%s  W A S D / Arrow keys  Move cursor     %s│%s\n",
           COL_CYAN, COL_RESET, COL_CYAN, COL_RESET);
    printf("  %s│%s  1-9                   Place digit      %s│%s\n",
           COL_CYAN, COL_RESET, COL_CYAN, COL_RESET);
    printf("  %s│%s  0 / Space / X         Erase cell       %s│%s\n",
           COL_CYAN, COL_RESET, COL_CYAN, COL_RESET);
    printf("  %s│%s  H                     Toggle help      %s│%s\n",
           COL_CYAN, COL_RESET, COL_CYAN, COL_RESET);
    printf("  %s│%s  Q                     Quit to menu     %s│%s\n",
           COL_CYAN, COL_RESET, COL_CYAN, COL_RESET);
    printf("  %s└────────────────────────────────────────┘%s\n\n",
           COL_CYAN, COL_RESET);
}

/* ══════════════════════════════════════════════════════════════════
 *  MAIN MENU
 * ══════════════════════════════════════════════════════════════════ */

void display_main_menu(void)
{
    CLEAR_SCREEN();
    printf("\n\n");
    printf("  %s╔════════════════════════════════════╗%s\n",
           COL_CYAN COL_BOLD, COL_RESET);
    printf("  %s║                                    ║%s\n",
           COL_CYAN COL_BOLD, COL_RESET);
    printf("  %s║   ███████╗██╗   ██╗██████╗         ║%s\n",
           COL_CYAN COL_BOLD, COL_RESET);
    printf("  %s║  ██╔════╝██║   ██║██╔══██╗        ║%s\n",
           COL_CYAN COL_BOLD, COL_RESET);
    printf("  %s║  ╚█████╗ ██║   ██║██║  ██║        ║%s\n",
           COL_CYAN COL_BOLD, COL_RESET);
    printf("  %s║   ╚═══██╗██║   ██║██║  ██║        ║%s\n",
           COL_CYAN COL_BOLD, COL_RESET);
    printf("  %s║  ██████╔╝╚██████╔╝██████╔╝        ║%s\n",
           COL_CYAN COL_BOLD, COL_RESET);
    printf("  %s║  ╚═════╝  ╚═════╝ ╚═════╝         ║%s\n",
           COL_CYAN COL_BOLD, COL_RESET);
    printf("  %s║       O K U                        ║%s\n",
           COL_YELLOW COL_BOLD, COL_RESET);
    printf("  %s║                                    ║%s\n",
           COL_CYAN COL_BOLD, COL_RESET);
    printf("  %s╚════════════════════════════════════╝%s\n\n",
           COL_CYAN COL_BOLD, COL_RESET);

    printf("  %s[1]%s  New Game\n",   COL_GREEN, COL_RESET);
    printf("  %s[2]%s  Leaderboard\n", COL_GREEN, COL_RESET);
    printf("  %s[3]%s  Help\n",        COL_GREEN, COL_RESET);
    printf("  %s[Q]%s  Quit\n\n",      COL_RED,   COL_RESET);

    printf("  Choice: ");
}

/* ══════════════════════════════════════════════════════════════════
 *  WIN / GAME OVER SCREENS
 * ══════════════════════════════════════════════════════════════════ */

void display_win_screen(const Grid *g, const char *player_name)
{
    int elapsed = get_elapsed_seconds(g);
    CLEAR_SCREEN();
    printf("\n\n");
    printf("  %s╔═══════════════════════════════╗%s\n",
           COL_GREEN COL_BOLD, COL_RESET);
    printf("  %s║   🎉  PUZZLE SOLVED!  🎉      ║%s\n",
           COL_GREEN COL_BOLD, COL_RESET);
    printf("  %s╚═══════════════════════════════╝%s\n\n",
           COL_GREEN COL_BOLD, COL_RESET);

    printf("  Player   : %s%s%s\n", COL_YELLOW, player_name, COL_RESET);
    printf("  Time     : %s%02d:%02d%s\n",
           COL_CYAN, elapsed/60, elapsed%60, COL_RESET);
    printf("  Mistakes : %s%d%s\n\n",
           g->mistakes > 0 ? COL_RED : COL_GREEN,
           g->mistakes, COL_RESET);

    printf("  %s[Enter]%s  Back to menu\n\n", COL_GREEN, COL_RESET);
}

void display_game_over(void)
{
    CLEAR_SCREEN();
    printf("\n\n");
    printf("  %s╔═══════════════════════════════╗%s\n",
           COL_RED COL_BOLD, COL_RESET);
    printf("  %s║        GAME OVER              ║%s\n",
           COL_RED COL_BOLD, COL_RESET);
    printf("  %s╚═══════════════════════════════╝%s\n\n",
           COL_RED COL_BOLD, COL_RESET);
    printf("  %s[Enter]%s  Back to menu\n\n", COL_GREEN, COL_RESET);
}

/* ══════════════════════════════════════════════════════════════════
 *  INPUT HELPERS
 * ══════════════════════════════════════════════════════════════════ */

/* Single-character read without requiring Enter */
char get_keypress(void)
{
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    read(STDIN_FILENO, &ch, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int prompt_difficulty(void)
{
    CLEAR_SCREEN();
    printf("\n  %sSelect difficulty:%s\n\n",
           COL_BOLD COL_CYAN, COL_RESET);
    printf("  %s[1]%s  Easy\n",   COL_GREEN,  COL_RESET);
    printf("  %s[2]%s  Medium\n", COL_YELLOW, COL_RESET);
    printf("  %s[3]%s  Hard\n\n", COL_RED,    COL_RESET);
    printf("  Choice: ");

    char c = get_keypress();
    printf("%c\n", c);
    if (c == '1') return EASY;
    if (c == '2') return MEDIUM;
    if (c == '3') return HARD;
    return EASY; /* default */
}

void prompt_player_name(char *buf, int max_len)
{
    printf("\n  Enter your name: ");
    /* Restore canonical mode temporarily */
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    if (fgets(buf, max_len, stdin)) {
        buf[strcspn(buf, "\n")] = '\0';
    }
    if (strlen(buf) == 0) strcpy(buf, "Player");

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}