#ifndef CHECKERS_H
#define CHECKERS_H

#include <stdint.h>

typedef uint64_t U64;

typedef struct {
    U64 p1_pieces;
    U64 p1_kings;
    U64 p2_pieces;
    U64 p2_kings;
    int current_turn;
} GameState;

void init_game(GameState* g);
void print_board(const GameState* g);
int  try_move(GameState* g, int from_r, int from_c, int to_r, int to_c);
int  check_winner(const GameState* g);

#endif
