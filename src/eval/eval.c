#include "eval.h"
#include "evalhelpers.h"

float evaluate(Board *pBoard) {
	// for now we just use a naive evaluation, which counts material and does
	// somewhat primitive checks of mobility and board control
	return evaluateNaive(pBoard);
}

float evaluateNaive(Board *pBoard) {
	return (float) (pBoard->info.whiteMaterial - pBoard->info.blackMaterial)/pBoard->info.material;
}
