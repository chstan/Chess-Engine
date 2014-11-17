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

// flips a square vertically
int vflip_square(int sq) {
    return sq ^ 56;
}

// Preliminary piece square tables
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
const int king_squares[64] = {-30,-40,-40,-50,-50,-40,-40,-30,
                              -30,-40,-40,-50,-50,-40,-40,-30,
                              -30,-40,-40,-50,-50,-40,-40,-30,
                              -30,-40,-40,-50,-50,-40,-40,-30,
                              -20,-30,-30,-40,-40,-30,-30,-20,
                              -10,-20,-20,-20,-20,-20,-20,-10,
                              20, 20,  0,  0,  0,  0, 20, 20,
                              20, 30, 10,  0,  0, 10, 30, 20};

int piece_square_difference(U64 white_pieces, U64 black_pieces,
                      const int *piece_squares) {
    int origin = -1, shift = 0;
    int score = 0;
    while (white_pieces) {
        shift = LSB(white_pieces) + 1;
        if (shift < 64) white_pieces >>= shift;
        else white_pieces = 0;
        origin += shift;
        score += piece_squares[origin];
    }
    origin = -1;
    shift = 0;
    while (black_pieces) {
        shift = LSB(black_pieces) + 1;
        if (shift < 64) black_pieces >>= shift;
        else black_pieces = 0;
        origin += shift;
        score -= piece_squares[vflip_square(origin)];
    }
    return score;
}

int all_piece_square_scores (Board *pBoard) {
    int score = 0;
    score += piece_square_difference(pBoard->position.pieceBB[WHITE_PAWN],
                                     pBoard->position.pieceBB[BLACK_PAWN],
                                     pawn_squares);
    score += piece_square_difference(pBoard->position.pieceBB[WHITE_KNIGHT],
                                     pBoard->position.pieceBB[BLACK_KNIGHT],
                                     knight_squares);
    score += piece_square_difference(pBoard->position.pieceBB[WHITE_BISHOP],
                                     pBoard->position.pieceBB[BLACK_BISHOP],
                                     bishop_squares);
    score += piece_square_difference(pBoard->position.pieceBB[WHITE_ROOK],
                                     pBoard->position.pieceBB[BLACK_ROOK],
                                     rook_squares);
    score += piece_square_difference(pBoard->position.pieceBB[WHITE_QUEEN],
                                     pBoard->position.pieceBB[BLACK_QUEEN],
                                     queen_squares);
    //score += piece_square_difference(pBoard->position.pieceBB[WHITE_KING],
    //                                     pBoard->position.pieceBB[BLACK_KING],
    //king_squares);
    return score;
}

// we could generalize to x files, and should
BitBoard passedPawn[2][64];
BitBoard outsidePassedPawn[2][64];

BitBoard kingSafetyZone[2][64];
BitBoard pawnShield[2][64];


void initEval() {
    for(int rank = 0; rank < 8; rank++) {
        for(int file = 0; file < 8; file++) {
            int square = SQ(rank+1, file+1);

            BitBoard passedMask = 0;
            BitBoard outsideMask = 0;
            for(int fileIndex = 0; fileIndex < 8; fileIndex++) {
                if(abs(fileIndex - file) < 2)
                    passedMask |= fileBB[fileIndex];
                if(abs(fileIndex - file) < 3)
                    outsideMask |= fileBB[fileIndex];
            }

            if(square < A8)
                passedPawn[WHITE][square] = passedMask & ~(BITSET(square + 2) - 1);
            else
                passedPawn[WHITE][square] = 0;

            if(square >= A2)
                passedPawn[BLACK][square] = passedMask & (BITSET(square - 6) - 1);
            else
                passedPawn[BLACK][square] = 0;


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



int doubledPawns(Board *pBoard) {
    int doubledPawns = 0;
    for(int currentFile = 0; currentFile < 8; currentFile++) {
        if(countBits(pBoard->position.pieceBB[WHITE_PAWN] & fileBB[currentFile]) == 2) doubledPawns++;
        if(countBits(pBoard->position.pieceBB[BLACK_PAWN] & fileBB[currentFile]) == 2) doubledPawns--;
    }
    return doubledPawns;
}

int tripledPawns(Board *pBoard) {
    int tripledPawns = 0;
    for(int currentFile = 0; currentFile < 8; currentFile++) {
        if(countBits(pBoard->position.pieceBB[WHITE_PAWN] & fileBB[currentFile]) == 3) tripledPawns++;
        if(countBits(pBoard->position.pieceBB[BLACK_PAWN] & fileBB[currentFile]) == 3) tripledPawns--;
    }
    return tripledPawns;
}

int passedPawns(Board *pBoard) {
    int passedPawnCount = 0;
    // not as fast as an optimized bitboard method, but this is all still proof of concept
    for(int square = A1; square <= H8; square++) {
        int piece = pBoard->position.square[square];
        if(piece == WHITE_PAWN) {
            if(!(passedPawn[WHITE][square] & pBoard->position.pieceBB[BLACK_PAWN]))
                passedPawnCount++;
        } else if(piece == BLACK_PAWN) {
            if(!(passedPawn[BLACK][square] & pBoard->position.pieceBB[WHITE_PAWN]))
                passedPawnCount--;
        }
    }
    return passedPawnCount;
}

int outsidePassedPawns(Board *pBoard) {
    int outsidePawnCount = 0;
    // not as fast as an optimized bitboard method, but this is all still proof of concept
    for(int square = A1; square <= H8; square++) {
        int piece = pBoard->position.square[square];
        if(piece == WHITE_PAWN) {
            if(!(outsidePassedPawn[WHITE][square] & pBoard->position.pieceBB[BLACK_PAWN]))
                outsidePawnCount++;
        } else if(piece == BLACK_PAWN) {
            if(!(outsidePassedPawn[BLACK][square] & pBoard->position.pieceBB[WHITE_PAWN]))
                outsidePawnCount--;
        }
    }
    return outsidePawnCount;
}

int bishopMobility(Board *pBoard);

int rookMobility(Board *pBoard);

int kingSafety(Board *pBoard);

int controlOfCenter(Board *pBoard);

int control(__attribute__((unused)) Board *pBoard,
            __attribute__((unused)) int square) {
    // we count the number of white pieces attacking the square, and subtract the number of black pieces.
    assert(false);
    return 0;
}
