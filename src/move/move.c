#include <stdio.h>
#include <assert.h>

#include "move.h"
#include "../board/board.h"
#include "../defines.h"
#include "../extglobals.h"

#ifdef DEBUG
Move last_unmade = 0;
Move second_last_unmade = 0;
#endif

// checks that the move is legal and coherent
bool coherent_move(Board *p_board, Move m) {
    assert(m && "Nonzero move.");
    if (color(moved_piece(m)) != p_board->info.to_play) {
        printf("COHERENTMOVE Color does not match\n");
        return false;
    }

    int mn = p_board->info.current_move;
    MoveInfo st = p_board->info.state[mn];
    if (p_board->position.square[from(m)] != moved_piece(m)) {
        printf("COHERENTMOVE Moved piece not on board at from.\n");
        return false;
    }
    if (p_board->position.square[to(m)] != captured_piece(m)
        && st.en_passant_square != to(m)) {
        printf("COHERENTMOVE Captured piece (or empty) not on board at to.\n");
        return false;
    }
    if (white_castle(m)) {
        if (p_board->info.to_play != WHITE) {
            printf("COHERENTMOVE Castling wrong color.\n");
            return false;
        }
        if (FILE(to(m)) == 2 /* C file */) {
            if(!(st.castle_white & CAN_CASTLE_OOO)) {
                printf("COHERENTMOVE Cannot castle queenside.\n");
                return false;
            }
        }
        if (FILE(to(m)) == 6 /* C file */) {
            if(!(st.castle_white & CAN_CASTLE_OO)) {
                printf("COHERENTMOVE Cannot castle kingside.\n");
                return false;
            }
        }
    }
    if (black_castle(m)) {
        if (p_board->info.to_play != BLACK) {
            printf("COHERENTMOVE Castling wrong color.\n");
            return false;
        }
        if (FILE(to(m)) == 2 /* C file */) {
            if(!(st.castle_black & CAN_CASTLE_OOO)) {
                printf("COHERENTMOVE Cannot castle queenside.\n");
                return false;
            }
        }
        if (FILE(to(m)) == 6 /* C file */) {
            if(!(st.castle_black & CAN_CASTLE_OO)) {
                printf("COHERENTMOVE Cannot castle kingside.\n");
                return false;
            }
        }
    }
    return true;
}

/* As a word of note:
 *   These functions assume that the move
 * is valid for the board and that the movement
 * obeys all piece laws. The rest is up to
 * the move generator itself. Furthermore
 * it assumes that the data contained in
 * m is coherent.
 */
void make_null_move(Board *p_board) {
    // ALWAYS PAIR WITH AN UNMAKE NULL MOVE
    int current_move = p_board->info.current_move;
    U64 old_z_key = p_board->info.state[current_move]._zobrist_key;

    //p_board->info.state[current_move+1].move = 0;
    p_board->info.state[current_move+1].stale_moves =
        p_board->info.state[current_move].stale_moves;

    // set en passant square
    p_board->info.state[current_move+1].en_passant_square = INVALID_SQUARE;
    // en passant Zobrist modification
    if (p_board->info.state[current_move].en_passant_square != INVALID_SQUARE) {
        old_z_key ^= ep_file_keys[FILE(p_board->info.state[current_move].en_passant_square)];
    }
    if (p_board->info.state[current_move+1].en_passant_square != INVALID_SQUARE) {
        old_z_key ^= ep_file_keys[FILE(p_board->info.state[current_move+1].en_passant_square)];
    }

    p_board->info.to_play ^= 1;
    old_z_key ^= white_to_move_key;

    p_board->info.state[current_move+1]._zobrist_key = old_z_key;
    p_board->info.current_move++;
#ifdef DEBUG
    assert(p_board->info.current_move < MAX_MOVES_PER_GAME + 1);
    if (old_z_key != full_zobrist_key(p_board)) {
        printf("Man you gotta fix this null move.\n");
        print_move(m);
        display_board(p_board);
        assert(0 && "BAD ZOBRIST");
    }
#endif
}

void unmake_null_move(Board *p_board) {
    rewind_state(p_board);
}

