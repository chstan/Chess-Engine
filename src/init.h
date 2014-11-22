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
 * U64s used later by the move generators
 * and to allocate space for the board
 * and a few other data structures.
 */
void init();

/* Function: move_table_init
 * -----------------------
 * Initializes the U64s used in the move tables.
 */
void move_table_init();

/* Function: data_init
 * ------------------
 * data_init sets up some global data used
 * in bit functions and sporadically throughout the
 * rest of the code.
 */
void data_init();

/* Function: board_init
 * -------------------
 * allocates memory for the board
 * and proceeds to call functions in board.c
 * to reset it to a coherent start state.
 */
void board_init();

#endif
