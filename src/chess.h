#ifndef CHESS_H
#define CHESS_H

typedef int MOVE;

typedef enum {
	black = 0, white = 1
} COLOR;

typedef enum {
	empty = 0, pawn = 1, knight = 2, bishop = 3, rook = 4, queen = 5, king = 6
} PIECE;

typedef enum {
	empty_value = 0, pawn_value = 1, knight_value = 3, bishop_value = 3,
	rook_value = 5, queen_value = 9, king_value = 100
} PIECE_VALUE;

typedef enum {
	FILEA, FILEB, FILEC, FILED, FILEE, FILEF, FILEG, FILEH
} files;

typedef enum {
	RANK1, RANK2, RANK3, RANK4, RANK5, RANK6, RANK7, RANK8
} ranks;

typedef struct {
	MOVE move1;
	MOVE move2;
} KILLER;

#endif

