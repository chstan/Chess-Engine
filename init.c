#include "defines.h"
#include "protos.h"
#include "extglobals.h"
#include "board.h"
#include <stdlib.h>

void init(void) {
	dataInit();
	boardInit();
	return;
}

void dataInit(void) {
	int i, rank, file;
	
	BITSET[0] = 0x1;
	for(i = 1; i < 64; i++) {
		BITSET[i] = BITSET[i-1] << 1;
	}
	
	for(rank = 0; rank < 9; rank++) {
		for(file = 0; file < 9; file++) {
			BOARDINDEX[file][rank] = sq(rank, file);
		}
	}
	
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

void boardInit(void) {
	pBoard = malloc(sizeof(Board));
	resetBoard(pBoard);
	return;
}