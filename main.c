#include <unistd.h>
#include "game_logic.h"
#include "generator.h"
#include "ui_display.h"
#include "leaderboard.h"

/* ══════════════════════════════════════════════════════════════════
 *  GAME LOOP
 * ══════════════════════════════════════════════════════════════════ */

static void move_cursor(Grid *g, int dr, int dc)
{
    g->selected_row = (g->selected_row + dr + GRID_SIZE) % GRID_SIZE;
    g->selected_col = (g->selected_col + dc + GRID_SIZE) % GRID_SIZE;
}

static void run_game(Difficulty diff, const char *player_name)
{
    Grid g;
    generate_puzzle(&g, diff);

    int show_help = 0;
    int max_mistakes = 5;

    while (1) {
        display_grid(&g);
        display_status_bar(&g);
        if (show_help) display_help();

        /* Check win condition */
        if (is_solved(&g)) {
            lb_add_score(player_name,
                         get_elapsed_seconds(&g),
                         g.mistakes,
                         diff);
            display_win_screen(&g, player_name);
            getchar();
            return;
        }

        /* Check too many mistakes */
        if (g.mistakes >= max_mistakes) {
            display_game_over();
            getchar();
            return;
        }

        /* ── Input ── */
        char key = get_keypress();

        switch (key) {
            /* Movement: WASD */
            case 'w': case 'W': move_cursor(&g, -1,  0); break;
            case 's': case 'S': move_cursor(&g, +1,  0); break;
            case 'a': case 'A': move_cursor(&g,  0, -1); break;
            case 'd': case 'D': move_cursor(&g,  0, +1); break;

            /* Arrow keys send 3-byte escape sequences */
            case '\033': {
                char seq[3];
                read(STDIN_FILENO, &seq[0], 1);
                if (seq[0] == '[') {
                    read(STDIN_FILENO, &seq[1], 1);
                    switch (seq[1]) {
                        case 'A': move_cursor(&g, -1,  0); break; /* Up    */
                        case 'B': move_cursor(&g, +1,  0); break; /* Down  */
                        case 'C': move_cursor(&g,  0, +1); break; /* Right */
                        case 'D': move_cursor(&g,  0, -1); break; /* Left  */
                    }
                }
                break;
            }

            /* Digit input */
            case '1': case '2': case '3':
            case '4': case '5': case '6':
            case '7': case '8': case '9':
                place_number(&g, g.selected_row, g.selected_col,
                             key - '0');
                break;

            /* Erase */
            case '0': case ' ': case 'x': case 'X':
                erase_cell(&g, g.selected_row, g.selected_col);
                break;

            /* Help toggle */
            case 'h': case 'H':
                show_help = !show_help;
                break;

            /* Quit to menu */
            case 'q': case 'Q':
                return;

            default: break;
        }
    }
}

/* ══════════════════════════════════════════════════════════════════
 *  MAIN
 * ══════════════════════════════════════════════════════════════════ */

int main(void)
{
    srand((unsigned int)time(NULL));

    char player_name[NAME_LEN] = "Player";
    printf("\n  ");
    prompt_player_name(player_name, NAME_LEN);

    while (1) {
        display_main_menu();
        char choice = get_keypress();
        printf("%c\n", choice);

        switch (choice) {
            case '1': {
                Difficulty diff = (Difficulty)prompt_difficulty();
                run_game(diff, player_name);
                break;
            }
            case '2':
                lb_display();
                break;
            case '3':
                CLEAR_SCREEN();
                printf("\n");
                display_help();
                printf("  Press any key to return...\n");
                get_keypress();
                break;
            case 'q': case 'Q':
                CLEAR_SCREEN();
                printf("\n  %sThanks for playing Sudoku! Goodbye.%s\n\n",
                       COL_CYAN, COL_RESET);
                return 0;
            default:
                break;
        }
    }
}