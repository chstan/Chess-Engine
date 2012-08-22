#include "search.h"
#include "../move/movegen.h"
#include "../eval/eval.h"
#include "../board/board.h"
#include "defines.h"
#include "../extglobals.h"

// kind of a hack, would be better if we had
// proper data structures, vectors or stacks would be nice
// C++!
int bestMoveIndices[MAX_PLY];
Move bestMoves[MAX_PLY][MAX_PLY];

Move think() {
	// a really stupid search for the moment
	// should manage its time and use an iterative deepening method
	int searchDepth = 5;
	
	int score = alphaBeta(0, searchDepth, -INFTY, INFTY);
	return bestMoves[0][0];
}

int alphaBeta(int ply, int depth, int alpha, int beta) {
	// we'll use a negascout variation, without quiescence search, pvs, futility pruning, razoring
	// or any of the extras really, at least for now
	// will be one hell of an unstable search. :P
	
	// oh, and we'll need a transposition table and move sort too
	
	bestMoveIndices[ply] = ply;
		// should do quiescence search!
	if(depth == 0) return evaluate(pBoard);
	
	int value;
	
	MoveSet moves = initializeMoveSetQuiet(pBoard, &moves);
	for(int i = 0; i < moves.totalMoves; i++) {
		makeMove(pBoard, moves.moveList[i]);
		value = -alphaBeta(ply+1, depth-1, -beta, -alpha);
		unmakeMove(pBoard, moves.moveList[i]);
		
		if(value >= beta)
			return beta;
		
		if(value > alpha) {
			alpha = value;
			bestMoves[ply][ply] = moves.moveList[i];
			
			for(int j = ply+1; j < bestMoveIndices[ply+1]; j++)
				bestMoves[ply][j] = bestMoves[ply+1][j];
			
			bestMoveIndices[ply] = bestMoveIndices[ply+1];
		}
	}
	return alpha;
}