#include "raylib.h"
#include "game_logic.h"
#include "generator.h"
#include "ui.h"
#include "leaderboard.h"
#include "start_menu.h"
#include "save_game.h"

// Variabile globale
int GRID_SIZE;
int BOX_SIZE;
int MAX_DIGIT;
int EASY_REMOVE;
int MEDIUM_REMOVE;
int HARD_REMOVE;
int CELL_SIZE;
int GRID_OFFSET_X;
int GRID_OFFSET_Y;
int SCREEN_WIDTH;
int SCREEN_HEIGHT;

int scroll_x = 0;
int scroll_y = 0;
int max_scroll_x = 0;
int max_scroll_y = 0;
int time_limit = 0;
int show_time_warning = 0;
float warning_timer = 0;
int game_finished = 0;  // Flag pentru a opri timerul după câștig/pierdere
int game_start_time = 0; // Timpul de start înghețat

void set_game_mode(int mode) {
    if (mode == MODE_CLASSIC) {
        GRID_SIZE = CLASSIC_SIZE;
        BOX_SIZE = CLASSIC_BOX;
        MAX_DIGIT = CLASSIC_SIZE;
        EASY_REMOVE = CLASSIC_REMOVE_EASY;
        MEDIUM_REMOVE = CLASSIC_REMOVE_MEDIUM;
        HARD_REMOVE = CLASSIC_REMOVE_HARD;
        SCREEN_WIDTH = 1000;
        SCREEN_HEIGHT = 800;
        GRID_OFFSET_X = 200;
        GRID_OFFSET_Y = 80;
    } else {
        GRID_SIZE = SCALE_SIZE;
        BOX_SIZE = SCALE_BOX;
        MAX_DIGIT = SCALE_SIZE;
        EASY_REMOVE = SCALE_REMOVE_EASY;
        MEDIUM_REMOVE = SCALE_REMOVE_MEDIUM;
        HARD_REMOVE = SCALE_REMOVE_HARD;
        SCREEN_WIDTH = 1200;
        SCREEN_HEIGHT = 800;
        GRID_OFFSET_X = 220;
        GRID_OFFSET_Y = 80;
    }
    CELL_SIZE = 50;
    max_scroll_x = (GRID_SIZE * CELL_SIZE + GRID_OFFSET_X * 2) - SCREEN_WIDTH;
    if (max_scroll_x < 0) max_scroll_x = 0;
    max_scroll_y = (GRID_SIZE * CELL_SIZE + GRID_OFFSET_Y * 2) - SCREEN_HEIGHT;
    if (max_scroll_y < 0) max_scroll_y = 0;
}

int get_time_limit(Difficulty diff) {
    switch(diff) {
        case EASY: return TIME_LIMIT_EASY;
        case MEDIUM: return TIME_LIMIT_MEDIUM;
        case HARD: return TIME_LIMIT_HARD;
        default: return TIME_LIMIT_EASY;
    }
}

