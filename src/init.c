#include <stdlib.h>
#include <stdio.h>

#include "init.h"
#include "manage_time.h"
#include "log.h"
#include "bit.h"
#include "defines.h"
#include "extglobals.h"
#include "board/board.h"
#include "move/move.h"
#include "aux/random.h"
#include "eval/evalhelpers.h"
#include "zobrist.h"

U64 safe_bitset (int idx) {
    if (idx < 0 || idx > 63) return 0;
    return BITSET(idx);
}

void pre_init() {
    init_logging();
}

void init() {
    data_init();
    move_table_init();

    initEval();

    init_random();

    initHashTable();

    board_init();

    init_manage_time();
    return;
}

void move_table_init() {
    int rank, file, index;
    int lRank, hRank, lFile, hFile;
    U64 bits;
    // i will be rank and j will be file
    for(rank = 1; rank <= 8; rank++) {
        for(file = 1; file <= 8; file++) {
            index = SQ(rank, file);

            //--------PAWN ONE SQUARE----------
            if(rank < 7) {
                pawnMoveWhite[index] = safe_bitset(index + 8);
            } else {
                pawnMoveWhite[index] = emptyBoard;
            }

            if(rank > 2) {
                pawnMoveBlack[index] = safe_bitset(index - 8);
            } else {
                pawnMoveBlack[index] = emptyBoard;
            }

            //--------PAWN TWO SQUARES---------
            if(rank == 2) {
                pawnDoubleWhite[index] = safe_bitset(index + 16);
            } else {
                pawnDoubleWhite[index] = emptyBoard;
            }
            if(rank == 7) {
                pawnDoubleBlack[index] = safe_bitset(index - 16);
            } else {
                pawnDoubleBlack[index] = emptyBoard;
            }

            //----------PAWN CAPTURES----------
            if(8 > file && file > 1) {
                if (rank <= 6) pawnCaptureWhite[index] = safe_bitset(index + 9) | safe_bitset(index + 7);
                if (rank >= 3) pawnCaptureBlack[index] = safe_bitset(index - 9) | safe_bitset(index - 7);
            } else {
                if(file == 1) {
                    pawnCaptureWhite[index] = emptyBoard;
                    pawnCaptureBlack[index] = emptyBoard;
                    if (rank <= 6) pawnCaptureWhite[index] = safe_bitset(index + 9);
                    if (rank >= 3) pawnCaptureBlack[index] = safe_bitset(index - 7);
                } else {
                    pawnCaptureWhite[index] = emptyBoard;
                    pawnCaptureBlack[index] = emptyBoard;
                    if (rank <= 6) pawnCaptureWhite[index] = safe_bitset(index + 7);
                    if (rank >= 3) pawnCaptureBlack[index] = safe_bitset(index - 9);
                }
            }

            //--------PAWN ALL CAPTURES--------
            if(8 > file && file > 1) {
                pawnAllCaptureWhite[index] = safe_bitset(index + 9) | safe_bitset(index + 7);
                pawnAllCaptureBlack[index] = safe_bitset(index - 9) | safe_bitset(index - 7);
            } else {
                if(file == 1) {
                    pawnAllCaptureWhite[index] = safe_bitset(index + 9);
                    pawnAllCaptureWhite[index] = safe_bitset(index - 7);
                } else {
                    pawnAllCaptureWhite[index] = safe_bitset(index + 7);
                    pawnAllCaptureWhite[index] = safe_bitset(index - 9);
                }
            }
            //-----PAWN CAPTURE PROMOTIONS-----
            if(8 > file && file > 1) {
                if (rank == 7) pawnPromotionCaptureWhite[index] = safe_bitset(index + 9) | safe_bitset(index + 7);
                if (rank == 2) pawnPromotionCaptureBlack[index] = safe_bitset(index - 9) | safe_bitset(index - 7);
            } else {
                if(file == 1) {
                    pawnPromotionCaptureWhite[index] = emptyBoard;
                    pawnPromotionCaptureBlack[index] = emptyBoard;
                    if (rank == 7) pawnPromotionCaptureWhite[index] = safe_bitset(index + 9);
                    if (rank == 2) pawnPromotionCaptureBlack[index] = safe_bitset(index - 7);
                } else {
                    pawnPromotionCaptureWhite[index] = emptyBoard;
                    pawnPromotionCaptureBlack[index] = emptyBoard;
                    if (rank == 7) pawnPromotionCaptureWhite[index] = safe_bitset(index + 7);
                    if (rank == 2) pawnPromotionCaptureBlack[index] = safe_bitset(index - 9);
                }
            }

            //---------PAWN PROMOTIONS---------
            if(rank == 7) {
                pawnPromotionWhite[index] = safe_bitset(index + 8);
            } else {
                pawnPromotionWhite[index] = emptyBoard;
            }
            if(rank == 2) {
                pawnPromotionBlack[index] = safe_bitset(index - 8);
            } else {
                pawnPromotionBlack[index] = emptyBoard;
            }

            //--------KING    MOVEMENTS--------
            lRank = (rank - 1 >= 1) ? rank - 1 : 1;
            hRank = (rank + 1 <= 8) ? rank + 1 : 8;
            lFile = (file - 1 >= 1) ? file - 1 : 1;
            hFile = (file + 1 <= 8) ? file + 1 : 8;
            bits = 0;
            int iF;
            int iR;
            for(iR = lRank; iR <= hRank; iR++) {
                for(iF = lFile; iF <= hFile; iF++) {
                    bits |= safe_bitset(SQ(iR, iF));
                }
            }
            kingMove[index] = bits & ~safe_bitset(index);

            //--------KNIGHT  MOVEMENTS--------
            bits = 0;
            if(rank + 2 <= 8 && file + 1 <= 8) {
                bits |= safe_bitset(SQ(rank+2, file+1));
            }
            if(rank + 2 <= 8 && file - 1 >= 1) {
                bits |= safe_bitset(SQ(rank+2, file-1));
            }
            if(rank + 1 <= 8 && file + 2 <= 8) {
                bits |= safe_bitset(SQ(rank+1, file+2));
            }
            if(rank + 1 <= 8 && file - 2 >= 1) {
                bits |= safe_bitset(SQ(rank+1, file-2));
            }
            if(rank - 1 >= 1 && file + 2 <= 8) {
                bits |= safe_bitset(SQ(rank-1, file+2));
            }
            if(rank - 1 >= 1 && file - 2 >= 1) {
                bits |= safe_bitset(SQ(rank-1, file-2));
            }
            if(rank - 2 >= 1 && file + 1 <= 8) {
                bits |= safe_bitset(SQ(rank-2, file+1));
            }
            if(rank - 2 >= 1 && file - 1 >= 1) {
                bits |= safe_bitset(SQ(rank-2, file-1));
            }
            knightMove[index] = bits;

            //--------DIAGON  MOVEMENTS--------
            for(lFile = file+1, lRank = rank+1, bits = 0; lFile <= 8 && lRank <= 8; lFile++, lRank++) {
                bits |= safe_bitset(SQ(lFile, lRank));
            }
            rightUpMove[index] = bits;

            for(lFile = file+1, lRank = rank-1, bits = 0; lFile <= 8 && lRank >= 1; lFile++, lRank--) {
                bits |= safe_bitset(SQ(lFile, lRank));
            }
            leftUpMove[index] = bits;

            for(lFile = file-1, lRank = rank+1, bits = 0; lFile >= 1 && lRank <= 8; lFile--, lRank++) {
                bits |= safe_bitset(SQ(lFile, lRank));
            }
            rightDownMove[index] = bits;

            for(lFile = file-1, lRank = rank-1, bits = 0; lFile >= 1 && lRank >= 1; lFile--, lRank--) {
                bits |= safe_bitset(SQ(lFile, lRank));
            }
            leftDownMove[index] = bits;

            //--------ORTHOG  MOVEMENTS--------
            for(lFile = file+1, bits = 0; lFile <= 8; lFile++) {
                bits |= safe_bitset(SQ(lFile, rank));
            }
            upMove[index] = bits;

            for(lFile = file-1, bits = 0; lFile >= 1; lFile--) {
                bits |= safe_bitset(SQ(lFile, rank));
            }
            downMove[index] = bits;

            for(lRank = rank+1, bits = 0; lRank <= 8; lRank++) {
                bits |= safe_bitset(SQ(file, lRank));
            }
            rightMove[index] = bits;

            for(lRank = rank-1, bits = 0; lRank >= 1; lRank--) {
                bits |= safe_bitset(SQ(file, lRank));
            }
            leftMove[index] = bits;
        }
    }
    return;
}

void data_init() {
    int i, rank, file;

    for(int i = 0; i < 8; i++) {
        rankBB[i] = 0;
        fileBB[i] = 0;
    }

    // Sets up a 2D array for looking up
    // SQuares on the board.
    for(rank = 0; rank < 8; rank++) {
        for(file = 0; file < 8; file++) {
            rankBB[rank] |= safe_bitset(SQ(rank+1, file+1));
            fileBB[file] |= safe_bitset(SQ(rank+1, file+1));
        }
    }

    // for all the numbers between 0 and 255
    // GS1B[i] has the index of the greatest significant
    // set bit in the binary representation of i
    for(i = 0; i < 256; i++) {
    GS1B[i] = (
        (i > 127) ? 7 :
        (i >  63) ? 6 :
        (i >  31) ? 5 :
        (i >  15) ? 4 :
        (i >   7) ? 3 :
        (i >   3) ? 2 :
        (i >   1) ? 1 : 0 );
    }
    return;
}

void board_init() {
    p_board = malloc(sizeof(Board));
    reset_board(p_board);
    return;
}