void make_move(Board *p_board, Move m) {
#ifdef DEBUG
    if (p_board->info.state[p_board->info.current_move]._zobrist_key
        != full_zobrist_key(p_board)) {
        print_move(p_board->info.state[p_board->info.current_move].move);
        display_board(p_board);
        assert(p_board->info.state[p_board->info.current_move]._zobrist_key
               == full_zobrist_key(p_board));
    }

    if (!coherent_move(p_board, m)) {
        display_board(p_board);
        print_move(m);
        print_move(last_unmade);
        print_move(second_last_unmade);
        assert(0 && "Incoherent.\n");
    }
#endif
    if(white_castle(m) | black_castle(m)) {
        // CASTLING
        if(white_castle(m)) {
            castle(p_board, to(m), WHITE);
        } else {
            castle(p_board, to(m), BLACK);
        }
    } else {
        set_empty_at(p_board, from(m), moved_piece(m));

        if(!(white_en_passant(m) | black_en_passant(m))) {
            set_piece_at(p_board, to(m), moved_piece(m), captured_piece(m));
        } else {
            // EN PASSANT
            set_piece_at(p_board, to(m), moved_piece(m), 0);
            en_passant(p_board, color(moved_piece(m)));
        }

        if(promote(m)) {
            // PROMOTION
            do_promote(p_board, to(m), promote(m));
            remove_material(p_board, moved_piece(m));
            add_material(p_board, promote(m));
        }
        if (captured_piece(m))
            remove_material(p_board, captured_piece(m));
    }
    advance_state(p_board, m);
#ifdef DEBUG
    if(!debug_board(p_board)) {
            print_move(m);
    }
#endif
}

void unmake_move(Board *p_board, Move m) {
#ifdef DEBUG
    assert(m == p_board->info.state[p_board->info.current_move].move);
    if (!debug_board(p_board)) printf("WTH?\n");
    second_last_unmade = last_unmade;
    last_unmade = m;
    assert(p_board->info.state[p_board->info.current_move]._zobrist_key
           == full_zobrist_key(p_board) && "Right at start of rewind.");
#endif
    bool patha = false;
    bool pathb = false;
    bool pathc = false;
    bool pathd = false;
    if(white_castle(m) | black_castle(m)) {
        // CASTLING
        if(white_castle(m)) {
            un_castle(p_board, to(m), WHITE);
        } else {
            un_castle(p_board, to(m), BLACK);
        }
    } else {
        if(promote(m)) {
            un_promote(p_board, to(m));
            remove_material(p_board, promote(m));
            add_material(p_board, moved_piece(m));
        }

        set_empty_at(p_board, to(m), moved_piece(m));
        if(!(white_en_passant(m) | black_en_passant(m))) {
            patha = true;
            if(captured_piece(m)) {
                pathb = true;
                set_piece_at(p_board, to(m), captured_piece(m), 0);
            }
            set_piece_at(p_board, from(m), moved_piece(m), 0);
        } else {
            pathc = true;
            set_piece_at(p_board, from(m), moved_piece(m), 0);
            un_passant(p_board, to(m), color(moved_piece(m)));
        }
        if (captured_piece(m)) {
            pathd = true;
            add_material(p_board, captured_piece(m));
        }
    }
    rewind_state(p_board);
#ifdef DEBUG
    init_board_from_squares(p_board, p_board->info.to_play,
                         p_board->info.state[p_board->info.current_move].stale_moves,
                         p_board->info.state[p_board->info.current_move].castle_white,
                         p_board->info.state[p_board->info.current_move].castle_black,
                         p_board->info.state[p_board->info.current_move].en_passant_square,
                         p_board->info.current_move);

    if(false && !debug_board(p_board)) {
                    print_move(m);
    }
    if (p_board->info.state[p_board->info.current_move]._zobrist_key
        != full_zobrist_key(p_board)) {
        print_move(m);
        display_board(p_board);
        printf("%d %d %d %d\n", patha, pathb, pathc, pathd);
        assert(0 && "Rewind correctly.");
    }
#endif
}

void unmake_last_move(Board *p_board) {
    if(p_board->info.current_move > 0)
        unmake_move(p_board, p_board->info.state[p_board->info.current_move].move);
}

