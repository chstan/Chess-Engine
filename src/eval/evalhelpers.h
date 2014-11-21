#ifndef EVALHELPERS_H
#define EVALHELPERS_H

#include "../board/board.h"

void initEval();

void build_pawn_ranges(Board *pBoard,
                       BitBoard *wpfr, BitBoard *wpbr,BitBoard *bpfr, BitBoard *bpbr,
                       BitBoard *wpwfr, BitBoard *bpwfr);

int all_piece_square_scores (Board *pBoard);

int doubledPawns(Board *pBoard);

int tripledPawns(Board *pBoard);

int passed_pawns(Board *pBoard, BitBoard white_pawn_forward_range,
                 BitBoard black_pawn_forward_range);

int outside_passed_pawns(Board *pBoard, BitBoard white_pawn_forward_range,
                         BitBoard black_pawn_forward_range);

int bishopMobility(Board *pBoard);

int rookMobility(Board *pBoard);

int kingSafety(Board *pBoard);

int control(Board *pBoard, int square);

int controlOfCenter(Board *pBoard);

#endif
