#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "evalhelpers.h"
#include "../defines.h"
#include "../extglobals.h"
#include "../bit.h"

// TODO
/*
 * Rook, Bishop, Queen, Knight outposts and outpost value tables
 * Material hash tables
 * Pawn value hash tables
 * Evaluation hash tables
 * Endgame hash tables
 * Tempo Bonus, lost tempo penalty
 * Trapped rook no castling penalty
 * Uncatchable pawns
 *
 * Maintain attack tables which should be incrementally updated <- PLEASE!
 *
 * Normalize scores with tunable constants, this will allow machine learning
 * to tune the evaluators externally using a program like CuteChess to run
 * tournaments
 */

// we could generalize to x files, and should
U64 forward_range[2][64];
U64 backward_range[2][64];

U64 outsidePassedPawn[2][64];

U64 king_safetyZone[2][64];
U64 pawnShield[2][64];

void build_pawn_ranges(Board *p_board,
                       U64 *wpfr, U64 *wpbr,U64 *bpfr, U64 *bpbr,
                       U64 *wpwfr, U64 *bpwfr) {
    // white pawn forward range
    *wpfr = p_board->position.piece_BB[WHITE_PAWN];
    U64 pawns = *wpfr;
    int origin = -1, shift = 0;
    while (pawns) {
        shift = LSB(pawns) + 1;
        if (shift < 64) pawns >>= shift;
        origin += shift;
        *wpfr |= forward_range[WHITE][origin];
    }
    // ...
    *wpbr = p_board->position.piece_BB[WHITE_PAWN];
    pawns = *wpbr;
    origin = -1;
    shift = 0;
    while (pawns) {
        shift = LSB(pawns) + 1;
        if (shift < 64) pawns >>= shift;
        origin += shift;
        *wpbr |= forward_range[BLACK][origin];
    }

    *bpfr = p_board->position.piece_BB[BLACK_PAWN];
    pawns = *bpfr;
    origin = -1;
    shift = 0;
    while (pawns) {
        shift = LSB(pawns) + 1;
        if (shift < 64) pawns >>= shift;
        origin += shift;
        *bpfr |= forward_range[BLACK][origin];
    }

    *bpbr = p_board->position.piece_BB[BLACK_PAWN];
    pawns = *bpbr;
    origin = -1;
    shift = 0;
    while (pawns) {
        shift = LSB(pawns) + 1;
        if (shift < 64) pawns >>= shift;
        origin += shift;
        *bpbr |= forward_range[WHITE][origin];
    }
    // build wide forward ranges from forward ranges
    // basic process: mask off A and H files and smear them
    // unfinished. Not sure what will be effective for outside passed
    // pawn detection.
    // *wpwfr = *wpfr & (~fileBB[0] & ~fileBB[7]);
    *wpwfr = 0;
    *bpwfr = 0;
}

// flips a square vertically
int vflip_square(int sq) {
    return sq ^ 56;
}

// Preliminary piece square tables
// ordering A1->A2->...->B8
//           |
//          B1->B2->...->B8
//           |
//           -
//           |
//          H1->H2->...->H8
// so be sure to flip!
const int pawn_squares[64] = {0,  0,  0,  0,  0,  0,  0,  0,
                              50, 50, 50, 50, 50, 50, 50, 50,
                              10, 10, 20, 30, 30, 20, 10, 10,
                              5,  5, 10, 25, 25, 10,  5,  5,
                              0,  0,  0, 20, 20,  0,  0,  0,
                              5, -5,-10,  0,  0,-10, -5,  5,
                              5, 10, 10,-20,-20, 10, 10,  5,
                              0,  0,  0,  0,  0,  0,  0,  0};
const int knight_squares[64] = {-50,-40,-30,-30,-30,-30,-40,-50,
                                -40,-20,  0,  0,  0,  0,-20,-40,
                                -30,  0, 10, 15, 15, 10,  0,-30,
                                -30,  5, 15, 20, 20, 15,  5,-30,
                                -30,  0, 15, 20, 20, 15,  0,-30,
                                -30,  5, 10, 15, 15, 10,  5,-30,
                                -40,-20,  0,  5,  5,  0,-20,-40,
                                -50,-40,-30,-30,-30,-30,-40,-50};
