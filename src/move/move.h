#ifndef MOVE_H
#define MOVE_H

#include "../binary.h"
#include "../board/board.h"

typedef unsigned int Move;

// H G F E D C B A
// #SPECIAL SAUCE# #PROMO# #CAPTD#
// --------------- ------- -------
// 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

// #MOVED# #FROM BITS# # TO BITS #
// ------- ----------- -----------
// 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 <= Lowest order bit

// SPECIAL SAUCE:
/* A:   WHITE CASTLING
 * B:   BLACK CASTLING
 * C:   WHITE ENPASSANT
 * D:   BLACK ENPASSANT
 * E-H: UNUSED SAUCE
 */

//---------------MASKS----------------
//                            ----PPCC PPPPCCCC MMMMFFFF FFTTTTTT
#define tM      B32(11111111,11111111,11111111,11000000)
#define fM      B32(11111111,11111111,11110000,00111111)
#define mM      B32(11111111,11111111,00001111,11111111)
#define cM      B32(11111111,11110000,11111111,11111111)
#define pM      B32(11111111,00001111,11111111,11111111)
#define wC      B32(00000001,00000000,00000000,00000000)
#define bC      B32(00000010,00000000,00000000,00000000)
#define wP      B32(00000100,00000000,00000000,00000000)
#define bP      B32(00001000,00000000,00000000,00000000)

//------------INITIALIZER-------------
#define moveF(bP, wP, bC, wC, promo, capt, mov, fr, to) (((((((((((((((((bP) << 1) | (wP)) << 1) | (bC)) << 1) | (wC)) << 4) | (promo)) << 4) | (capt)) << 4) | (mov)) << 6) | (fr)) << 6) | (to))
#define move(capt, mov, fr, to) (((((((capt) << 4) | (mov)) << 6) | (fr)) << 6) | (to))

//--------------GETTERS---------------
#define to(move)                                            ((move) & 63)
#define from(move)                                      (((move) >> 6) & 63)
#define movedPiece(move)                            (((move) >> 12) & 15)
#define capturedPiece(move)                     (((move) >> 16) & 15)
#define promote(move)                               (((move) >> 20) & 15)
#define whiteCastle(move)                       (((move) >> 24) & 1)
#define blackCastle(move)                       (((move) >> 25) & 1)
#define whiteEnPassant(move)                    (((move) >> 26) & 1)
#define blackEnPassant(move)                    (((move) >> 27) & 1)

//--------------SETTERS---------------
#define setTo(move, to)                             ((move) &= (((tM) | (to)))
#define setFrom(move, from)                     ((move) &= (((fM) | ((to) << 6)))
#define setMovedPiece(move, piece)      ((move) &= (((mM) | ((piece) << 12)))
#define setCapturedPiece(move, piece) ((move) &= (((cM) | ((piece) << 16)))
#define setPromote(move, promo)             ((move) &= (((pM) | ((promo) << 20)))
#define setWhiteCastle(move)                    ((move) |= (wC))
#define unsetWhiteCastle(move)              ((move) &= ~(wC))
#define setBlackCastle(move)                    ((move) |= (bC))
#define unsetBlackCastle(move)              ((move) &= ~(bC))
#define setWhiteEnPassant(move)             ((move) |= (wP))
#define unsetWhiteEnPassant(move)       ((move) &= ~(wP))
#define setBlackEnPassant(move)             ((move) |= (bP))
#define unsetBlackEnPassant(move)       ((move) &= ~(bP))

void makeMove(Board *pBoard, Move m);

void unmakeMove(Board *pBoard, Move m);

void unmakeLastMove(Board *pBoard);

void printMove(Move m);

void advanceState(Board *pBoard, Move m);

void rewindState(Board *pBoard, Move m);

/** Function: compMove
 *  ------------------
 * compMove is a comparison operator for moves, useful for when moves have to be
 * sorted according to *approximately* how good they might be at first glance.
 * Subject to MUCH change.
 **/
int compMove (const void* p_move_a, const void* p_move_b);

#endif
