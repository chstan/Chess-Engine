#include <assert.h>
#include <stdio.h>

#include "board.h"
#include "../bit.h"
#include "../defines.h"
#include "../extglobals.h"
#include "../move/move.h"
#include "../move/movegen.h"

void reset_board(Board *p_board) {
    for(int i = 0; i < 64; i++) p_board->position.square[i] = EMPTY;
    for(int i = 0; i < MAX_MOVES_PER_GAME; i++) {
        p_board->info.state[i].en_passant_square = INVALID_SQUARE;
    }

    p_board->position.square[A1] = WHITE_ROOK;
    p_board->position.square[B1] = WHITE_KNIGHT;
    p_board->position.square[C1] = WHITE_BISHOP;
    p_board->position.square[D1] = WHITE_QUEEN;
    p_board->position.square[E1] = WHITE_KING;
    p_board->position.square[F1] = WHITE_BISHOP;
    p_board->position.square[G1] = WHITE_KNIGHT;
    p_board->position.square[H1] = WHITE_ROOK;
    p_board->position.square[A2] = WHITE_PAWN;
    p_board->position.square[B2] = WHITE_PAWN;
    p_board->position.square[C2] = WHITE_PAWN;
    p_board->position.square[D2] = WHITE_PAWN;
    p_board->position.square[E2] = WHITE_PAWN;
    p_board->position.square[F2] = WHITE_PAWN;
    p_board->position.square[G2] = WHITE_PAWN;
    p_board->position.square[H2] = WHITE_PAWN;

    p_board->position.square[A8] = BLACK_ROOK;
    p_board->position.square[B8] = BLACK_KNIGHT;
    p_board->position.square[C8] = BLACK_BISHOP;
    p_board->position.square[D8] = BLACK_QUEEN;
    p_board->position.square[E8] = BLACK_KING;
    p_board->position.square[F8] = BLACK_BISHOP;
    p_board->position.square[G8] = BLACK_KNIGHT;
    p_board->position.square[H8] = BLACK_ROOK;
    p_board->position.square[A7] = BLACK_PAWN;
    p_board->position.square[B7] = BLACK_PAWN;
    p_board->position.square[C7] = BLACK_PAWN;
    p_board->position.square[D7] = BLACK_PAWN;
    p_board->position.square[E7] = BLACK_PAWN;
    p_board->position.square[F7] = BLACK_PAWN;
    p_board->position.square[G7] = BLACK_PAWN;
    p_board->position.square[H7] = BLACK_PAWN;

    p_board->info.display_rotated = false;
    p_board->info.current_move = 0;

    init_board_from_squares(p_board, WHITE, 0, CAN_CASTLE, CAN_CASTLE, INVALID_SQUARE, 0);
    return;
}

U64 full_zobrist_key(Board *p_board) {
    U64 key = 0;
    for (int sq_idx = 0; sq_idx < 64; sq_idx++) {
        UCHAR at_sq = p_board->position.square[sq_idx];
        if(is_piece(at_sq)) {
            key ^= piece_keys[at_sq][sq_idx];
        }
    }
    MoveInfo m_info = p_board->info.state[p_board->info.current_move];
    key ^= castling_rights_keys[(m_info.castle_white + 4*m_info.castle_black)];

    if (p_board->info.to_play == W) key ^= white_to_move_key;

    int ep_square = m_info.en_passant_square;
    if (ep_square != INVALID_SQUARE) key ^= ep_file_keys[FILE(ep_square)];

    return key;
}

void init_board_from_squares(Board* p_board, unsigned char to_play, int stale_moves, int castle_W,
int castle_B, int en_passant_square, int turn_count) {

    reset_BB(p_board);
    update_BB_from_squares(p_board);
    update_aggregate_BB(p_board);
    update_material_from_BB(p_board);
    update_piece_counts_from_BB(p_board);

    p_board->info.current_move = turn_count;
    p_board->info.to_play = to_play;
    p_board->info.state[p_board->info.current_move].castle_white = castle_W;
    p_board->info.state[p_board->info.current_move].castle_black = castle_B;
    p_board->info.state[p_board->info.current_move].en_passant_square = en_passant_square;
    p_board->info.state[p_board->info.current_move].stale_moves = stale_moves;

    // rebuild Zobrist key
    p_board->info.state[p_board->info.current_move]._zobrist_key = full_zobrist_key(p_board);

    return;
}

