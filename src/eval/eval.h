#ifndef EVAL_H
#define EVAL_H
/* eval.h
 *
 * eval.h contains headers for board evaluation functions used by the chess AI.
 *
 */

#include "../board/board.h"

float evaluate(Board *pBoard);

float evaluateNaive(Board *pBoard);

#endif
