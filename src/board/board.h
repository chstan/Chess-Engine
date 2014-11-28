#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

#include "../defines.h"
#include "../zobrist.h"

#define MAX_MOVES_PER_GAME 512

// TODO: use lookup tables to determine when discovered checks occur
//       then monitoring for check is as easy as seeing whether the
//       moved/promoted piece attacks the king, or a slider has discovered
//       check, by using a fast MSB/LSB to check only one slider at most,
//       this process can be made very quick

typedef struct {
    U64 piece_BB[16];

    U64 white_occupied;
    U64 black_occupied;
    U64 occupied;

    int square[64];

    signed char pieces[16];
    signed char total_white;
    signed char total_black;
    signed char total_pieces;
} Position;

typedef struct {
    unsigned char castle_white;
    unsigned char castle_black;

    // the en passant index
    // represents where a pawn can capture to, rather than
    // where the pawn actually is
    int en_passant_square;
    int stale_moves;

    U64 _zobrist_key;

    // this doesn't make me happy, but it works for the moment
    Move move;

} MoveInfo;

typedef struct {
    int material;
    int white_material;
    int black_material;

    unsigned char to_play;
    MoveInfo state[MAX_MOVES_PER_GAME];

    // indexs into the array, gives us the number
    // of moves that have elapsed so far
    unsigned int current_move;

    bool display_rotated;
} BoardInfo;

typedef struct {
    Position position;
    BoardInfo info;
} Board;

//--------------------RESET AND DISPLAY----------------------

void reset_board(Board *p_board);

void init_board_from_squares(Board* p_board, unsigned char to_play, int stale_moves, int castle_W,
    int castle_B, int en_passant_square, int turn_count);

void display_board(Board *p_board);

void rotate_board(Board *p_board);

//------------------------DEBUG------------------------------
bool debug_board(Board *p_board);

//---------------------MAKE FUNCTIONS------------------------
void set_empty_at(Board *p_board, UCHAR index, UCHAR last_occupant);

void set_piece_at(Board *p_board, UCHAR index, UCHAR moved_piece, UCHAR captured_piece);

void update_en_passant_square(Board *p_board, int index);

void en_passant(Board *p_board, int color);

void un_passant(Board *p_board, int index, int color);

void castle(Board *p_board, UCHAR index, UCHAR which_king);

void un_castle(Board *p_board, UCHAR index, UCHAR which_king);

void do_promote(Board *p_board, UCHAR index, UCHAR which_promote);

void un_promote(Board *p_board, UCHAR index);

void add_material(Board *p_board, UCHAR piece);

void remove_material(Board *p_board, UCHAR piece);

//---------------------AUX FUNCTIONS-------------------------
void update_piece_counts_from_BB(Board *p_board);

void reset_BB(Board *p_board);

void update_aggregate_BB(Board *p_board);

void update_BB_from_squares(Board *p_board);

void update_material_from_BB(Board *p_board);

U64 full_zobrist_key(Board *p_board);

#endif
