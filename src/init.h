#ifndef INIT_H
#define INIT_H

#include "move/movegen.h"

/* Function: pre_init
 * ------------------
 * Initializes some things that should be available before
 * the GUI gives the isready command like logging capabilities.
 */
void pre_init();

/* Function: init
 * --------------
 * init is called by main to fill
 * BitBoards used later by the move generators
 * and to allocate space for the board
 * and a few other data structures.
 */
void init();

/* Function: moveTableInit
 * -----------------------
 * Initializes the BitBoards used in the move tables.
 */
void moveTableInit();

/* Function: dataInit
 * ------------------
 * dataInit sets up some global data used
 * in bit functions and sporadically throughout the
 * rest of the code.
 */
void dataInit();

/* Function: boardInit
 * -------------------
 * allocates memory for the board
 * and proceeds to call functions in board.c
 * to reset it to a coherent start state.
 */
void boardInit();

#endif
