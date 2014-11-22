#ifndef MOVEGENHELPERS_H
#define MOVEGENHELPERS_H

#include "../defines.h"
#include "../board/board.h"

//----------------ROOK-----------------

/* Function: rook_timid_BB
 * Finds the valid moves for a rook at rook_index which
 * does not capture any piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
U64 rook_timid_BB(Board *p_board, UCHAR rook_index, int side);

/* Function: rook_capture_BB
 * Finds the valid moves for a rook at rook_index which
 * do capture an enemy piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
U64 rook_capture_BB(Board *p_board, UCHAR rook_index, int side);

/* Function: rook_move_BB
 * Finds the valid moves for a rook at rook_index which can
 * but does not have to capture a piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
U64 rook_move_BB(Board *p_board, UCHAR rook_index, int side);

//---------------BISHOP----------------

/* Function: bishop_timid_BB
 * Finds the valid moves for a bishop at bishop_index which
 * does not capture any piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
U64 bishop_timid_BB(Board *p_board, UCHAR bishop_index, int side);

/* Function: bishop_capture_BB
 * Finds the valid moves for a bishop at bishop_index which
 * do capture an enemy piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
U64 bishop_capture_BB(Board *p_board, UCHAR bishop_index, int side);

/* Function: bishop_move_BB
 * Finds the valid moves for a bishop at bishop_index which can
 * but does not have to capture a piece. AS A NOTE, this current
 * incarnation uses loops to achieve the rook move generation
 * it can probably be done in O(1) with clever bit ops.
 */
U64 bishop_move_BB(Board *p_board, UCHAR bishop_index, int side);

//----------------QUEEN----------------
U64 queen_timid_BB(Board *p_board, UCHAR queen_index, int side);

U64 queen_capture_BB(Board *p_board, UCHAR queen_index, int side);

U64 queen_move_BB(Board *p_board, UCHAR queen_index, int side);

//---------------KNIGHT----------------
U64 knight_timid_BB(Board *p_board, UCHAR knight_index, int side);

U64 knight_capture_BB(Board *p_board, UCHAR knight_index, int side);

U64 knight_move_BB(Board *p_board, UCHAR knight_index, int side);

//----------------KING-----------------
U64 king_timid_BB(Board *p_board, UCHAR king_index, int side);

U64 king_capture_BB(Board *p_board, UCHAR king_index, int side);

U64 king_move_BB(Board *p_board, UCHAR king_index, int side);

//----------------PAWN-----------------
U64 en_passant_BB(Board *p_board, int side);

U64 pawn_timid_BB(Board *p_board, UCHAR pawn_index, int side);

U64 pawn_promotion_BB(Board *p_board, UCHAR pawn_index, int side);

U64 pawn_capture_BB(Board *p_board, UCHAR pawn_index, int side);

U64 pawn_promotion_capture_BB(Board *p_board, UCHAR pawn_index, int side);

U64 pawn_move_BB(Board *p_board, UCHAR pawn_index, int side);

typedef U64 (*MoveGenCB)(Board *p_board, UCHAR index, int side);

#endif
