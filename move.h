#ifndef MOVE_H
#define MOVE_H

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
//							  ----PPCCPPPPCCCCMMMMFFFFFFTTTTTT
#define tM 			0b11111111111111111111111111000000
#define fM			0b11111111111111111111000000111111
#define mM			0b11111111111111110000111111111111
#define cM			0b11111111111100001111111111111111
#define pM			0b11111111000011111111111111111111
#define wC			0b00000001000000000000000000000000
#define bC			0b00000010000000000000000000000000
#define wP			0b00000100000000000000000000000000
#define bP			0b00001000000000000000000000000000

//--------------GETTERS---------------
#define to(move) 											(move & 63)
#define from(move) 										((move >> 6) & 63)
#define movedPiece(move) 							((move >> 12) & 15)
#define capturedPiece(move) 					((move >> 16) & 15)
#define promo(move) 									((move >> 20) & 15)
#define whiteCastle(move) 						((move >> 21) & 1)
#define blackCastle(move) 						((move >> 22) & 1)
#define whiteEnPassant(move) 					((move >> 23) & 1)
#define blackEnPassant(move) 					((move >> 24) & 1)

//--------------SETTERS---------------
#define setTo(move, to) 							(move &= (tM | to))
#define setFrom(move, from) 					(move &= (fM | (to << 6)))
#define setMovedPiece(move, piece) 		(move &= (mM | (piece << 12))) 
#define setCapturedPiece(move, piece) (move &= (cM | (piece << 16)))
#define setPromo(move, promo) 				(move &= (pM | (promo << 20)))
#define setWhiteCastle(move) 					(move |= wC)
#define unsetWhiteCastle(move) 				(move &= ~wC)
#define setBlackCastle(move) 					(move |= bC)
#define unsetBlackCastle(move) 				(move &= ~bC)
#define setWhiteEnPassant(move) 			(move |= wP)
#define unsetWhiteEnPassant(move) 		(move &= ~wP)
#define setBlackEnPassant(move) 			(move |= bP)
#define unsetBlackEnPassant(move) 		(move &= ~bP)

#endif