void display_board(Board *p_board) {
    const char *header  = "   A     B     C     D     E     F     G     H   ";
    const char *reverse = "   H     G     F     E     D     C     B     A   ";
    const char *divider = "+-----+-----+-----+-----+-----+-----+-----+-----+";

    printf("\n\t%s\n", p_board->info.to_play ? reverse : header);

    int i, j;
    for(i = 0; i < 8; i++) {
        printf("\t%s\n\t", divider);
        for(j = 0; j < 8; j++) {
            switch(p_board->info.to_play) {
                case W:
                    printf("| %s ", PIECE_NAMES[p_board->position.square[SQ(8-i, j+1)]]);
                break;
                case B:
                    printf("| %s ", PIECE_NAMES[p_board->position.square[SQ(i+1, 8-j)]]);
                break;
            }
        }
        printf("|  %d\n", p_board->info.to_play ? i+1 : 8-i);
    }
    printf("\t%s\n\n", divider);

    if(p_board->info.to_play == W) {
        printf("White to play.\n");
    } else {
        printf("Black to play.\n");
    }
    printf("Material counts:\n\tWhite: %d\n\tBlack: %d\n", p_board->info.white_material, p_board->info.black_material);
    if(p_board->info.state[p_board->info.current_move].castle_white == CANNOT_CASTLE) printf("White cannot castle.\n");
    else {
        printf("White can castle");
        if(p_board->info.state[p_board->info.current_move].castle_white == CAN_CASTLE_OOO)
            printf(" queenside");
        if(p_board->info.state[p_board->info.current_move].castle_white == CAN_CASTLE_OO)
            printf(" kingside");
        printf(".\n");
    }
    if(p_board->info.state[p_board->info.current_move].castle_black == CANNOT_CASTLE) printf("Black cannot castle.\n");
    else {
        printf("Black can castle");
        if(p_board->info.state[p_board->info.current_move].castle_black == CAN_CASTLE_OOO)
            printf(" queenside");
        if(p_board->info.state[p_board->info.current_move].castle_black == CAN_CASTLE_OO)
            printf(" kingside");
        printf(".\n\n");
    }
    if(checks(p_board, WHITE)) {
        printf("White is in check.\n");
    } else if(checks(p_board, BLACK)) {
        printf("Black is in check.\n\n");
    }
    int ep_square = p_board->info.state[p_board->info.current_move].en_passant_square;
    if(ep_square == INVALID_SQUARE)
        printf("EP Square : --\n");
    else
        printf("EP Square : %s\n", SQUARENAME[ep_square]);
}

void rotate_board(Board *p_board) {
    p_board->info.display_rotated = !p_board->info.display_rotated;
}