int main(void) {
    srand(time(NULL));
    
    set_game_mode(MODE_CLASSIC);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sudoku Advanced");
    SetTargetFPS(60);
    
    StartMenu start_menu;
    init_start_menu(&start_menu);
    
    Grid game;
    UIState ui;
    init_ui(&ui);
    
    int game_active = 0;
    int confirm_exit = 0;
    
    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        int key = GetKeyPressed();
        
        float wheel = GetMouseWheelMove();
        if (wheel != 0 && game_active && GRID_SIZE == 16 && !game_finished) {
            scroll_y -= wheel * 30;
            if (scroll_y < 0) scroll_y = 0;
            if (scroll_y > max_scroll_y) scroll_y = max_scroll_y;
        }
        
        if (!game_active) {
            handle_start_menu_input(&start_menu, mouse, key);
            
            if (start_menu.start_game == 1) {
                // Joc nou
                set_game_mode(start_menu.selected_mode);
                SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
                SetWindowPosition(GetMonitorWidth(GetCurrentMonitor())/2 - SCREEN_WIDTH/2,
                                  GetMonitorHeight(GetCurrentMonitor())/2 - SCREEN_HEIGHT/2);
                scroll_x = 0;
                scroll_y = 0;
                game_finished = 0;
                game_start_time = 0;
                strcpy(ui.player_name, start_menu.player_name);
                grid_init(&game, start_menu.selected_mode);
                reset_game(&game, start_menu.selected_diff, start_menu.selected_mode);
                time_limit = get_time_limit(start_menu.selected_diff);
                show_time_warning = 0;
                game_active = 1;
                start_menu.start_game = 0;
            } 
            else if (start_menu.start_game == 2) {
                // Load game
                if (load_game(&game, ui.player_name)) {
                    set_game_mode(game.game_mode);
                    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
                    SetWindowPosition(GetMonitorWidth(GetCurrentMonitor())/2 - SCREEN_WIDTH/2,
                                      GetMonitorHeight(GetCurrentMonitor())/2 - SCREEN_HEIGHT/2);
                    scroll_x = 0;
                    scroll_y = 0;
                    game_finished = 0;
                    game_start_time = 0;
                    time_limit = get_time_limit(game.difficulty);
                    show_time_warning = 0;
                    game_active = 1;
                    show_message(&ui, "Game loaded successfully!");
                } else {
                    show_message(&ui, "Failed to load game!");
                }
                start_menu.start_game = 0;
            }
            
            BeginDrawing();
            draw_start_menu(&start_menu);
            EndDrawing();
        }
        else if (game_active && confirm_exit) {
            // Meniu confirmare exit
            BeginDrawing();
            ClearBackground((Color){20,20,40,255});
            DrawRectangle(SCREEN_WIDTH/2 - 250, SCREEN_HEIGHT/2 - 100, 500, 200, (Color){0,0,0,220});
            DrawRectangleLines(SCREEN_WIDTH/2 - 250, SCREEN_HEIGHT/2 - 100, 500, 200, YELLOW);
            DrawText("SAVE BEFORE EXITING?", SCREEN_WIDTH/2 - 180, SCREEN_HEIGHT/2 - 60, 30, YELLOW);
            
            DrawRectangle(SCREEN_WIDTH/2 - 180, SCREEN_HEIGHT/2 + 20, 120, 40, GREEN);
            DrawText("SAVE & EXIT", SCREEN_WIDTH/2 - 160, SCREEN_HEIGHT/2 + 30, 18, BLACK);
            
            DrawRectangle(SCREEN_WIDTH/2 - 20, SCREEN_HEIGHT/2 + 20, 120, 40, ORANGE);
            DrawText("EXIT", SCREEN_WIDTH/2 + 15, SCREEN_HEIGHT/2 + 30, 18, BLACK);
            
            DrawRectangle(SCREEN_WIDTH/2 + 140, SCREEN_HEIGHT/2 + 20, 100, 40, RED);
            DrawText("CANCEL", SCREEN_WIDTH/2 + 160, SCREEN_HEIGHT/2 + 30, 18, BLACK);
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (mouse.x >= SCREEN_WIDTH/2 - 180 && mouse.x <= SCREEN_WIDTH/2 - 60 &&
                    mouse.y >= SCREEN_HEIGHT/2 + 20 && mouse.y <= SCREEN_HEIGHT/2 + 60) {
                    save_game(&game, ui.player_name);
                    confirm_exit = 0;
                    game_active = 0;
                    game_finished = 0;
                    grid_free(&game);
                    init_start_menu(&start_menu);
                    set_game_mode(MODE_CLASSIC);
                    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
                }
                else if (mouse.x >= SCREEN_WIDTH/2 - 20 && mouse.x <= SCREEN_WIDTH/2 + 100 &&
                         mouse.y >= SCREEN_HEIGHT/2 + 20 && mouse.y <= SCREEN_HEIGHT/2 + 60) {
                    delete_save();
                    confirm_exit = 0;
                    game_active = 0;
                    game_finished = 0;
                    grid_free(&game);
                    init_start_menu(&start_menu);
                    set_game_mode(MODE_CLASSIC);
                    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
                }
                else if (mouse.x >= SCREEN_WIDTH/2 + 140 && mouse.x <= SCREEN_WIDTH/2 + 240 &&
                         mouse.y >= SCREEN_HEIGHT/2 + 20 && mouse.y <= SCREEN_HEIGHT/2 + 60) {
                    confirm_exit = 0;
                }
            }
            EndDrawing();
        }
        else {
            // Joc activ - calculează timpul doar dacă nu e terminat
            int elapsed, time_left;
            
            if (!game_finished) {
                elapsed = get_elapsed_seconds(&game);
                time_left = time_limit - elapsed;
                
                // Verifică timpul
                if (time_left <= 0 && !game.win && !game.game_over) {
                    game.game_over = 1;
                    game_finished = 1;
                    game_start_time = elapsed;
                    show_message(&ui, "TIME'S UP! GAME OVER!");
                    delete_save();
                }
                
                // Avertizare cu 60 de secunde înainte
                if (time_left <= 60 && time_left > 0 && !show_time_warning && !game.win) {
                    show_time_warning = 1;
                    warning_timer = 3.0f;
                    show_message(&ui, "WARNING: Only 1 minute left!");
                }
            } else {
                // Joc terminat - folosește timpul înghețat
                elapsed = game_start_time;
                time_left = time_limit - elapsed;
                if (time_left < 0) time_left = 0;
            }
            
            if (warning_timer > 0) {
                warning_timer -= GetFrameTime();
                if (warning_timer <= 0) {
                    warning_timer = 0;
                    show_time_warning = 0;
                }
            }
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                int grid_x = mouse.x - GRID_OFFSET_X + scroll_x;
                int grid_y = mouse.y - GRID_OFFSET_Y + scroll_y;
                if (grid_x >= 0 && grid_x < GRID_SIZE * CELL_SIZE &&
                    grid_y >= 0 && grid_y < GRID_SIZE * CELL_SIZE && !game_finished) {
                    game.selected_row = grid_y / CELL_SIZE;
                    game.selected_col = grid_x / CELL_SIZE;
                }
                
                if (is_exit_button_clicked(mouse) && !game_finished) {
                    confirm_exit = 1;
                }
                
                // Butonul BACK TO MENU după terminarea jocului
                if (game_finished) {
                    if (mouse.x >= SCREEN_WIDTH/2 - 100 && mouse.x <= SCREEN_WIDTH/2 + 100 &&
                        mouse.y >= SCREEN_HEIGHT - 80 && mouse.y <= SCREEN_HEIGHT - 40) {
                        game_active = 0;
                        game_finished = 0;
                        grid_free(&game);
                        init_start_menu(&start_menu);
                        set_game_mode(MODE_CLASSIC);
                        SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
                    }
                }
            }
            
            // Navigare cu săgeți (doar dacă jocul nu e terminat)
            if (!game_finished) {
                if (key == KEY_UP && game.selected_row > 0) game.selected_row--;
                if (key == KEY_DOWN && game.selected_row < GRID_SIZE - 1) game.selected_row++;
                if (key == KEY_LEFT && game.selected_col > 0) game.selected_col--;
                if (key == KEY_RIGHT && game.selected_col < GRID_SIZE - 1) game.selected_col++;
                
                int target_scroll_y = game.selected_row * CELL_SIZE + GRID_OFFSET_Y - SCREEN_HEIGHT/2;
                if (target_scroll_y < 0) target_scroll_y = 0;
                if (target_scroll_y > max_scroll_y) target_scroll_y = max_scroll_y;
                scroll_y = scroll_y * 0.9 + target_scroll_y * 0.1;
            }
            
            // Input-uri (doar dacă jocul nu e terminat)
            if (!game_finished && !game.win && !game.game_over && time_left > 0) {
                if (key >= KEY_ONE && key <= KEY_NINE && key - KEY_ONE + 1 <= MAX_DIGIT) {
                    place_number(&game, game.selected_row, game.selected_col, key - KEY_ONE + 1);
                } else if (MAX_DIGIT == 16 && key >= KEY_A && key <= KEY_G) {
                    place_number(&game, game.selected_row, game.selected_col, key - KEY_A + 10);
                } else if (key == KEY_DELETE || key == KEY_BACKSPACE) {
                    erase_cell(&game, game.selected_row, game.selected_col);
                } else if (key == KEY_H) {
                    if (!game.cells[game.selected_row][game.selected_col].given) {
                        place_number(&game, game.selected_row, game.selected_col, game.solution[game.selected_row][game.selected_col]);
                        show_message(&ui, "Hint placed!");
                    }
                } else if (key == KEY_S) {
                    save_game(&game, ui.player_name);
                    show_message(&ui, "Game saved!");
                }
            }
            
            if (key == KEY_ESCAPE && !game_finished) {
                ui.show_leaderboard = !ui.show_leaderboard;
            }
            
            // Verifică pierdere
            if (!game_finished && !game.win && game.mistakes >= 5 && !game.game_over) {
                game.game_over = 1;
                game_finished = 1;
                game_start_time = get_elapsed_seconds(&game);
                show_message(&ui, "GAME OVER! Too many mistakes!");
                delete_save();
            }
            
            // Verifică câștig
            if (!game_finished && !game.game_over && is_solved(&game) && !game.win) {
                game.win = 1;
                game_finished = 1;
                game_start_time = get_elapsed_seconds(&game);
                int final_time = game_start_time;
                if (final_time <= time_limit) {
                    lb_add_score(ui.player_name, final_time, game.mistakes, game.difficulty);
                    show_message(&ui, "CONGRATULATIONS! You won!");
                } else {
                    show_message(&ui, "CONGRATULATIONS! You solved it!");
                }
                delete_save();
            }
            
            BeginDrawing();
            ClearBackground((Color){20,20,30,255});
            
            int start_x = GRID_OFFSET_X;
            int start_y = GRID_OFFSET_Y;
            
            for (int r = 0; r < GRID_SIZE; r++) {
                for (int c = 0; c < GRID_SIZE; c++) {
                    int screen_x = start_x + c * CELL_SIZE - scroll_x;
                    int screen_y = start_y + r * CELL_SIZE - scroll_y;
                    
                    if (screen_x + CELL_SIZE > 0 && screen_x < SCREEN_WIDTH &&
                        screen_y + CELL_SIZE > 0 && screen_y < SCREEN_HEIGHT) {
                        
                        Color bg = (r == game.selected_row && c == game.selected_col && !game_finished) ? (Color){50,100,200,255} : 
                                   ((r+c)%2==0) ? (Color){30,30,40,255} : (Color){40,40,50,255};
                        DrawRectangle(screen_x, screen_y, CELL_SIZE, CELL_SIZE, bg);
                        DrawRectangleLines(screen_x, screen_y, CELL_SIZE, CELL_SIZE, DARKGRAY);
                        
                        if (game.cells[r][c].value) {
                            char text[3] = { digit_to_char(game.cells[r][c].value), 0 };
                            Color tc = game.cells[r][c].given ? (Color){255,255,100,255} : (game.cells[r][c].error ? RED : GREEN);
                            int font_size = (GRID_SIZE == 9) ? 28 : 20;
                            int tw = MeasureText(text, font_size);
                            DrawText(text, screen_x + CELL_SIZE/2 - tw/2, screen_y + CELL_SIZE/2 - font_size/2, font_size, tc);
                        }
                    }
                }
            }
            
            for (int i = 1; i < BOX_SIZE; i++) {
                int y = start_y + i * BOX_SIZE * CELL_SIZE - scroll_y;
                if (y > 0 && y < SCREEN_HEIGHT) {
                    DrawLine(start_x - scroll_x, y, start_x + GRID_SIZE * CELL_SIZE - scroll_x, y, YELLOW);
                }
                int x = start_x + i * BOX_SIZE * CELL_SIZE - scroll_x;
                if (x > 0 && x < SCREEN_WIDTH) {
                    DrawLine(x, start_y - scroll_y, x, start_y + GRID_SIZE * CELL_SIZE - scroll_y, YELLOW);
                }
            }
            
            draw_status(&game, &ui);
            draw_message(&ui);
            
            // Butonul EXIT (doar dacă jocul nu e terminat)
            if (!game_finished) {
                draw_exit_button();
            }
            
            // Afișează timpul rămas sau mesaj de final
            if (!game_finished && time_left > 0) {
                char time_left_str[50];
                int minutes = time_left / 60;
                int seconds = time_left % 60;
                Color time_color = (time_left <= 60) ? RED : (time_left <= 120) ? ORANGE : WHITE;
                sprintf(time_left_str, "Time left: %02d:%02d", minutes, seconds);
                DrawText(time_left_str, SCREEN_WIDTH - 180, 60, 20, time_color);
            } else if (game_finished) {
                if (game.win) {
                    DrawText("YOU WIN!", SCREEN_WIDTH - 140, 60, 25, GREEN);
                } else if (game.game_over) {
                    DrawText("GAME OVER!", SCREEN_WIDTH - 150, 60, 20, RED);
                }
            }
            
            DrawText("S=Save H=Hint ESC=Leaderboard", 20, SCREEN_HEIGHT - 45, 16, GRAY);
            
            if (ui.show_leaderboard && !game_finished) {
                draw_leaderboard(&ui);
            }
            
            if (GRID_SIZE == 16 && !game_finished) {
                DrawText("Use MOUSE WHEEL to scroll | Arrows to move", 20, SCREEN_HEIGHT - 25, 16, GRAY);
            }
            
            // Buton BACK TO MENU după terminarea jocului
            if (game_finished) {
                DrawRectangle(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT - 80, 200, 45, (Color){100,50,150,255});
                DrawRectangleLines(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT - 80, 200, 45, WHITE);
                DrawText("BACK TO MENU", SCREEN_WIDTH/2 - 60, SCREEN_HEIGHT - 68, 18, WHITE);
            }
            
            EndDrawing();
        }
    }
    
    CloseWindow();
    return 0;
}