void advance_state(Board *p_board, Move m) {
    int current_move = p_board->info.current_move;
    int square;

    U64 old_z_key = p_board->info.state[current_move]._zobrist_key;
    // still need to update the Zobrist key

    p_board->info.state[current_move+1].move = m;

    // handle piece movement Zobrist modifications
    if (white_castle(m) | black_castle(m)) {
        // some castling happened!
        UCHAR to_idx = to(m);
        if (to_idx == C1) {
            // WHITE O-O-O
            old_z_key ^= piece_keys[WHITE_ROOK][A1];
            old_z_key ^= piece_keys[WHITE_ROOK][D1];
            old_z_key ^= piece_keys[WHITE_KING][E1];
            old_z_key ^= piece_keys[WHITE_KING][C1];
        } else if (to_idx == G1) {
            // WHITE O-O
            old_z_key ^= piece_keys[WHITE_ROOK][F1];
            old_z_key ^= piece_keys[WHITE_ROOK][H1];
            old_z_key ^= piece_keys[WHITE_KING][E1];
            old_z_key ^= piece_keys[WHITE_KING][G1];
        } else if (to_idx == C8) {
            // BLACK O-O-O
            old_z_key ^= piece_keys[BLACK_ROOK][A8];
            old_z_key ^= piece_keys[BLACK_ROOK][D8];
            old_z_key ^= piece_keys[BLACK_KING][E8];
            old_z_key ^= piece_keys[BLACK_KING][C8];
        } else {
            // BLACK O-O
            old_z_key ^= piece_keys[BLACK_ROOK][F8];
            old_z_key ^= piece_keys[BLACK_ROOK][H8];
            old_z_key ^= piece_keys[BLACK_KING][E8];
            old_z_key ^= piece_keys[BLACK_KING][G8];
        }
    } else {
        /* CASES:
         * capture
         *   en passant
         * original piece movement
         * promotion
         *   with capture?
         */

        // remove the piece that moved from the key
        old_z_key ^= piece_keys[moved_piece(m)][from(m)];

        if(!(white_en_passant(m) | black_en_passant(m))) {
            if (captured_piece(m)) old_z_key ^= piece_keys[captured_piece(m)][to(m)];
            if (promote(m)) {
                // only add the new piece, remember we removed the
                // moved piece already
                old_z_key ^= piece_keys[promote(m)][to(m)];
            } else {
                // can just set the moved piece at the movement location
                old_z_key ^= piece_keys[moved_piece(m)][to(m)];
            }
        } else {
            // en passant
            // the to field contains the destination of the capturing pawn
            old_z_key ^= piece_keys[moved_piece(m)][to(m)];
            // remove the captured pawn
            int ep_capt_idx = (color(moved_piece(m)) == WHITE) ? to(m) - 8 : to(m) + 8;
            old_z_key ^= piece_keys[captured_piece(m)][ep_capt_idx];
        }
    }

    // update according to the fifty move rule
    if(captured_piece(m) || moved_piece(m) % 8 == WHITE_PAWN) {
        p_board->info.state[current_move+1].stale_moves = 0;
    } else {
        p_board->info.state[current_move+1].stale_moves = p_board->info.state[current_move].stale_moves;
    }

    // en passant
    if(moved_piece(m) % 8 == WHITE_PAWN) {
        if(((from(m) - to(m) == 16) || (to(m) - from(m)) == 16)) {
            p_board->info.state[current_move+1].en_passant_square = (to(m) + from(m)) / 2;
        }
    } else {
        p_board->info.state[current_move+1].en_passant_square = INVALID_SQUARE;
    }

    // en passant Zobrist modification
    if (p_board->info.state[current_move].en_passant_square != INVALID_SQUARE) {
        old_z_key ^= ep_file_keys[FILE(p_board->info.state[current_move].en_passant_square)];
    }
    if (p_board->info.state[current_move+1].en_passant_square != INVALID_SQUARE) {
        old_z_key ^= ep_file_keys[FILE(p_board->info.state[current_move+1].en_passant_square)];
    }

    // castling
    // white
    UCHAR old_white_castling_rights = p_board->info.state[current_move].castle_white;
    UCHAR old_black_castling_rights = p_board->info.state[current_move].castle_black;
    bool invalidated_zobrist = false;

    p_board->info.state[current_move+1].castle_white = old_white_castling_rights;

    if (moved_piece(m) == WHITE_ROOK || captured_piece(m) == WHITE_ROOK) {
        if (moved_piece(m) == WHITE_ROOK) square = from(m);
        else square = to(m);
        if(square == A1) {
            p_board->info.state[current_move+1].castle_white &= ~CAN_CASTLE_OOO;
            invalidated_zobrist = true;
        } else if (square == H1) {
            p_board->info.state[current_move+1].castle_white &= ~CAN_CASTLE_OO;
            invalidated_zobrist = true;
        }
    } else if (moved_piece(m) == WHITE_KING && from(m) == E1) {
        p_board->info.state[current_move+1].castle_white = CANNOT_CASTLE;
        invalidated_zobrist = true;
    }
    // black
    p_board->info.state[current_move+1].castle_black = old_black_castling_rights;
    if (moved_piece(m) == BLACK_ROOK || captured_piece(m) == BLACK_ROOK) {
        if (moved_piece(m) == BLACK_ROOK) square = from(m);
        else square = to(m);
        if (square == A8) {
            p_board->info.state[current_move+1].castle_black &= ~CAN_CASTLE_OOO;
            invalidated_zobrist = true;
        } else if (square == H8) {
            p_board->info.state[current_move+1].castle_black &= ~CAN_CASTLE_OO;
            invalidated_zobrist = true;
        }
    } else if(moved_piece(m) == BLACK_KING && from(m) == E8) {
        p_board->info.state[current_move+1].castle_black = CANNOT_CASTLE;
        invalidated_zobrist = true;
    }
    if (invalidated_zobrist) {
        old_z_key ^= castling_rights_keys[old_white_castling_rights +
                                          4 * old_black_castling_rights];
        UCHAR new_white_castling_rights = p_board->info.state[current_move+1].castle_white;
        UCHAR new_black_castling_rights = p_board->info.state[current_move+1].castle_black;
        old_z_key ^= castling_rights_keys[new_white_castling_rights +
                                          4 * new_black_castling_rights];
    }


    // update the move index and switch colors to play
    p_board->info.to_play ^= 1;
    old_z_key ^= white_to_move_key;

    p_board->info.state[current_move+1]._zobrist_key = old_z_key;

    p_board->info.current_move++;

    // some debugging stuff
#ifdef DEBUG
    assert(p_board->info.current_move < MAX_MOVES_PER_GAME + 1);
    if (old_z_key != full_zobrist_key(p_board)) {
        printf("Man you gotta fix this.\n");
        print_move(m);
        display_board(p_board);
        assert(0 && "BAD ZOBRIST");
    }
#endif
}

