#include "start_menu.h"
#include "leaderboard.h"
#include "save_game.h"
#include "raylib.h"
#include <ctype.h>

void draw_start_menu(StartMenu *menu) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    
    if (menu->show_leaderboard) {
        // Afișează leaderboard-ul
        DrawRectangle(0, 0, sw, sh, (Color){0,0,0,220});
        DrawRectangle(sw/2 - 350, 80, 700, 600, (Color){0,0,0,240});
        DrawRectangleLines(sw/2 - 350, 80, 700, 600, YELLOW);
        DrawText("LEADERBOARD", sw/2 - 120, 110, 40, YELLOW);
        
        ScoreEntry scores[10];
        int count = 0;
        lb_load(scores, &count);
        
        DrawText("Rank", sw/2 - 300, 170, 20, WHITE);
        DrawText("Name", sw/2 - 220, 170, 20, WHITE);
        DrawText("Time", sw/2 - 20, 170, 20, WHITE);
        DrawText("Mistakes", sw/2 + 100, 170, 20, WHITE);
        DrawText("Mode", sw/2 + 200, 170, 20, WHITE);
        
        for (int i = 0; i < count && i < 15; i++) {
            char line[128];
            char mode_text[20];
            sprintf(mode_text, "%dx%d", scores[i].difficulty == EASY ? 9 : 16, scores[i].difficulty == EASY ? 9 : 16);
            sprintf(line, "%d", i+1);
            DrawText(line, sw/2 - 300, 200 + i*35, 20, LIGHTGRAY);
            DrawText(scores[i].name, sw/2 - 220, 200 + i*35, 20, LIGHTGRAY);
            sprintf(line, "%02d:%02d", scores[i].time_seconds/60, scores[i].time_seconds%60);
            DrawText(line, sw/2 - 30, 200 + i*35, 20, LIGHTGRAY);
            sprintf(line, "%d", scores[i].mistakes);
            DrawText(line, sw/2 + 100, 200 + i*35, 20, LIGHTGRAY);
            DrawText(mode_text, sw/2 + 200, 200 + i*35, 20, LIGHTGRAY);
        }
        
        DrawRectangle(sw/2 - 60, 650, 120, 40, DARKGRAY);
        DrawRectangleLines(sw/2 - 60, 650, 120, 40, WHITE);
        DrawText("BACK", sw/2 - 20, 662, 20, WHITE);
        return;
    }
    
    // Meniul principal
    DrawRectangle(0, 0, sw, sh, (Color){20, 20, 40, 255});
    
    DrawText("SUDOKU", sw/2 - 150, 60, 80, (Color){255, 255, 100, 255});
    DrawText("Advanced", sw/2 - 80, 130, 40, (Color){100, 255, 100, 255});
    
    // Caseta nume
    Color name_bg = (menu->name_active) ? (Color){70, 70, 100, 255} : (Color){50, 50, 70, 255};
    DrawRectangle(sw/2 - 250, 200, 500, 50, name_bg);
    DrawRectangleLines(sw/2 - 250, 200, 500, 50, (Color){255, 255, 255, 255});
    DrawText("Name:", sw/2 - 230, 215, 25, WHITE);
    DrawText(menu->player_name, sw/2 - 150, 215, 25, YELLOW);
    
    if (menu->name_active && (GetTime() * 2 - (int)(GetTime() * 2) < 0.5)) {
        int cursor_x = sw/2 - 150 + MeasureText(menu->player_name, 25);
        DrawLine(cursor_x, 215, cursor_x, 240, YELLOW);
    }
    
    DrawText("Select Mode:", sw/2 - 80, 280, 25, WHITE);
    
    // Butoane mod
    Color classic_color = (menu->selected_mode == MODE_CLASSIC) ? (Color){0,150,0,255} : (Color){80,80,100,255};
    DrawRectangle(sw/2 - 150, 310, 130, 45, classic_color);
    DrawRectangleLines(sw/2 - 150, 310, 130, 45, WHITE);
    DrawText("Classic 9x9", sw/2 - 130, 323, 18, WHITE);
    
    Color scale_color = (menu->selected_mode == MODE_SCALE_UP) ? (Color){0,150,0,255} : (Color){80,80,100,255};
    DrawRectangle(sw/2 + 20, 310, 130, 45, scale_color);
    DrawRectangleLines(sw/2 + 20, 310, 130, 45, WHITE);
    DrawText("Scale 16x16", sw/2 + 35, 323, 18, WHITE);
    
    DrawText("Difficulty:", sw/2 - 80, 390, 25, WHITE);
    
    // Butoane dificultate
    Color easy_color = (menu->selected_diff == EASY) ? (Color){0,150,0,255} : (Color){80,80,100,255};
    DrawRectangle(sw/2 - 220, 430, 120, 40, easy_color);
    DrawRectangleLines(sw/2 - 220, 430, 120, 40, WHITE);
    DrawText("Easy", sw/2 - 180, 442, 20, WHITE);
    
    Color medium_color = (menu->selected_diff == MEDIUM) ? (Color){0,150,0,255} : (Color){80,80,100,255};
    DrawRectangle(sw/2 - 60, 430, 120, 40, medium_color);
    DrawRectangleLines(sw/2 - 60, 430, 120, 40, WHITE);
    DrawText("Medium", sw/2 - 35, 442, 20, WHITE);
    
    Color hard_color = (menu->selected_diff == HARD) ? (Color){0,150,0,255} : (Color){80,80,100,255};
    DrawRectangle(sw/2 + 100, 430, 120, 40, hard_color);
    DrawRectangleLines(sw/2 + 100, 430, 120, 40, WHITE);
    DrawText("Hard", sw/2 + 135, 442, 20, WHITE);
    
    // Buton Start Game
    DrawRectangle(sw/2 - 130, 510, 260, 45, (Color){0,100,200,255});
    DrawRectangleLines(sw/2 - 130, 510, 260, 45, WHITE);
    DrawText("START NEW GAME", sw/2 - 95, 523, 20, WHITE);
    
    // Buton Load Game (dacă există save)
    if (save_exists()) {
        DrawRectangle(sw/2 - 130, 570, 260, 45, (Color){150,100,0,255});
        DrawRectangleLines(sw/2 - 130, 570, 260, 45, WHITE);
        DrawText("LOAD SAVED GAME", sw/2 - 95, 583, 20, WHITE);
    } else {
        DrawRectangle(sw/2 - 130, 570, 260, 45, (Color){60,60,60,255});
        DrawRectangleLines(sw/2 - 130, 570, 260, 45, (Color){100,100,100,255});
        DrawText("NO SAVE FOUND", sw/2 - 80, 583, 18, GRAY);
    }
    
    // Buton Leaderboard
    DrawRectangle(sw/2 - 130, 630, 260, 45, (Color){100,100,150,255});
    DrawRectangleLines(sw/2 - 130, 630, 260, 45, WHITE);
    DrawText("LEADERBOARD", sw/2 - 70, 643, 20, WHITE);
    
    // Buton Exit
    DrawRectangle(sw/2 - 130, 690, 260, 45, (Color){150,50,50,255});
    DrawRectangleLines(sw/2 - 130, 690, 260, 45, WHITE);
    DrawText("EXIT GAME", sw/2 - 60, 703, 20, WHITE);
    
    if (menu->error_timer > 0) {
        menu->error_timer -= GetFrameTime();
        if (menu->error_timer <= 0) menu->error_timer = 0;
        int w = MeasureText(menu->error_msg, 20);
        DrawRectangle(sw/2 - w/2 - 10, 760, w + 20, 30, (Color){0,0,0,200});
        DrawText(menu->error_msg, sw/2 - w/2, 770, 20, RED);
    }
}

