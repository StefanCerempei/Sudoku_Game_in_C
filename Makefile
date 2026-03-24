# ─── Sudoku 9×9 – Makefile ──────────────────────────────────────────
CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c11 -O2 -g
TARGET  = sudoku
SRC     = main.c game_logic.c generator.c ui_display.c leaderboard.c
OBJ     = $(SRC:.c=.o)

# ─── Default target ──────────────────────────────────────────────────
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# ─── Pattern rule for object files ──────────────────────────────────
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# ─── Header dependencies (keep it simple) ───────────────────────────
main.o:        main.c       game_logic.h generator.h ui_display.h leaderboard.h utils.h
game_logic.o:  game_logic.c game_logic.h utils.h
generator.o:   generator.c  generator.h  game_logic.h utils.h
ui_display.o:  ui_display.c ui_display.h game_logic.h leaderboard.h utils.h
leaderboard.o: leaderboard.c leaderboard.h ui_display.h utils.h

# ─── Utility targets ─────────────────────────────────────────────────
run: all
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

clean-scores:
	rm -f leaderboard.csv

valgrind: all
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

.PHONY: all run clean clean-scores valgrind