const int bishop_squares[64] = {-20,-10,-10,-10,-10,-10,-10,-20,
                                -10,  0,  0,  0,  0,  0,  0,-10,
                                -10,  0,  5, 10, 10,  5,  0,-10,
                                -10,  5,  5, 10, 10,  5,  5,-10,
                                -10,  0, 10, 10, 10, 10,  0,-10,
                                -10, 10, 10, 10, 10, 10, 10,-10,
                                -10,  5,  0,  0,  0,  0,  5,-10,
                                -20,-10,-10,-10,-10,-10,-10,-20};
const int rook_squares[64] = {0,  0,  0,  0,  0,  0,  0,  0,
                              5, 10, 10, 10, 10, 10, 10,  5,
                              -5,  0,  0,  0,  0,  0,  0, -5,
                              -5,  0,  0,  0,  0,  0,  0, -5,
                              -5,  0,  0,  0,  0,  0,  0, -5,
                              -5,  0,  0,  0,  0,  0,  0, -5,
                              -5,  0,  0,  0,  0,  0,  0, -5,
                              0,  0,  0,  5,  5,  0,  0,  0};
const int queen_squares[64] = {-20,-10,-10, -5, -5,-10,-10,-20,
                               -10,  0,  0,  0,  0,  0,  0,-10,
                               -10,  0,  5,  5,  5,  5,  0,-10,
                               -5,  0,  5,  5,  5,  5,  0, -5,
                               0,  0,  5,  5,  5,  5,  0, -5,
                               -10,  5,  5,  5,  5,  5,  0,-10,
                               -10,  0,  5,  5,  0,  0,  0,-10,
                               -20,-10,-10, -5, -5,-10,-10,-20};

// should use different piece square table for late in the game
const int king_squares_early[64] = {-30,-40,-40,-50,-50,-40,-40,-30,
                                    -30,-40,-40,-50,-50,-40,-40,-30,
                                    -30,-40,-40,-50,-50,-40,-40,-30,
                                    -30,-40,-40,-50,-50,-40,-40,-30,
                                    -20,-30,-30,-40,-40,-30,-30,-20,
                                    -10,-20,-20,-20,-20,-20,-20,-10,
                                    20, 20,  0,  0,  0,  0, 20, 20,
                                    20, 30, 10,  0,  0, 10, 30, 20};

const int king_squares_late[64] = {0, 0, 0, 0, 0, 0, 0, 0,
                                   0, 0, 5, 5, 5, 5, 0, 0,
                                   0, 5, 7, 8, 8, 7, 5, 0,
                                   0, 5, 8, 15, 15, 8, 5, 0,
                                   0, 5, 8, 15, 15, 8, 5, 0,
                                   0, 5, 7, 8, 8, 7, 5, 0,
                                   0, 0, 5, 5, 5, 5, 0, 0,
                                   0, 0, 0, 0, 0, 0, 0, 0};


int piece_square_difference(U64 white_pieces, U64 black_pieces,
                      const int *piece_squares) {
    int origin = -1, shift = 0;
    int score = 0;
    while (white_pieces) {
        shift = LSB(white_pieces) + 1;
        if (shift < 64) white_pieces >>= shift;
        else white_pieces = 0;
        origin += shift;
        score += piece_squares[vflip_square(origin)];
    }
    origin = -1;
    shift = 0;
    while (black_pieces) {
        shift = LSB(black_pieces) + 1;
        if (shift < 64) black_pieces >>= shift;
        else black_pieces = 0;
        origin += shift;
        score -= piece_squares[origin];
    }
    return score;
}

int all_piece_square_scores (Board *p_board) {
    int score;
    score = piece_square_difference(p_board->position.piece_BB[WHITE_PAWN],
                                     p_board->position.piece_BB[BLACK_PAWN],
                                     pawn_squares);
    score += piece_square_difference(p_board->position.piece_BB[WHITE_KNIGHT],
                                     p_board->position.piece_BB[BLACK_KNIGHT],
                                     knight_squares);
    score += piece_square_difference(p_board->position.piece_BB[WHITE_BISHOP],
                                     p_board->position.piece_BB[BLACK_BISHOP],
                                     bishop_squares);
    score += piece_square_difference(p_board->position.piece_BB[WHITE_ROOK],
                                     p_board->position.piece_BB[BLACK_ROOK],
                                     rook_squares);
    score += piece_square_difference(p_board->position.piece_BB[WHITE_QUEEN],
                                     p_board->position.piece_BB[BLACK_QUEEN],
                                     queen_squares);
    if (p_board->info.current_move > 50 || p_board->info.material < 2000) {
        // better would be to do checks on remaining minor pieces, I think
        score += piece_square_difference(p_board->position.piece_BB[WHITE_KING],
                                         p_board->position.piece_BB[BLACK_KING],
                                         king_squares_early);
    } else {
        score += piece_square_difference(p_board->position.piece_BB[WHITE_KING],
                                         p_board->position.piece_BB[BLACK_KING],
                                         king_squares_late);
    }

    return score;
}

