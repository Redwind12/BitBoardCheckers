#include <stdio.h>

typedef unsigned long long U64;

typedef struct {
    U64 p1_pieces;   /* 'r' */
    U64 p1_kings;    /* 'R' */
    U64 p2_pieces;   /* 'b' */
    U64 p2_kings;    /* 'B' */
    int current_turn;/* 1 or 2 */
} GameState;

/* helpers */
static int idx(int row, int col) { return row * 8 + col; }
static int is_dark_square(int row, int col) { return (row + col) % 2 == 1; }
static int get_bit64(U64 bb, int pos) { return (pos >= 0 && pos < 64) ? (int)((bb >> pos) & 1ULL) : 0; }
static int inside_board(int r, int c) { return r >= 0 && r < 8 && c >= 0 && c < 8; }
static U64 all_occupied(const GameState* g) { return g->p1_pieces | g->p1_kings | g->p2_pieces | g->p2_kings; }
static int square_empty(const GameState* g, int r, int c) { return ((all_occupied(g) >> idx(r,c)) & 1ULL) == 0ULL; }
static int abs_i(int x) { return x < 0 ? -x : x; }
static int is_opponent_at(const GameState* g, int player, int r, int c) {
    int p = idx(r, c);
    if (player == 1) return ((g->p2_pieces >> p) & 1ULL) || ((g->p2_kings >> p) & 1ULL);
    else             return ((g->p1_pieces >> p) & 1ULL) || ((g->p1_kings >> p) & 1ULL);
}

/* init */
void init_game(GameState* g) {
    g->p1_pieces = g->p1_kings = g->p2_pieces = g->p2_kings = 0ULL;
    g->current_turn = 1;

    for (int r = 0; r <= 2; r++)
        for (int c = 0; c < 8; c++)
            if (is_dark_square(r, c)) g->p1_pieces |= (1ULL << idx(r, c));

    for (int r = 5; r <= 7; r++)
        for (int c = 0; c < 8; c++)
            if (is_dark_square(r, c)) g->p2_pieces |= (1ULL << idx(r, c));
}

/* display */
void print_board(const GameState* g) {
    printf("\n    0 1 2 3 4 5 6 7 (columns)\n");
    printf("   -------------------------\n");
    for (int r = 0; r < 8; r++) {
        printf("%d | ", r);
        for (int c = 0; c < 8; c++) {
            if (!is_dark_square(r, c)) { printf(". "); continue; }
            int p = idx(r, c);
            char ch = '-';
            if      (get_bit64(g->p1_kings, p))  ch = 'R';
            else if (get_bit64(g->p1_pieces, p)) ch = 'r';
            else if (get_bit64(g->p2_kings, p))  ch = 'B';
            else if (get_bit64(g->p2_pieces, p)) ch = 'b';
            printf("%c ", ch);
        }
        printf("\n");
    }
    printf("   -------------------------\n");
    printf("Turn: Player %d\n\n", g->current_turn);
}

/* promotion */
static void maybe_promote(GameState* g, int to_r, int to_c, int player, int was_king) {
    if (was_king) return;
    int p = idx(to_r, to_c);
    if (player == 1 && to_r == 7) {
        g->p1_pieces &= ~(1ULL << p);
        g->p1_kings  |=  (1ULL << p);
    } else if (player == 2 && to_r == 0) {
        g->p2_pieces &= ~(1ULL << p);
        g->p2_kings  |=  (1ULL << p);
    }
}

