#ifndef MOVEGENHELPERS_H
#define MOVEGENHELPERS_H

#include "../defines.h"
#include "../board/board.h"

//----------------ROOK-----------------

/* Function: rookTimidBB
 * Finds the valid moves for a rook at rookIndex which
 * does not capture any piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
BitBoard rookTimidBB(Board *pBoard, UCHAR rookIndex, int side);

/* Function: rookCaptureBB
 * Finds the valid moves for a rook at rookIndex which
 * do capture an enemy piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
BitBoard rookCaptureBB(Board *pBoard, UCHAR rookIndex, int side);

/* Function: rookMoveBB
 * Finds the valid moves for a rook at rookIndex which can
 * but does not have to capture a piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
BitBoard rookMoveBB(Board *pBoard, UCHAR rookIndex, int side);

//---------------BISHOP----------------

/* Function: bishopTimidBB
 * Finds the valid moves for a bishop at bishopIndex which
 * does not capture any piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
BitBoard bishopTimidBB(Board *pBoard, UCHAR bishopIndex, int side);

/* Function: bishopCaptureBB
 * Finds the valid moves for a bishop at bishopIndex which
 * do capture an enemy piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
BitBoard bishopCaptureBB(Board *pBoard, UCHAR bishopIndex, int side);

/* Function: bishopMoveBB
 * Finds the valid moves for a bishop at bishopIndex which can
 * but does not have to capture a piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
BitBoard bishopMoveBB(Board *pBoard, UCHAR bishopIndex, int side);

//----------------QUEEN----------------
BitBoard queenTimidBB(Board *pBoard, UCHAR queenIndex, int side);

BitBoard queenCaptureBB(Board *pBoard, UCHAR queenIndex, int side);

BitBoard queenMoveBB(Board *pBoard, UCHAR queenIndex, int side);

//---------------KNIGHT----------------
BitBoard knightTimidBB(Board *pBoard, UCHAR knightIndex, int side);

BitBoard knightCaptureBB(Board *pBoard, UCHAR knightIndex, int side);

BitBoard knightMoveBB(Board *pBoard, UCHAR knightIndex, int side);

//----------------KING-----------------
BitBoard kingTimidBB(Board *pBoard, UCHAR kingIndex, int side);

BitBoard kingCaptureBB(Board *pBoard, UCHAR kingIndex, int side);

BitBoard kingMoveBB(Board *pBoard, UCHAR kingIndex, int side);

//----------------PAWN-----------------
BitBoard pawnTimidBB(Board *pBoard, UCHAR pawnIndex, int side);

BitBoard pawnPromotionBB(Board *pBoard, UCHAR pawnIndex, int side);

BitBoard pawnCaptureBB(Board *pBoard, UCHAR pawnIndex, int side);

BitBoard pawnPromotionCaptureBB(Board *pBoard, UCHAR pawnIndex, int side);

BitBoard pawnMoveBB(Board *pBoard, UCHAR pawnIndex, int side);


#endif