bool debug_board(Board *p_board) {
    // we first check if the board is in a consistent state, and report to the user
    // certain problems cannot be found at runtime however, and require prior knowledge of the
    // state of the board, for instance in the case of castling.
    bool board_consistent = true;
    for(int i = 0; i < 64; i++) {
        UCHAR current_piece = p_board->position.square[i];
        if(current_piece != EMPTY) {
            if(!(BITSET(i) & p_board->position.piece_BB[current_piece])) {
                board_consistent = false;
                printf("The array of squares reports a %s at %s,"
                       " but the bitboard is empty.\n",
                       PIECE_NAMES_FULL[current_piece], SQUARENAME[i]);
            }
        }
    }

    // material checks
    int materialB = p_board->info.black_material;
    int materialW = p_board->info.white_material;

    update_material_from_BB(p_board);

    if (materialB != p_board->info.black_material ||
        materialW != p_board->info.white_material) {
        printf("Material values are not consistent. WP%d WA%d BP%d BA%d",
               materialW, p_board->info.white_material,
               materialB, p_board->info.black_material);
    }
    //assert(materialB == p_board->info.black_material);
    //assert(materialW == p_board->info.white_material);

    // array of pieces
    // no real check needed here at the moment, though one could be added for material consistency

    // bitboards
    for(int current_pieceType = 0; current_pieceType < TOTAL_PIECE_TYPES; current_pieceType++) {
        if(is_piece(current_pieceType)) {
            U64 currentU64 = p_board->position.piece_BB[current_pieceType];
            int index = -1, shift = 0;
            while(currentU64) {
                shift = LSB(currentU64)+1;
                if (shift < 64) currentU64 >>= shift;
                else currentU64 = 0;
                index += shift;
                if(p_board->position.square[index] != current_pieceType) {
                    board_consistent = false;
                    printf("The bitboards report a %s at %s, but the array of pieces does not.\n", PIECE_NAMES_FULL[current_pieceType], SQUARENAME[index]);
                }
            }
        }
    }

    // rudimentary castling checks
    switch(p_board->info.state[p_board->info.current_move].castle_white) {
        break;
        case CAN_CASTLE_OO:
            if(p_board->position.square[E1] != WHITE_KING || p_board->position.square[H1] != WHITE_ROOK) {
                board_consistent = false;
                printf("The board info incorrectly reports that white can castle kingside.");
            }
        break;
        case CAN_CASTLE_OOO:
            if(p_board->position.square[E1] != WHITE_KING || p_board->position.square[A1] != WHITE_ROOK) {
                board_consistent = false;
                printf("The board info incorrectly reports that white can castle queenside.");
            }
        break;
        case CAN_CASTLE_OO | CAN_CASTLE_OOO:
            if(p_board->position.square[E1] != WHITE_KING || p_board->position.square[A1] != WHITE_ROOK || p_board->position.square[H1] != WHITE_ROOK) {
                board_consistent = false;
                printf("The board info incorrectly reports that white can castle kingside or queenside.");
            }
        break;
    }

    switch(p_board->info.state[p_board->info.current_move].castle_black) {
        case CAN_CASTLE_OO:
            if(p_board->position.square[E8] != BLACK_KING || p_board->position.square[H8] != BLACK_ROOK) {
                board_consistent = false;
                printf("The board info incorrectly reports that black can castle kingside.");
            }
        break;
        case CAN_CASTLE_OOO:
            if(p_board->position.square[E8] != BLACK_KING || p_board->position.square[A8] != BLACK_ROOK) {
                board_consistent = false;
                printf("The board info incorrectly reports that black can castle queenside.");
            }
        break;
        case CAN_CASTLE_OO | CAN_CASTLE_OOO:
            if(p_board->position.square[E8] != BLACK_KING || p_board->position.square[A8] != BLACK_ROOK || p_board->position.square[H8] != BLACK_ROOK) {
                board_consistent = false;
                printf("The board info incorrectly reports that black can castle kingside or queenside.");
            }
        break;
    }

    // if it is not, we will print out a diagnostic
    if(!board_consistent) {
        display_board(p_board);
        // other info would be nice too, but this is fine for the moment
        // leave a breakpoint for debugging, not that much can be found here...
        #ifdef __clang__
        __asm__("int3");
        #elif (__GNUC__ || __cplusplus)
        __asm__("int3");
        #endif
    }
    return board_consistent;
}

//---------------------MAKE FUNCTIONS------------------------
void set_empty_at(Board *p_board, UCHAR index, UCHAR last_occupant) {
    //DOES NOT UPDATE ANY MATERIAL COUNTS!!!
    p_board->position.piece_BB[last_occupant] &= ~BITSET(index);
    p_board->position.occupied &= ~BITSET(index);
    p_board->position.square[index] = EMPTY;
    if(color(last_occupant) == B) {
        p_board->position.black_occupied &= ~BITSET(index);
    } else {
        p_board->position.white_occupied &= ~BITSET(index);
    }
}

void set_piece_at(Board *p_board, UCHAR index, UCHAR moved_piece, UCHAR captured_piece) {
    if(captured_piece) {
        set_empty_at(p_board, index, captured_piece);
    }
    p_board->position.piece_BB[moved_piece] |= BITSET(index);
    p_board->position.occupied |= BITSET(index);
    p_board->position.square[index] = moved_piece;
    if(color(moved_piece) == B) {
        p_board->position.black_occupied |= BITSET(index);
    } else {
        p_board->position.white_occupied |= BITSET(index);
    }
}

void update_en_passant_square(Board *p_board, int index) {
    p_board->info.state[p_board->info.current_move].en_passant_square = index;
}

void en_passant(Board *p_board, int color) {
    // the color should be the color of the *capturing* pawn
    // find where the en passant will take place and
    // make sure the board is updated so that no further enpassant is possible
    int index = p_board->info.state[p_board->info.current_move].en_passant_square;

    // the piece will either be a white pawn or a black pawn, but
    // we can get it either way here
    if(color == WHITE) {
        set_empty_at(p_board, index - 8, BLACK_PAWN);
    } else {
        set_empty_at(p_board, index + 8, WHITE_PAWN);
    }
    return;
}

void un_passant(Board *p_board, int index, int color) {
    // a horrible corruption of the French language... mon Dieu!
    if(color == BLACK) {
        set_piece_at(p_board, index + 8, WHITE_PAWN, NO_PIECE);
    } else {
        set_piece_at(p_board, index - 8, BLACK_PAWN, NO_PIECE);
    }
}

