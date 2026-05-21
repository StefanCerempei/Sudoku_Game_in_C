CC = gcc
CFLAGS = -Wall -O2 -std=c99
LIBS = -lraylib -lm -lpthread -ldl -lrt -lX11

all:
	gcc $(CFLAGS) -o sudoku src/main.c src/game_logic.c src/generator.c src/ui.c src/leaderboard.c src/start_menu.c src/save_game.c $(LIBS)

clean:
	rm -f sudoku sudoku_save.csv

run: all
	./sudoku
