#include <stdio.h>
#include <stdlib.h>

#include "fen.h"

void loadFromFEN(Board *pBoard, char *fen) {
	char boardString[73];
	char turnChar;
	char castleString[5];
	char enPassantString[3];
	int halfmoveClock; // since pawn advance or capture
	int fullmoveClock;
	if(sscanf(fen, "%72s %c %4s %2s %d %d", boardString, &turnChar, castleString, enPassantString, &halfmoveClock, &fullmoveClock) != 6) {
		printf("sscanf failed to read from:\n%s\n", fen);
		abort();
	}
	// unfinished
}