/* move: simple step OR single capture; no mandatory capture; no chaining */
int try_move_simple(GameState* g, int from_r, int from_c, int to_r, int to_c) {
    if (!inside_board(from_r, from_c) || !inside_board(to_r, to_c)) { printf("Out of bounds.\n"); return 0; }
    if (!is_dark_square(from_r, from_c) || !is_dark_square(to_r, to_c)) { printf("Use dark squares only.\n"); return 0; }
    if (!square_empty(g, to_r, to_c)) { printf("Target not empty.\n"); return 0; }

    int from_p = idx(from_r, from_c);
    int to_p   = idx(to_r, to_c);

    int player = 0, king = 0;
    if      ((g->p1_kings  >> from_p) & 1ULL) { player = 1; king = 1; }
    else if ((g->p1_pieces >> from_p) & 1ULL) { player = 1; king = 0; }
    else if ((g->p2_kings  >> from_p) & 1ULL) { player = 2; king = 1; }
    else if ((g->p2_pieces >> from_p) & 1ULL) { player = 2; king = 0; }
    else { printf("No piece at source.\n"); return 0; }

    if (player != g->current_turn) { printf("Not your turn.\n"); return 0; }

    int dr = to_r - from_r;
    int dc = to_c - from_c;

    /* simple 1-step */
    if (abs_i(dr) == 1 && abs_i(dc) == 1) {
        if (!king) {
            if (player == 1 && dr != 1)  { printf("P1 moves down.\n"); return 0; }
            if (player == 2 && dr != -1) { printf("P2 moves up.\n");   return 0; }
        }
        if (player == 1) {
            if (king) { g->p1_kings  &= ~(1ULL << from_p); g->p1_kings  |= (1ULL << to_p); }
            else      { g->p1_pieces &= ~(1ULL << from_p); g->p1_pieces |= (1ULL << to_p); }
        } else {
            if (king) { g->p2_kings  &= ~(1ULL << from_p); g->p2_kings  |= (1ULL << to_p); }
            else      { g->p2_pieces &= ~(1ULL << from_p); g->p2_pieces |= (1ULL << to_p); }
        }
        maybe_promote(g, to_r, to_c, player, king);
        g->current_turn = (g->current_turn == 1) ? 2 : 1;
        return 1;
    }

    /* single 2-step capture */
    if (abs_i(dr) == 2 && abs_i(dc) == 2) {
        int mid_r = from_r + dr/2, mid_c = from_c + dc/2;
        if (!is_opponent_at(g, player, mid_r, mid_c)) { printf("No opponent to capture.\n"); return 0; }
        if (!king) {
            if (player == 1 && dr != 2)  { printf("P1 captures down.\n"); return 0; }
            if (player == 2 && dr != -2) { printf("P2 captures up.\n");   return 0; }
        }

        int mid_p = idx(mid_r, mid_c);
        if (player == 1) {
            g->p2_pieces &= ~(1ULL << mid_p);
            g->p2_kings  &= ~(1ULL << mid_p);
        } else {
            g->p1_pieces &= ~(1ULL << mid_p);
            g->p1_kings  &= ~(1ULL << mid_p);
        }

        if (player == 1) {
            if (king) { g->p1_kings  &= ~(1ULL << from_p); g->p1_kings  |= (1ULL << to_p); }
            else      { g->p1_pieces &= ~(1ULL << from_p); g->p1_pieces |= (1ULL << to_p); }
        } else {
            if (king) { g->p2_kings  &= ~(1ULL << from_p); g->p2_kings  |= (1ULL << to_p); }
            else      { g->p2_pieces &= ~(1ULL << from_p); g->p2_pieces |= (1ULL << to_p); }
        }

        maybe_promote(g, to_r, to_c, player, king);
        g->current_turn = (g->current_turn == 1) ? 2 : 1; /* turn always switches */
        return 1;
    }

    printf("Illegal: only 1-step or 2-step diagonals.\n");
    return 0;
}

/* basic winner: by pieces remaining only */
static int player_has_pieces(const GameState* g, int player) {
    return player == 1 ? (g->p1_pieces | g->p1_kings) != 0ULL
                       : (g->p2_pieces | g->p2_kings) != 0ULL;
}
int check_winner(const GameState* g) {
    if (!player_has_pieces(g, 1)) return 2;
    if (!player_has_pieces(g, 2)) return 1;
    return 0;
}
