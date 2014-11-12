#ifndef EVAL_H
#define EVAL_H
/* eval.h
 *
 * eval.h contains headers for board evaluation functions used by the chess AI.
 *
 */

#include "../board/board.h"

extern const int EVAL_INFTY;
extern const int EVAL_MATE;

int evaluate(Board *pBoard);

int evaluateNaive(Board *pBoard);

#endif
