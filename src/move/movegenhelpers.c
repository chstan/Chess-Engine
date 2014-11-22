#include "movegenhelpers.h"
#include "../defines.h"
#include "../extglobals.h"

#define FILE_0_MASK 0xFEFEFEFEFEFEFEFEUL
#define FILE_7_MASK 0x7F7F7F7F7F7F7F7FUL

#define UP(bb)   ((bb) << 8)
#define DOWN(bb) ((bb) >> 8)
#define LEFT(bb)       (((bb) & FILE_7_MASK) << 1)
#define RIGHT(bb)      (((bb) & FILE_0_MASK) >> 1)
#define UP_LEFT(bb)    (((bb) & FILE_7_MASK) << 9)
#define UP_RIGHT(bb)   (((bb) & FILE_0_MASK) << 7)
#define DOWN_LEFT(bb)  (((bb) & FILE_7_MASK) >> 7)
#define DOWN_RIGHT(bb) (((bb) & FILE_0_MASK) >> 9)

//----------------ROOK-----------------

U64 rook_timid_BB(Board *p_board, UCHAR rook_index,
                     __attribute__((unused)) int side) {
    U64 validMoves = 0;
    // UP
    U64 currentIndex = UP(BITSET(rook_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = UP(currentIndex);
    }
    // DOWN
    currentIndex = DOWN(BITSET(rook_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = DOWN(currentIndex);
    }
    // RIGHT
    currentIndex = RIGHT(BITSET(rook_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = RIGHT(currentIndex);
    }
    // LEFT
    currentIndex = LEFT(BITSET(rook_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = LEFT(currentIndex);
    }
    return validMoves;
}

U64 rook_capture_BB(Board *p_board, UCHAR rook_index, int side) {
    U64 validMoves = 0;
    U64 takable = (side == BLACK)? p_board->position.white_occupied : p_board->position.black_occupied;
    // UP
    U64 currentIndex = UP(BITSET(rook_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        currentIndex = UP(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    // DOWN
    currentIndex = DOWN(BITSET(rook_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        currentIndex = DOWN(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    // RIGHT
    currentIndex = RIGHT(BITSET(rook_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        currentIndex = RIGHT(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    // LEFT
    currentIndex = LEFT(BITSET(rook_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        currentIndex = LEFT(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    return validMoves;
}

U64 rook_move_BB(Board *p_board, UCHAR rook_index, int side) {
    return rook_timid_BB(p_board, rook_index, side)
             | rook_capture_BB(p_board, rook_index, side);
}

//---------------BISHOP----------------

U64 bishop_timid_BB(Board *p_board, UCHAR bishop_index,
                       __attribute__((unused)) int side) {
    U64 validMoves = 0;
    // UL
    U64 currentIndex = UP_LEFT(BITSET(bishop_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = UP_LEFT(currentIndex);
    }
    // UR
    currentIndex = UP_RIGHT(BITSET(bishop_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = UP_RIGHT(currentIndex);
    }
    // DL
    currentIndex = DOWN_LEFT(BITSET(bishop_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = DOWN_LEFT(currentIndex);
    }
    // DR
    currentIndex = DOWN_RIGHT(BITSET(bishop_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = DOWN_RIGHT(currentIndex);
    }
    return validMoves;
}

U64 bishop_capture_BB(Board *p_board, UCHAR bishop_index, int side) {
    U64 validMoves = 0;
    U64 takable = (side == BLACK)? p_board->position.white_occupied : p_board->position.black_occupied;
    // UL
    U64 currentIndex = UP_LEFT(BITSET(bishop_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        currentIndex = UP_LEFT(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    // UR
    currentIndex = UP_RIGHT(BITSET(bishop_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        currentIndex = UP_RIGHT(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    // DL
    currentIndex = DOWN_LEFT(BITSET(bishop_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        currentIndex = DOWN_LEFT(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    // DR
    currentIndex = DOWN_RIGHT(BITSET(bishop_index));
    while(currentIndex & ~(p_board->position.occupied)) {
        currentIndex = DOWN_RIGHT(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    return validMoves;
}

U64 bishop_move_BB(Board *p_board, UCHAR bishop_index, int side) {
    return bishop_timid_BB(p_board, bishop_index, side)
             | bishop_capture_BB(p_board, bishop_index, side);
}

//----------------QUEEN----------------
U64 queen_timid_BB(Board *p_board, UCHAR queen_index, int side) {
    return bishop_timid_BB(p_board, queen_index, side)
             | rook_timid_BB(p_board, queen_index, side);
}

U64 queen_capture_BB(Board *p_board, UCHAR queen_index, int side) {
    return bishop_capture_BB(p_board, queen_index, side)
             | rook_capture_BB(p_board, queen_index, side);
}

U64 queen_move_BB(Board *p_board, UCHAR queen_index, int side) {
    return bishop_move_BB(p_board, queen_index, side)
             | rook_move_BB(p_board, queen_index, side);
}

//---------------KNIGHT----------------
U64 knight_timid_BB(Board *p_board, UCHAR knight_index,
                       __attribute__((unused)) int side) {
    return knightMove[knight_index] & ~p_board->position.occupied;
}

U64 knight_capture_BB(Board *p_board, UCHAR knight_index, int side) {
    switch(side) {
        case W:
            return knightMove[knight_index] & p_board->position.black_occupied;
        break;
        case B:
            return knightMove[knight_index] & p_board->position.white_occupied;
        break;
    }
    return 0;
}

U64 knight_move_BB(Board *p_board, UCHAR knight_index, int side) {
    switch(side) {
        case W:
            return knightMove[knight_index] & ~p_board->position.white_occupied;
        break;
        case B:
            return knightMove[knight_index] & ~p_board->position.black_occupied;
        break;
    }
    return 0;
}

//----------------KING-----------------
U64 king_timid_BB(Board *p_board, UCHAR king_index,
                     __attribute__((unused)) int side) {
    return kingMove[king_index] & ~p_board->position.occupied;
}

U64 king_capture_BB(Board *p_board, UCHAR king_index, int side) {
    switch(side) {
        case W:
            return kingMove[king_index] & p_board->position.black_occupied;
        break;
        case B:
            return kingMove[king_index] & p_board->position.white_occupied;
        break;
    }
    return 0;
}

U64 king_move_BB(Board *p_board, UCHAR king_index, int side) {
    switch(side) {
        case W:
            return kingMove[king_index] & ~p_board->position.white_occupied;
        break;
        case B:
            return kingMove[king_index] & ~p_board->position.black_occupied;
        break;
    }
    return 0;
}

//----------------PAWN-----------------
U64 en_passant_BB(Board *p_board, int side) {
    if(p_board->info.state[p_board->info.current_move].en_passant_square == INVALID_SQUARE) return 0;

    switch(side) {
        case W:
            return pawnCaptureBlack[p_board->info.state[p_board->info.current_move].en_passant_square] & p_board->position.piece_BB[WHITE_PAWN];
        break;
        case B:
            return pawnCaptureWhite[p_board->info.state[p_board->info.current_move].en_passant_square] & p_board->position.piece_BB[BLACK_PAWN];
        break;
    }
    return 0;
}

U64 pawn_timid_BB(Board *p_board, UCHAR pawn_index, int side) {
    U64 oneStep;
    switch(side) {
        case W:
            oneStep = pawnMoveWhite[pawn_index] & ~p_board->position.occupied;
            if(oneStep) {
                return oneStep | (pawnDoubleWhite[pawn_index] & ~p_board->position.occupied);
            }
            return 0;
        break;
        case B:
            oneStep = pawnMoveBlack[pawn_index] & ~p_board->position.occupied;
            if(oneStep) {
                return oneStep | (pawnDoubleBlack[pawn_index] & ~p_board->position.occupied);
            }
            return 0;
        break;
    }
    return 0;
}

U64 pawn_promotion_BB(Board *p_board, UCHAR pawn_index, int side) {
    switch(side) {
        case W:
            return pawnPromotionWhite[pawn_index] & ~p_board->position.occupied;
        break;
        case B:
            return pawnPromotionBlack[pawn_index] & ~p_board->position.occupied;
        break;
    }
    return 0;
}

U64 pawn_capture_BB(Board *p_board, UCHAR pawn_index, int side) {
    switch(side) {
        case W:
            return pawnCaptureWhite[pawn_index] & p_board->position.black_occupied;
        break;
        case B:
            return pawnCaptureBlack[pawn_index] & p_board->position.white_occupied;
        break;
    }
    return 0;
}

U64 pawn_promotion_capture_BB(Board *p_board, UCHAR pawn_index, int side) {
    switch(side) {
        case W:
            return pawnPromotionCaptureWhite[pawn_index] & p_board->position.black_occupied;
        break;
        case B:
            return pawnPromotionCaptureBlack[pawn_index] & p_board->position.white_occupied;
        break;
    }
    return 0;
}

U64 pawn_move_BB(Board *p_board, UCHAR pawn_index, int side) {
    return pawn_capture_BB(p_board, pawn_index, side) | pawn_timid_BB(p_board, pawn_index, side);
}