void handle_start_menu_input(StartMenu *menu, Vector2 mouse, int key) {
    int sw = GetScreenWidth();
    
    if (menu->show_leaderboard) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (mouse.x >= sw/2 - 60 && mouse.x <= sw/2 + 60 &&
                mouse.y >= 650 && mouse.y <= 690) {
                menu->show_leaderboard = 0;
            }
        }
        return;
    }
    
    // Click pe caseta nume
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (mouse.x >= sw/2 - 250 && mouse.x <= sw/2 + 250 &&
            mouse.y >= 200 && mouse.y <= 250) {
            menu->name_active = 1;
        } else {
            menu->name_active = 0;
        }
        
        // Click pe butoane mod
        if (mouse.x >= sw/2 - 150 && mouse.x <= sw/2 - 20 &&
            mouse.y >= 310 && mouse.y <= 355) {
            menu->selected_mode = MODE_CLASSIC;
        }
        if (mouse.x >= sw/2 + 20 && mouse.x <= sw/2 + 150 &&
            mouse.y >= 310 && mouse.y <= 355) {
            menu->selected_mode = MODE_SCALE_UP;
        }
        
        // Click pe dificultate
        if (mouse.x >= sw/2 - 220 && mouse.x <= sw/2 - 100 &&
            mouse.y >= 430 && mouse.y <= 470) menu->selected_diff = EASY;
        if (mouse.x >= sw/2 - 60 && mouse.x <= sw/2 + 60 &&
            mouse.y >= 430 && mouse.y <= 470) menu->selected_diff = MEDIUM;
        if (mouse.x >= sw/2 + 100 && mouse.x <= sw/2 + 220 &&
            mouse.y >= 430 && mouse.y <= 470) menu->selected_diff = HARD;
        
        // Click pe START NEW GAME
        if (mouse.x >= sw/2 - 130 && mouse.x <= sw/2 + 130 &&
            mouse.y >= 510 && mouse.y <= 555) {
            if (strlen(menu->player_name) == 0) {
                strcpy(menu->error_msg, "Please enter your name!");
                menu->error_timer = 2.0f;
            } else {
                menu->start_game = 1;
            }
        }
        
        // Click pe LOAD SAVED GAME
        if (save_exists() && mouse.x >= sw/2 - 130 && mouse.x <= sw/2 + 130 &&
            mouse.y >= 570 && mouse.y <= 615) {
            if (strlen(menu->player_name) == 0) {
                strcpy(menu->player_name, "Player");
            }
            menu->start_game = 2; // 2 = load game
        }
        
        // Click pe LEADERBOARD
        if (mouse.x >= sw/2 - 130 && mouse.x <= sw/2 + 130 &&
            mouse.y >= 630 && mouse.y <= 675) {
            menu->show_leaderboard = 1;
        }
        
        // Click pe EXIT GAME
        if (mouse.x >= sw/2 - 130 && mouse.x <= sw/2 + 130 &&
            mouse.y >= 690 && mouse.y <= 735) {
            exit(0);
        }
    }
    
    // Input tastatură pentru nume
    if (menu->name_active && key > 0) {
        if (key == KEY_BACKSPACE && strlen(menu->player_name) > 0) {
            menu->player_name[strlen(menu->player_name) - 1] = '\0';
        } else if (key == KEY_ENTER) {
            menu->name_active = 0;
            if (strlen(menu->player_name) == 0) {
                strcpy(menu->player_name, "Player");
            }
        } else if (key >= KEY_A && key <= KEY_Z && strlen(menu->player_name) < NAME_LEN - 1) {
            int len = strlen(menu->player_name);
            menu->player_name[len] = tolower(key - KEY_A + 'a');
            menu->player_name[len + 1] = '\0';
        } else if (key == KEY_SPACE && strlen(menu->player_name) < NAME_LEN - 1) {
            int len = strlen(menu->player_name);
            menu->player_name[len] = ' ';
            menu->player_name[len + 1] = '\0';
        }
    }
}

void init_start_menu(StartMenu *menu) {
    strcpy(menu->player_name, "Player");
    menu->selected_mode = MODE_CLASSIC;
    menu->selected_diff = EASY;
    menu->start_game = 0;
    menu->name_active = 0;
    menu->error_timer = 0;
    menu->show_leaderboard = 0;
    strcpy(menu->error_msg, "");
}