void initEval() {
    for(int rank = 0; rank < 8; rank++) {
        for(int file = 0; file < 8; file++) {
            int square = SQ(rank+1, file+1);

            U64 forward_mask = 0;

            U64 outsideMask = 0;
            for(int fileIndex = 0; fileIndex < 8; fileIndex++) {
                if(abs(fileIndex - file) < 2) {
                    forward_mask |= fileBB[fileIndex];
                }
                if(abs(fileIndex - file) < 3)
                    outsideMask |= fileBB[fileIndex];
            }

            if (square < A8)
                forward_range[WHITE][square] = forward_mask & ~(BITSET(square + 2) - 1);
            else
                forward_range[WHITE][square] = 0;

            if (square >= A2)
                forward_range[BLACK][square] = forward_mask & (BITSET(square - 6) - 1);
            else
                forward_range[BLACK][square] = 0;

            if(square < A8)
                outsidePassedPawn[WHITE][square] = outsideMask & ~(BITSET(square + 3) - 1);
            else
                outsidePassedPawn[WHITE][square] = 0;

            if(square >= A2)
                outsidePassedPawn[BLACK][square] = outsideMask & (BITSET(square - 5) - 1);
            else
                outsidePassedPawn[BLACK][square] = 0;
        }
    }
}



int doubled_pawns(Board *p_board) {
    int doubled_pawns = 0;
    for(int currentFile = 0; currentFile < 8; currentFile++) {
        if(count_bits(p_board->position.piece_BB[WHITE_PAWN] & fileBB[currentFile]) == 2) doubled_pawns++;
        if(count_bits(p_board->position.piece_BB[BLACK_PAWN] & fileBB[currentFile]) == 2) doubled_pawns--;
    }
    return doubled_pawns;
}

int tripled_pawns(Board *p_board) {
    int tripled_pawns = 0;
    for(int currentFile = 0; currentFile < 8; currentFile++) {
        if(count_bits(p_board->position.piece_BB[WHITE_PAWN] & fileBB[currentFile]) == 3) tripled_pawns++;
        if(count_bits(p_board->position.piece_BB[BLACK_PAWN] & fileBB[currentFile]) == 3) tripled_pawns--;
    }
    return tripled_pawns;
}

int passed_pawns(Board *p_board, U64 white_pawn_forward_range,
                 U64 black_pawn_forward_range) {
    // returns the passed pawn delta (+ for white)
    int passed_pawn_count =
        ((int) count_bits(p_board->position.piece_BB[WHITE_PAWN] & ~black_pawn_forward_range))-
        ((int) count_bits(p_board->position.piece_BB[BLACK_PAWN] & ~white_pawn_forward_range));
    return passed_pawn_count;
}

int outside_passed_pawns(Board *p_board,
                         __attribute__((unused)) U64 white_pawn_wide_forward_range,
                         __attribute__((unused)) U64 black_pawn_wide_forward_range) {
    int outsidePawnCount = 0;
    // not as fast as an optimized bitboard method, but this is all still proof of concept
    for(int square = A1; square <= H8; square++) {
        int piece = p_board->position.square[square];
        if(piece == WHITE_PAWN) {
            if(!(outsidePassedPawn[WHITE][square] & p_board->position.piece_BB[BLACK_PAWN]))
                outsidePawnCount++;
        } else if(piece == BLACK_PAWN) {
            if(!(outsidePassedPawn[BLACK][square] & p_board->position.piece_BB[WHITE_PAWN]))
                outsidePawnCount--;
        }
    }
    return outsidePawnCount;
}

int bishop_mobility(Board *p_board);

int rook_mobility(Board *p_board);

int king_safety(Board *p_board);

int control_of_center(Board *p_board);

int control(__attribute__((unused)) Board *p_board,
            __attribute__((unused)) int square) {
    // we count the number of white pieces attacking the square, and subtract the number of black pieces.
    assert(false);
    return 0;
}
