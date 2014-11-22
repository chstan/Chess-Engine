#ifndef EVALHELPERS_H
#define EVALHELPERS_H

#include "../board/board.h"

void initEval();

void build_pawn_ranges(Board *p_board,
                       U64 *wpfr, U64 *wpbr,U64 *bpfr, U64 *bpbr,
                       U64 *wpwfr, U64 *bpwfr);

int all_piece_square_scores (Board *p_board);

int doubled_pawns(Board *p_board);

int tripled_pawns(Board *p_board);

int passed_pawns(Board *p_board, U64 white_pawn_forward_range,
                 U64 black_pawn_forward_range);

int outside_passed_pawns(Board *p_board, U64 white_pawn_forward_range,
                         U64 black_pawn_forward_range);

int bishop_mobility(Board *p_board);

int rook_mobility(Board *p_board);

int king_safety(Board *p_board);

int control(Board *p_board, int square);

int control_of_center(Board *p_board);

#endif
