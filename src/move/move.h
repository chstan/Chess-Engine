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
#define hB      B32(10000000,00000000,00000000,00000000)

//------------INITIALIZER-------------
#define move_full(bP, wP, bC, wC, promo, capt, mov, fr, to) (((((((((((((((((bP) << 1) | (wP)) << 1) | (bC)) << 1) | (wC)) << 4) | (promo)) << 4) | (capt)) << 4) | (mov)) << 6) | (fr)) << 6) | (to))
#define move(capt, mov, fr, to) (((((((capt) << 4) | (mov)) << 6) | (fr)) << 6) | (to))

//--------------GETTERS---------------
#define to(move)                                            ((move) & 63)
#define from(move)                                      (((move) >> 6) & 63)
#define moved_piece(move)                            (((move) >> 12) & 15)
#define captured_piece(move)                     (((move) >> 16) & 15)
#define promote(move)                               (((move) >> 20) & 15)
#define white_castle(move)                       (((move) >> 24) & 1)
#define black_castle(move)                       (((move) >> 25) & 1)
#define white_en_passant(move)                    (((move) >> 26) & 1)
#define black_en_passant(move)                    (((move) >> 27) & 1)
#define hash_move(move)                          ((move) & (hB))
#define zero_hash(move)                          ((move) & (~hB))

//--------------SETTERS---------------
#define set_to(move, to)                             ((move) &= (((tM) | (to)))
#define set_from(move, from)                     ((move) &= (((fM) | ((to) << 6)))
#define set_moved_piece(move, piece)      ((move) &= (((mM) | ((piece) << 12)))
#define set_captured_piece(move, piece) ((move) &= (((cM) | ((piece) << 16)))
#define set_promote(move, promo)             ((move) &= (((pM) | ((promo) << 20)))
#define set_white_castle(move)                    ((move) |= (wC))
#define unset_white_castle(move)              ((move) &= ~(wC))
#define set_black_castle(move)                    ((move) |= (bC))
#define unset_black_castle(move)              ((move) &= ~(bC))
#define set_white_en_passant(move)             ((move) |= (wP))
#define unset_white_en_passant(move)       ((move) &= ~(wP))
#define set_black_en_passant(move)             ((move) |= (bP))
#define unset_black_en_passant(move)       ((move) &= ~(bP))
#define set_hash_bit(move)                ((move) |= hB)
#define unset_hash_bit(move)              ((move) &= ~(hB))

void make_null_move(Board *p_board);

void unmake_null_move(Board *p_board);

void make_move(Board *p_board, Move m);

void unmake_move(Board *p_board, Move m);

void unmake_last_move(Board *p_board);

void print_move(Move m);

void advance_state(Board *p_board, Move m);

void rewind_state(Board *p_board);

/** Function: comp_move
 *  ------------------
 * comp_move is a comparison operator for moves, useful for when moves have to be
 * sorted according to *approximately* how good they might be at first glance.
 * Subject to MUCH change.
 **/
int comp_move (const void* p_move_a, const void* p_move_b);

#endif