void rewind_state(Board *p_board) {
    // we don't need to do anything else, the board simply forgets about what
    // happened in the future board states and we roll back to the last
    p_board->info.state[p_board->info.current_move].en_passant_square = INVALID_SQUARE;
    p_board->info.current_move--;
    p_board->info.to_play ^= 1;
}

void print_move(Move m) {
    static const char *lineA =
    "\tH G F E D C B A #PROMO# #CAPTD#\n"
    "\t- - - - - - - - ------- -------\n";
    static const char *lineB =
    "\t#MOVED# #FROM BITS# # TO BITS #\n"
    "\t------- ----------- -----------\n";
    static const char *footer=
    "A:    WHITE CASTLING\n"
    "B:    BLACK CASTLING\n"
    "C:    WHITE ENPASSANT\n"
    "D:    BLACK ENPASSANT\n"
    "E-H:  UNUSED FLAGS\n";
    printf("%s\t", lineA);
    for(int i = 31; i >= 16; i--) {
        printf("%d ", (m & BITSET(i)) ? 1 : 0);
    }
    printf("\n%s\t", lineB);
    for(int i = 15; i >= 0; i--) {
        printf("%d ", (m & BITSET(i)) ? 1 : 0);
    }
    printf("\n%s", footer);
    printf("From: %s\n", SQUARENAME[from(m)]);
    printf("To: %s\n", SQUARENAME[to(m)]);
    printf("Moved: %s\n", PIECE_NAMES_FULL[moved_piece(m)]);
    printf("Capt: %s\n", PIECE_NAMES_FULL[captured_piece(m)]);
    if (hash_move(m)) {
        printf("Move is from the hash table.\n");
    }
}

const int MVV_LVA_SCORES[16] =
    // NP, P, K, N,       B,    R,    Q
    {0, 1000, 0, 2000, 0, 3000, 4000, 5000,
     0, 1000, 0, 2000, 0, 3000, 4000, 5000};

int mvv_lva_score (Move m) {
    // returns its most valuable victim - least valuable attacker score
    return MVV_LVA_SCORES[captured_piece(m)] - (MVV_LVA_SCORES[moved_piece(m)]/1000);
}

int comp_move (const void* p_move_a, const void* p_move_b) {
    const Move move_a = *(const Move *)p_move_a;
    const Move move_b = *(const Move *)p_move_b;
    return -(mvv_lva_score(move_a) - mvv_lva_score(move_b));
}
