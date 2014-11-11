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

BitBoard rookTimidBB(Board *pBoard, UCHAR rookIndex,
                     __attribute__((unused)) int side) {
    BitBoard validMoves = 0;
    // UP
    BitBoard currentIndex = UP(BITSET(rookIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = UP(currentIndex);
    }
    // DOWN
    currentIndex = DOWN(BITSET(rookIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = DOWN(currentIndex);
    }
    // RIGHT
    currentIndex = RIGHT(BITSET(rookIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = RIGHT(currentIndex);
    }
    // LEFT
    currentIndex = LEFT(BITSET(rookIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = LEFT(currentIndex);
    }
    return validMoves;
}

BitBoard rookCaptureBB(Board *pBoard, UCHAR rookIndex, int side) {
    BitBoard validMoves = 0;
    BitBoard takable = (side == BLACK)? pBoard->position.whiteOccupied : pBoard->position.blackOccupied;
    // UP
    BitBoard currentIndex = UP(BITSET(rookIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        currentIndex = UP(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    // DOWN
    currentIndex = DOWN(BITSET(rookIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        currentIndex = DOWN(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    // RIGHT
    currentIndex = RIGHT(BITSET(rookIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        currentIndex = RIGHT(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    // LEFT
    currentIndex = LEFT(BITSET(rookIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        currentIndex = LEFT(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    return validMoves;
}

BitBoard rookMoveBB(Board *pBoard, UCHAR rookIndex, int side) {
    return rookTimidBB(pBoard, rookIndex, side)
             | rookCaptureBB(pBoard, rookIndex, side);
}

//---------------BISHOP----------------

BitBoard bishopTimidBB(Board *pBoard, UCHAR bishopIndex,
                       __attribute__((unused)) int side) {
    BitBoard validMoves = 0;
    // UL
    BitBoard currentIndex = UP_LEFT(BITSET(bishopIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = UP_LEFT(currentIndex);
    }
    // UR
    currentIndex = UP_RIGHT(BITSET(bishopIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = UP_RIGHT(currentIndex);
    }
    // DL
    currentIndex = DOWN_LEFT(BITSET(bishopIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = DOWN_LEFT(currentIndex);
    }
    // DR
    currentIndex = DOWN_RIGHT(BITSET(bishopIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        validMoves |= currentIndex;
        currentIndex = DOWN_RIGHT(currentIndex);
    }
    return validMoves;
}

BitBoard bishopCaptureBB(Board *pBoard, UCHAR bishopIndex, int side) {
    BitBoard validMoves = 0;
    BitBoard takable = (side == BLACK)? pBoard->position.whiteOccupied : pBoard->position.blackOccupied;
    // UL
    BitBoard currentIndex = UP_LEFT(BITSET(bishopIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        currentIndex = UP_LEFT(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    // UR
    currentIndex = UP_RIGHT(BITSET(bishopIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        currentIndex = UP_RIGHT(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    // DL
    currentIndex = DOWN_LEFT(BITSET(bishopIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        currentIndex = DOWN_LEFT(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    // DR
    currentIndex = DOWN_RIGHT(BITSET(bishopIndex));
    while(currentIndex & ~(pBoard->position.occupied)) {
        currentIndex = DOWN_RIGHT(currentIndex);
    }
    if(currentIndex & takable) {
        validMoves |= currentIndex;
    }
    return validMoves;
}

BitBoard bishopMoveBB(Board *pBoard, UCHAR bishopIndex, int side) {
    return bishopTimidBB(pBoard, bishopIndex, side)
             | bishopCaptureBB(pBoard, bishopIndex, side);
}

//----------------QUEEN----------------
BitBoard queenTimidBB(Board *pBoard, UCHAR queenIndex, int side) {
    return bishopTimidBB(pBoard, queenIndex, side)
             | rookTimidBB(pBoard, queenIndex, side);
}

BitBoard queenCaptureBB(Board *pBoard, UCHAR queenIndex, int side) {
    return bishopCaptureBB(pBoard, queenIndex, side)
             | rookCaptureBB(pBoard, queenIndex, side);
}

BitBoard queenMoveBB(Board *pBoard, UCHAR queenIndex, int side) {
    return bishopMoveBB(pBoard, queenIndex, side)
             | rookMoveBB(pBoard, queenIndex, side);
}

//---------------KNIGHT----------------
BitBoard knightTimidBB(Board *pBoard, UCHAR knightIndex,
                       __attribute__((unused)) int side) {
    return knightMove[knightIndex] & ~pBoard->position.occupied;
}

BitBoard knightCaptureBB(Board *pBoard, UCHAR knightIndex, int side) {
    switch(side) {
        case W:
            return knightMove[knightIndex] & pBoard->position.blackOccupied;
        break;
        case B:
            return knightMove[knightIndex] & pBoard->position.whiteOccupied;
        break;
    }
    return 0;
}

BitBoard knightMoveBB(Board *pBoard, UCHAR knightIndex, int side) {
    switch(side) {
        case W:
            return knightMove[knightIndex] & ~pBoard->position.whiteOccupied;
        break;
        case B:
            return knightMove[knightIndex] & ~pBoard->position.blackOccupied;
        break;
    }
    return 0;
}

//----------------KING-----------------
BitBoard kingTimidBB(Board *pBoard, UCHAR kingIndex,
                     __attribute__((unused)) int side) {
    return kingMove[kingIndex] & ~pBoard->position.occupied;
}

BitBoard kingCaptureBB(Board *pBoard, UCHAR kingIndex, int side) {
    switch(side) {
        case W:
            return kingMove[kingIndex] & pBoard->position.blackOccupied;
        break;
        case B:
            return kingMove[kingIndex] & pBoard->position.whiteOccupied;
        break;
    }
    return 0;
}

BitBoard kingMoveBB(Board *pBoard, UCHAR kingIndex, int side) {
    switch(side) {
        case W:
            return kingMove[kingIndex] & ~pBoard->position.whiteOccupied;
        break;
        case B:
            return kingMove[kingIndex] & ~pBoard->position.blackOccupied;
        break;
    }
    return 0;
}

//----------------PAWN-----------------
BitBoard enPassantBB(Board *pBoard, int side) {
    if(pBoard->info.state[pBoard->info.currentMove].enPassantSquare == INVALID_SQUARE) return 0;

    switch(side) {
        case W:
            return pawnCaptureBlack[pBoard->info.state[pBoard->info.currentMove].enPassantSquare] & pBoard->position.pieceBB[WHITE_PAWN];
        break;
        case B:
            return pawnCaptureWhite[pBoard->info.state[pBoard->info.currentMove].enPassantSquare] & pBoard->position.pieceBB[BLACK_PAWN];
        break;
    }
    return 0;
}

BitBoard pawnTimidBB(Board *pBoard, UCHAR pawnIndex, int side) {
    BitBoard oneStep;
    switch(side) {
        case W:
            oneStep = pawnMoveWhite[pawnIndex] & ~pBoard->position.occupied;
            if(oneStep) {
                return oneStep | (pawnDoubleWhite[pawnIndex] & ~pBoard->position.occupied);
            }
            return 0;
        break;
        case B:
            oneStep = pawnMoveBlack[pawnIndex] & ~pBoard->position.occupied;
            if(oneStep) {
                return oneStep | (pawnDoubleBlack[pawnIndex] & ~pBoard->position.occupied);
            }
            return 0;
        break;
    }
    return 0;
}

BitBoard pawnPromotionBB(Board *pBoard, UCHAR pawnIndex, int side) {
    switch(side) {
        case W:
            return pawnPromotionWhite[pawnIndex] & ~pBoard->position.occupied;
        break;
        case B:
            return pawnPromotionBlack[pawnIndex] & ~pBoard->position.occupied;
        break;
    }
    return 0;
}

BitBoard pawnCaptureBB(Board *pBoard, UCHAR pawnIndex, int side) {
    switch(side) {
        case W:
            return pawnCaptureWhite[pawnIndex] & pBoard->position.blackOccupied;
        break;
        case B:
            return pawnCaptureBlack[pawnIndex] & pBoard->position.whiteOccupied;
        break;
    }
    return 0;
}

BitBoard pawnPromotionCaptureBB(Board *pBoard, UCHAR pawnIndex, int side) {
    switch(side) {
        case W:
            return pawnPromotionCaptureWhite[pawnIndex] & pBoard->position.blackOccupied;
        break;
        case B:
            return pawnPromotionCaptureBlack[pawnIndex] & pBoard->position.whiteOccupied;
        break;
    }
    return 0;
}

BitBoard pawnMoveBB(Board *pBoard, UCHAR pawnIndex, int side) {
    return pawnCaptureBB(pBoard, pawnIndex, side) | pawnTimidBB(pBoard, pawnIndex, side);
}