void castle(Board *p_board, UCHAR index, UCHAR which_king) {
    switch(which_king) {
        case W:
            assert(index == C1 || index == G1);
            if(index == C1) {
                set_empty_at(p_board, E1, WHITE_KING);
                set_piece_at(p_board, C1, WHITE_KING, 0);
                set_empty_at(p_board, A1, WHITE_ROOK);
                set_piece_at(p_board, D1, WHITE_ROOK, 0);
            } else {
                set_empty_at(p_board, E1, WHITE_KING);
                set_piece_at(p_board, G1, WHITE_KING, 0);
                set_empty_at(p_board, H1, WHITE_ROOK);
                set_piece_at(p_board, F1, WHITE_ROOK, 0);
            }
        break;
        case B:
            assert(index == C8 || index == G8);
            if(index == C8) {
                set_empty_at(p_board, E8, BLACK_KING);
                set_piece_at(p_board, C8, BLACK_KING, 0);
                set_empty_at(p_board, A8, BLACK_ROOK);
                set_piece_at(p_board, D8, BLACK_ROOK, 0);
            } else {
                set_empty_at(p_board, E8, BLACK_KING);
                set_piece_at(p_board, G8, BLACK_KING, 0);
                set_empty_at(p_board, H8, BLACK_ROOK);
                set_piece_at(p_board, F8, BLACK_ROOK, 0);
            }
        break;
    }
    return;
}

void un_castle(Board *p_board, UCHAR index, UCHAR which_king) {
    switch(which_king) {
        case W:
            assert(index == C1 || index == G1);
            if(index == C1) {
                set_empty_at(p_board, C1, WHITE_KING);
                set_piece_at(p_board, E1, WHITE_KING, 0);
                set_empty_at(p_board, D1, WHITE_ROOK);
                set_piece_at(p_board, A1, WHITE_ROOK, 0);
            } else {
                set_empty_at(p_board, G1, WHITE_KING);
                set_piece_at(p_board, E1, WHITE_KING, 0);
                set_empty_at(p_board, F1, WHITE_ROOK);
                set_piece_at(p_board, H1, WHITE_ROOK, 0);
            }
        break;
        case B:
            assert(index == C8 || index == G8);
            if(index == C8) {
                set_empty_at(p_board, C8, BLACK_KING);
                set_piece_at(p_board, E8, BLACK_KING, 0);
                set_empty_at(p_board, D8, BLACK_ROOK);
                set_piece_at(p_board, A8, BLACK_ROOK, 0);
            } else {
                set_empty_at(p_board, G8, BLACK_KING);
                set_piece_at(p_board, E8, BLACK_KING, 0);
                set_empty_at(p_board, F8, BLACK_ROOK);
                set_piece_at(p_board, H8, BLACK_ROOK, 0);
            }
        break;
    }
    return;
}

void do_promote(Board *p_board, UCHAR index, UCHAR which_promote) {
    switch(color(which_promote)) {
        case W:
            set_empty_at(p_board, index, WHITE_PAWN);
            set_piece_at(p_board, index, which_promote, 0);
        break;
        case B:
            set_empty_at(p_board, index, BLACK_PAWN);
            set_piece_at(p_board, index, which_promote, 0);
        break;
    }
    return;
}

void un_promote(Board *p_board, UCHAR index) {
    switch(color(p_board->position.square[index])) {
        case W:
            set_empty_at(p_board, index, p_board->position.square[index]);
            set_piece_at(p_board, index, WHITE_PAWN, 0);
        break;
        case B:
            set_empty_at(p_board, index, p_board->position.square[index]);
            set_piece_at(p_board, index, BLACK_PAWN, 0);
        break;
    }
    return;
}

void add_material(Board *p_board, UCHAR piece) {
    p_board->position.pieces[piece]++;
    if(color(piece) == WHITE) {
        p_board->position.total_white++;
    } else {
        p_board->position.total_black++;
    }
    p_board->position.total_pieces++;
    if(color(piece) == BLACK) {
        p_board->info.black_material += PIECEVALUES[piece];
    } else {
        p_board->info.white_material += PIECEVALUES[piece];
    }
    p_board->info.material = p_board->info.white_material - p_board->info.black_material;
    return;
}

