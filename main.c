#include <stdio.h>

typedef unsigned long long U64;
typedef struct {
    U64 p1_pieces;
    U64 p1_kings;
    U64 p2_pieces;
    U64 p2_kings;
    int current_turn;
} GameState;

/* from checkers.c */
void init_game(GameState* g);
void print_board(const GameState* g);
int  try_move_simple(GameState* g, int from_r, int from_c, int to_r, int to_c);
int  check_winner(const GameState* g);

int main(void) {
    GameState game;
    init_game(&game);
    print_board(&game);

    while (1) {
        int w = check_winner(&game);
        if (w) { printf("Player %d wins!\n", w); break; }

        int fr, fc, tr, tc;
        printf("Player %d: enter move as four numbers: from row from column to row to column\n", game.current_turn);
        printf("Example: 2 1 3 0   (-1 to quit)\n> ");

        if (scanf("%d", &fr) != 1) return 0;
        if (fr == -1) break;
        if (scanf("%d %d %d", &fc, &tr, &tc) != 3) return 0;

        if (try_move_simple(&game, fr, fc, tr, tc)) {
            print_board(&game);
        } else {
            printf("Illegal move.\n");
        }
    }
    return 0;
}
