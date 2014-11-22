#ifndef MOVEGEN_H
#define MOVEGEN_H


#include "../defines.h"
#include "../extglobals.h"
#include "../board/board.h"

typedef struct {
    Move move_list[MAX_MOVES_PER_PLY];
    Move killer_list[MAX_KILLERS_PER_PLY];
    int timid_index;
    int total_moves;
    int move_iter;
    int current_move_index;

    int total_killers;
    int current_killer_index;
    int killer_iter;
} MoveSet;

bool moveset_contains(MoveSet *moves, Move m);


// void generate_check(Board *p_board);

// U64 attacks(Board *p_board, int attackee_index);
// U64 checks(Board *p_board, int king_index);

// void extractMoves(Board *p_board);


void generate_agnostic(Board *p_board, int color, U64 current_pieces,
    int piece, MoveSet *p_moves, U64 (*move_gen)(Board *p_board, UCHAR origin, int color));

void generate_timid(Board *p_board, MoveSet *p_moves);

void generate_capture(Board *p_board, MoveSet *p_moves);

void generate_move(Board *p_board, MoveSet *p_moves);

void initialize_move_set(Board *p_board, MoveSet *p_moves);

void initialize_move_set_quiet(Board *p_board, MoveSet *p_moves);

/**
 *\brief reset_move_set
 *
 *  reset_move_set is an auxilliary function
 *  which effectively clears the state of the
 *  moveSet passed in as the parameter.
 *
 *\param  Pointer to the moveset
 *\return (void)
**/
void reset_move_set(MoveSet *p_moves);

/**
 *\brief next_move
 *
 *  next_move returns the next unread move from the
 *  moveSet and updates the iterator to point to the
 *  next spot in the array. It also asserts that you are not
 *  trying to read an unwritten move from the moveSet.
 *
 *\param  Pointer to the moveSet
 *\return Move that was read from the moveSet
**/
Move next_move(MoveSet *p_moves);

/**
 *\brief write_move
 *
 *  write_move adds a non-killer move to a moveSet
 *  and leaves the moveSet in a consistent state, updating
 *  iterators as necessary.
 *
 *\param  Pointer to the moveSet
 *\param  Move to be written
 *\return (void)
**/
void write_move(MoveSet *p_moves, Move m);

/**
 *\brief write_killer
 *
 *  write_killer adds a killer move to a moveSet
 *  and leaves the moveSet in a consistent state, updating
 *  iterators as necessary.
 *
 *\param  Pointer to the moveSet
 *\param  Killer move to be written
 *\return (void)
**/
void write_killer(MoveSet *p_moves, Move killer);

U64 generate_all_attacks(Board *p_board, int color);

//===============UNFINISHED===================

void generate_check(Board *p_board, MoveSet *p_moves);

U64 attacks(Board *p_board, int attackee_index, int side);

bool checks(Board *p_board, int side);


#endif
