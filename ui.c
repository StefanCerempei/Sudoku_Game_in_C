#include "ui.h"
#include "leaderboard.h"

void init_ui(UIState *ui) {
    memset(ui, 0, sizeof(UIState));
    ui->show_menu = 0;
    ui->show_leaderboard = 0;
    ui->message_timer = 0;
}

void draw_grid(const Grid *g) {
    int cell_w = CELL_SIZE, cell_h = CELL_SIZE;
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            int x = GRID_OFFSET_X + c*cell_w, y = GRID_OFFSET_Y + r*cell_h;
            Color bg = (r==g->selected_row && c==g->selected_col) ? (Color){50,100,200,255} : 
                       ((r+c)%2==0) ? (Color){30,30,40,255} : (Color){40,40,50,255};
            DrawRectangle(x, y, cell_w, cell_h, bg);
            DrawRectangleLines(x, y, cell_w, cell_h, DARKGRAY);
            if (g->cells[r][c].value) {
                char text[3] = { digit_to_char(g->cells[r][c].value), 0 };
                Color tc = g->cells[r][c].given ? (Color){255,255,100,255} : (g->cells[r][c].error ? RED : GREEN);
                int font_size = (GRID_SIZE == 9) ? 32 : 20;
                int tw = MeasureText(text, font_size);
                DrawText(text, x+cell_w/2-tw/2, y+cell_h/2-font_size/2, font_size, tc);
            }
        }
    }
    for (int i = 1; i < BOX_SIZE; i++) {
        int ly = GRID_OFFSET_Y + i*BOX_SIZE*CELL_SIZE;
        DrawLine(GRID_OFFSET_X, ly, GRID_OFFSET_X+GRID_SIZE*CELL_SIZE, ly, YELLOW);
        int lx = GRID_OFFSET_X + i*BOX_SIZE*CELL_SIZE;
        DrawLine(lx, GRID_OFFSET_Y, lx, GRID_OFFSET_Y+GRID_SIZE*CELL_SIZE, YELLOW);
    }
}

void draw_buttons(UIState *ui) {
    (void)ui;
}

void draw_status(const Grid *g, UIState *ui) {
    int elapsed = get_elapsed_seconds(g);
    char time_str[32], mistakes_str[32];
    sprintf(time_str, "Time: %02d:%02d", elapsed/60, elapsed%60);
    sprintf(mistakes_str, "Mistakes: %d/5", g->mistakes);
    DrawText(time_str, 20, 20, 25, (Color){0,255,255,255});
    DrawText(mistakes_str, 20, 50, 25, g->mistakes>=5 ? RED : (g->mistakes>0 ? ORANGE : GREEN));
    DrawText(ui->player_name, 20, 80, 20, LIGHTGRAY);
    char mode_str[50];
    sprintf(mode_str, "Mode: %s %dx%d", g->game_mode == MODE_CLASSIC ? "Classic" : "Scale-up", GRID_SIZE, GRID_SIZE);
    DrawText(mode_str, 20, 110, 18, YELLOW);
    DrawText("ESC: Scores | H: Hint | DEL: Erase | Arrows: Move", 20, SCREEN_HEIGHT - 30, 16, GRAY);
    
    if (g->win) DrawText("YOU WIN!", SCREEN_WIDTH/2-80, SCREEN_HEIGHT/2-50, 40, GREEN);
    if (g->mistakes >= 5) DrawText("GAME OVER!", SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2-50, 40, RED);
}

void draw_leaderboard(UIState *ui) {
    DrawRectangle(SCREEN_WIDTH/2-350, 80, 700, 600, (Color){0,0,0,240});
    DrawRectangleLines(SCREEN_WIDTH/2-350, 80, 700, 600, YELLOW);
    DrawText("LEADERBOARD", SCREEN_WIDTH/2-120, 110, 40, YELLOW);
    ScoreEntry scores[10];
    int count = 0;
    lb_load(scores, &count);
    
    DrawText("Rank", SCREEN_WIDTH/2-300, 170, 20, WHITE);
    DrawText("Name", SCREEN_WIDTH/2-220, 170, 20, WHITE);
    DrawText("Time", SCREEN_WIDTH/2-20, 170, 20, WHITE);
    DrawText("Mistakes", SCREEN_WIDTH/2+100, 170, 20, WHITE);
    DrawText("Mode", SCREEN_WIDTH/2+200, 170, 20, WHITE);
    
    for (int i = 0; i < count && i < 15; i++) {
        char line[128];
        char mode_text[20];
        sprintf(mode_text, "%dx%d", scores[i].difficulty == EASY ? 9 : 16, scores[i].difficulty == EASY ? 9 : 16);
        sprintf(line, "%d", i+1);
        DrawText(line, SCREEN_WIDTH/2-300, 200 + i*35, 20, LIGHTGRAY);
        DrawText(scores[i].name, SCREEN_WIDTH/2-220, 200 + i*35, 20, LIGHTGRAY);
        sprintf(line, "%02d:%02d", scores[i].time_seconds/60, scores[i].time_seconds%60);
        DrawText(line, SCREEN_WIDTH/2-30, 200 + i*35, 20, LIGHTGRAY);
        sprintf(line, "%d", scores[i].mistakes);
        DrawText(line, SCREEN_WIDTH/2+100, 200 + i*35, 20, LIGHTGRAY);
        DrawText(mode_text, SCREEN_WIDTH/2+200, 200 + i*35, 20, LIGHTGRAY);
    }
    
    DrawText("Press ESC to continue", SCREEN_WIDTH/2-120, 650, 20, GRAY);
}

void draw_message(UIState *ui) {
    if (ui->message_timer > 0) {
        ui->message_timer -= GetFrameTime();
        int w = MeasureText(ui->message, 25);
        DrawRectangle(SCREEN_WIDTH/2-w/2-10, SCREEN_HEIGHT-100, w+20, 40, (Color){0,0,0,200});
        DrawText(ui->message, SCREEN_WIDTH/2-w/2, SCREEN_HEIGHT-95, 25, YELLOW);
    }
}

void show_message(UIState *ui, const char *msg) {
    strncpy(ui->message, msg, 255);
    ui->message_timer = 2.0f;
}

void draw_exit_button(void) {
    int btn_x = SCREEN_WIDTH - 100;
    int btn_y = 20;
    DrawRectangle(btn_x, btn_y, 80, 30, (Color){150,50,50,255});
    DrawRectangleLines(btn_x, btn_y, 80, 30, WHITE);
    DrawText("EXIT", btn_x + 22, btn_y + 8, 16, WHITE);
}

int is_exit_button_clicked(Vector2 mouse) {
    int btn_x = SCREEN_WIDTH - 100;
    int btn_y = 20;
    return (mouse.x >= btn_x && mouse.x <= btn_x + 80 &&
            mouse.y >= btn_y && mouse.y <= btn_y + 30);
}
