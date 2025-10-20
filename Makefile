# Makefile for BitBoard Checkers (C)
CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -O2
LDFLAGS :=

SRC := main.c bitops.c checkers.c
OBJ := $(SRC:.c=.o)
BIN := checkers

.PHONY: all run clean debug

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN)

debug: CFLAGS := -std=c11 -Wall -Wextra -O0 -g
debug: clean $(BIN)

clean:
	$(RM) $(OBJ) $(BIN)