void remove_material(Board *p_board, UCHAR piece) {
    p_board->position.pieces[piece]--;
    if(color(piece) == WHITE) {
        p_board->position.total_white--;
    } else {
        p_board->position.total_black--;
    }
    p_board->position.total_pieces--;
    if(color(piece) == BLACK) {
        p_board->info.black_material -= PIECEVALUES[piece];
    } else {
        p_board->info.white_material -= PIECEVALUES[piece];
    }
    p_board->info.material = p_board->info.white_material - p_board->info.black_material;
    return;
}

//---------------------AUX FUNCTIONS-------------------------
void update_piece_counts_from_BB(Board *p_board) {
    // Reset everything
    for(int current_pieceType = 0; current_pieceType < TOTAL_PIECE_TYPES; current_pieceType++) {
        p_board->position.pieces[current_pieceType] = 0;
    }

    p_board->position.total_white = 0;
    p_board->position.total_black = 0;

    // now iterate across the squares in order to update piece counts
    for(int currentSquare = 0; currentSquare < 64; currentSquare++) {
        UCHAR current_piece = p_board->position.square[currentSquare];
        if(is_piece(current_piece)) {
            int color = color(current_piece);
            p_board->position.pieces[current_piece]++;
            if(color == WHITE) {
                p_board->position.total_white++;
            } else {
                p_board->position.total_black++;
            }
        }
    }

    p_board->position.total_pieces = p_board->position.total_white + p_board->position.total_black;
    return;
}

void reset_BB(Board *p_board) {
    for(int current_pieceType = 0; current_pieceType < TOTAL_PIECE_TYPES; current_pieceType++) {
        p_board->position.piece_BB[current_pieceType] = 0;
    }

    p_board->position.white_occupied = 0;
    p_board->position.black_occupied = 0;
    p_board->position.occupied = 0;
    return;
}

void update_aggregate_BB(Board *p_board) {
    p_board->position.white_occupied = p_board->position.piece_BB[WHITE_PAWN] |
                                                                    p_board->position.piece_BB[WHITE_KING] |
                                                                    p_board->position.piece_BB[WHITE_QUEEN] |
                                                                    p_board->position.piece_BB[WHITE_BISHOP] |
                                                                    p_board->position.piece_BB[WHITE_KNIGHT] |
                                                                    p_board->position.piece_BB[WHITE_ROOK];

    p_board->position.black_occupied = p_board->position.piece_BB[BLACK_PAWN] |
                                                                    p_board->position.piece_BB[BLACK_KING] |
                                                                    p_board->position.piece_BB[BLACK_QUEEN] |
                                                                    p_board->position.piece_BB[BLACK_BISHOP] |
                                                                    p_board->position.piece_BB[BLACK_KNIGHT] |
                                                                    p_board->position.piece_BB[BLACK_ROOK];

    p_board->position.occupied = p_board->position.white_occupied |
                                                            p_board->position.black_occupied;
    return;
}

void update_BB_from_squares(Board *p_board) {
    reset_BB(p_board);

    for(int currentSquare = 0; currentSquare < 64; currentSquare++) {
        UCHAR current_piece = p_board->position.square[currentSquare];
        if(is_piece(current_piece)) {
            p_board->position.piece_BB[current_piece] |= BITSET(currentSquare);
        }
    }

    update_aggregate_BB(p_board);
    return;
}

void update_material_from_BB(Board *p_board) {
    p_board->info.white_material =
        count_bits(p_board->position.piece_BB[WHITE_PAWN]) * PAWN_VALUE
      + count_bits(p_board->position.piece_BB[WHITE_BISHOP]) * BISHOP_VALUE
      + count_bits(p_board->position.piece_BB[WHITE_KNIGHT]) * KNIGHT_VALUE
      + count_bits(p_board->position.piece_BB[WHITE_ROOK]) * ROOK_VALUE
      + count_bits(p_board->position.piece_BB[WHITE_QUEEN]) * QUEEN_VALUE;

    p_board->info.black_material =
        count_bits(p_board->position.piece_BB[BLACK_PAWN]) * PAWN_VALUE
      + count_bits(p_board->position.piece_BB[BLACK_BISHOP]) * BISHOP_VALUE
      + count_bits(p_board->position.piece_BB[BLACK_KNIGHT]) * KNIGHT_VALUE
      + count_bits(p_board->position.piece_BB[BLACK_ROOK]) * ROOK_VALUE
      + count_bits(p_board->position.piece_BB[BLACK_QUEEN]) * QUEEN_VALUE;

    p_board->info.material =
        p_board->info.white_material
      - p_board->info.black_material;

    return;
}
