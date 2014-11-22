#include <assert.h>
#include <stdio.h>

#include "movegen.h"
#include "movegenhelpers.h"
#include "move.h"
#include "../extglobals.h"
#include "../bit.h"
#include "../defines.h"

const MoveGenCB moveCB[] = {
    NULL,
    pawn_move_BB,
    king_move_BB,
    knight_move_BB,
    NULL,
    bishop_move_BB,
    rook_move_BB,
    queen_move_BB,
    NULL,
    pawn_move_BB,
    king_move_BB,
    knight_move_BB,
    NULL,
    bishop_move_BB,
    rook_move_BB,
    queen_move_BB
};

const MoveGenCB timidCB[] = {
    NULL,
    pawn_timid_BB,
    king_timid_BB,
    knight_timid_BB,
    NULL,
    bishop_timid_BB,
    rook_timid_BB,
    queen_timid_BB,
    NULL,
    pawn_timid_BB,
    king_timid_BB,
    knight_timid_BB,
    NULL,
    bishop_timid_BB,
    rook_timid_BB,
    queen_timid_BB
};

const MoveGenCB captureCB[] = {
    NULL,
    pawn_capture_BB,
    king_capture_BB,
    knight_capture_BB,
    NULL,
    bishop_capture_BB,
    rook_capture_BB,
    queen_capture_BB,
    NULL,
    pawn_capture_BB,
    king_capture_BB,
    knight_capture_BB,
    NULL,
    bishop_capture_BB,
    rook_capture_BB,
    queen_capture_BB
};

bool moveset_contains(MoveSet *moves, Move m) {
    unset_hash_bit(m);
    for (int iter = 0; iter < moves->total_moves; iter++) {
        if (moves->move_list[iter] == m)
            return true;
    }
    return false;
}

__attribute__((unused)) static void debugMoves(MoveSet *p_moves) {
    for(int current_move_index = 0; current_move_index < p_moves->total_moves; current_move_index++) {
        Move toCheck = p_moves->move_list[current_move_index];
        for(int checkIndex = current_move_index+1; checkIndex < p_moves->total_moves; checkIndex++) {
            if(toCheck == p_moves->move_list[checkIndex]) {
                printf("Found duplicate move:\n");
                print_move(toCheck);
            }
        }
    }
}

static Move extractMove(int piece, int origin, int destination) {
    int occupant = 0;
    // we assume it's occupied by the appropriate color if this function is called
    if (p_board->position.occupied & BITSET(destination)) {
        occupant = p_board->position.square[destination];
    }

    return move(occupant, piece, origin, destination);
}

static void extractEnPassant(Board *p_board, MoveSet *p_moves) {
    U64 generated = en_passant_BB(p_board, p_board->info.to_play);
    int color = p_board->info.to_play;
    int piece = (color == WHITE) ? WHITE_PAWN : BLACK_PAWN;
    int taken = (color == WHITE) ? BLACK_PAWN : WHITE_PAWN;
    int origin = -1, shift = 0;
    while(generated) {
        shift = LSB(generated)+1;
        if (shift < 64) generated >>= shift;
        origin += shift;
        int to = p_board->info.state[p_board->info.current_move].en_passant_square;
        Move m = move_full((color == BLACK) ? 1 : 0, (color == WHITE) ? 1 : 0, 0, 0, 0, taken, piece, origin, to);
        //print_move(m);
        write_move(p_moves, m);
    }

    return;
}

void generatePromotions(Board *p_board, MoveSet *p_moves) {
    static int whitePromotionRank = 6;
    static int blackPromotionRank = 1;
    int color = p_board->info.to_play;
    U64 timidCandidates;
    U64 captureCandidates;
    switch(color) {
        case W:
            {
                // by removing all pawns that naturally cannot promote we speed this up greatly
                timidCandidates = p_board->position.piece_BB[WHITE_PAWN] & rankBB[whitePromotionRank];
                captureCandidates = timidCandidates;

                int origin = -1, shift = 0, destination = 0;
                U64 generatedMoves = 0;
                while(timidCandidates) {
                    shift = LSB(timidCandidates) + 1;
                    if(shift < 64) timidCandidates >>= shift;
                    else timidCandidates = 0;
                    origin += shift;
                    generatedMoves = pawn_promotion_BB(p_board, origin, color);
                    int i = 0;
                    destination = -1;
                    while(generatedMoves) {
                        i = LSB(generatedMoves) + 1;
                        if(i < 64) generatedMoves >>= i;
                        else generatedMoves = 0;
                        destination += i;
                        write_move(p_moves, move_full(0, 0, 0, 0, WHITE_QUEEN, 0, WHITE_PAWN, origin, destination));
                        write_move(p_moves, move_full(0, 0, 0, 0, WHITE_ROOK, 0, WHITE_PAWN, origin, destination));
                        write_move(p_moves, move_full(0, 0, 0, 0, WHITE_BISHOP, 0, WHITE_PAWN, origin, destination));
                        write_move(p_moves, move_full(0, 0, 0, 0, WHITE_KNIGHT, 0, WHITE_PAWN, origin, destination));
                    }
                }

                origin = -1;
                while(captureCandidates) {
                    shift = LSB(captureCandidates) + 1;
                    if(shift < 64) captureCandidates >>= shift;
                    else captureCandidates = 0;
                    origin += shift;
                    generatedMoves = pawn_promotion_capture_BB(p_board, origin, color);
                    int i = 0;
                    destination = -1;
                    while(generatedMoves) {
                        i = LSB(generatedMoves) + 1;
                        if(i < 64) generatedMoves >>= i;
                        else generatedMoves = 0;
                        destination += i;
                        write_move(p_moves, move_full(0, 0, 0, 0, WHITE_QUEEN, p_board->position.square[destination], WHITE_PAWN, origin, destination));
                        write_move(p_moves, move_full(0, 0, 0, 0, WHITE_ROOK, p_board->position.square[destination], WHITE_PAWN, origin, destination));
                        write_move(p_moves, move_full(0, 0, 0, 0, WHITE_BISHOP, p_board->position.square[destination], WHITE_PAWN, origin, destination));
                        write_move(p_moves, move_full(0, 0, 0, 0, WHITE_KNIGHT, p_board->position.square[destination], WHITE_PAWN, origin, destination));
                    }
                }
            }
        break;
        case B:
            {
                timidCandidates = p_board->position.piece_BB[BLACK_PAWN] & rankBB[blackPromotionRank];
                captureCandidates = timidCandidates;

                int origin = -1, shift = 0, destination = 0;
                U64 generatedMoves = 0;
                while(timidCandidates) {
                    shift = LSB(timidCandidates) + 1;
                    if(shift < 64) timidCandidates >>= shift;
                    else timidCandidates = 0;
                    origin += shift;
                    generatedMoves = pawn_promotion_BB(p_board, origin, color);
                    int i = 0;
                    destination = -1;
                    while(generatedMoves) {
                        i = LSB(generatedMoves) + 1;
                        if(i < 64) generatedMoves >>= i;
                        else generatedMoves = 0;
                        destination += i;
                        write_move(p_moves, move_full(0, 0, 0, 0, BLACK_QUEEN, 0, BLACK_PAWN, origin, destination));
                        write_move(p_moves, move_full(0, 0, 0, 0, BLACK_ROOK, 0, BLACK_PAWN, origin, destination));
                        write_move(p_moves, move_full(0, 0, 0, 0, BLACK_BISHOP, 0, BLACK_PAWN, origin, destination));
                        write_move(p_moves, move_full(0, 0, 0, 0, BLACK_KNIGHT, 0, BLACK_PAWN, origin, destination));
                    }
                }

                origin = -1;
                while(captureCandidates) {
                    shift = LSB(captureCandidates) + 1;
                    if(shift < 64) captureCandidates >>= shift;
                    else captureCandidates = 0;
                    origin += shift;
                    generatedMoves = pawn_promotion_capture_BB(p_board, origin, color);
                    int i = 0;
                    destination = -1;
                    while(generatedMoves) {
                        i = LSB(generatedMoves) + 1;
                        if(i < 64) generatedMoves >>= i;
                        else generatedMoves = 0;
                        destination += i;
                        write_move(p_moves, move_full(0, 0, 0, 0, BLACK_QUEEN, p_board->position.square[destination], BLACK_PAWN, origin, destination));
                        write_move(p_moves, move_full(0, 0, 0, 0, BLACK_ROOK, p_board->position.square[destination], BLACK_PAWN, origin, destination));
                        write_move(p_moves, move_full(0, 0, 0, 0, BLACK_BISHOP, p_board->position.square[destination], BLACK_PAWN, origin, destination));
                        write_move(p_moves, move_full(0, 0, 0, 0, BLACK_KNIGHT, p_board->position.square[destination], BLACK_PAWN, origin, destination));
                    }
                }
            }
        break;
    }
}

static U64 attackedSquaresByPiece(Board *p_board, int piece, int side) {
    static int whitePromotionRank = 6;
    static int blackPromotionRank = 1;

    // I hate repeating this everywhere... makes you wish for a more expressive language
    U64 current_pieces = p_board->position.piece_BB[piece];
    int origin = -1, shift = 0;
    U64 generatedMoves = 0;
    while(current_pieces) {
        shift = LSB(current_pieces)+1;
        if(shift < 64) current_pieces >>= shift;
        else current_pieces = 0;
        origin += shift;
        // although slower unoptimized, the optimizer will hoist this out of the white, because
        // the value of piece cannot be changed
        switch(piece) {
            case WP:
                if(BITSET(origin) & rankBB[whitePromotionRank]) {
                    generatedMoves |= pawnPromotionCaptureWhite[origin];
                } else {
                    generatedMoves |= pawnCaptureWhite[origin];
                }
            break;
            case BP:
                if(BITSET(origin) & rankBB[blackPromotionRank]) {
                    generatedMoves |= pawnPromotionCaptureBlack[origin];
                } else {
                    generatedMoves |= pawnCaptureBlack[origin];
                }
            break;
            case WN:
            case BN:
                generatedMoves |= knightMove[origin];
            break;
            case WB:
            case BB:
                generatedMoves |= bishop_move_BB(p_board, origin, side);
            break;
            case WR:
            case BR:
                generatedMoves |= rook_move_BB(p_board, origin, side);
            break;
            case WK:
            case BK:
                generatedMoves |= kingMove[origin];
            break;
            case WQ:
            case BQ:
                generatedMoves |= queen_move_BB(p_board, origin, side);
            break;
            default:
                continue;
            break;
        }
    }
    return generatedMoves;
}

// side is the color *DOING* the attacking
// though changing this is easy, if you'd like
U64 attackedSquares(Board *p_board, int side) {
    U64 attacked = 0;
    int piece, endpiece;
    if(side == BLACK) {
        piece = BP;
        endpiece = BQ;
    } else {
        piece = WP;
        endpiece = WQ;
    }

    for(; piece <= endpiece; piece++) {
        attacked |= attackedSquaresByPiece(p_board, piece, side);
    }
    return attacked;
}

void generateCastle(Board *p_board, MoveSet *p_moves) {
    U64 WhiteKingsideCheck = BITSET(E1) | BITSET(F1) | BITSET(G1);
    U64 WhiteKingsideClearance = BITSET(F1) | BITSET(G1);
    U64 BlackKingsideCheck = BITSET(E8) | BITSET(F8) | BITSET(G8);
    U64 BlackKingsideClearance = BITSET(F8) | BITSET(G8);

    U64 WhiteQueensideCheck = BITSET(E1) | BITSET(D1) | BITSET(C1);
    U64 WhiteQueensideClearance =  BITSET(D1) | BITSET(C1) | BITSET(B1);
    U64 BlackQueensideCheck = BITSET(E8) | BITSET(D8) | BITSET(C8);
    U64 BlackQueensideClearance = BITSET(D8) | BITSET(C8) | BITSET(B8);


    switch(p_board->info.to_play) {
        case W:
            {
                // check if white can castle
                int castle_white = p_board->info.state[p_board->info.current_move].castle_white;
                if(castle_white) {
                    U64 blackAttacks = attackedSquares(p_board, BLACK);
                    if(castle_white & CAN_CASTLE_OO) {
                        if(!(p_board->position.occupied & WhiteKingsideClearance) && !(blackAttacks & WhiteKingsideCheck))
                            write_move(p_moves, move_full(0, 0, 0, 1, 0, 0, WHITE_KING, E1, G1));
                    }
                    if(castle_white & CAN_CASTLE_OOO) {
                        if(!(p_board->position.occupied & WhiteQueensideClearance) && !(blackAttacks & WhiteQueensideCheck))
                            write_move(p_moves, move_full(0, 0, 0, 1, 0, 0, WHITE_KING, E1, C1));
                    }
                }
            }
        break;
        case B:
            {
                // check if black can castle
                int castle_black = p_board->info.state[p_board->info.current_move].castle_black;
                if(castle_black) {
                    U64 whiteAttacks = attackedSquares(p_board, WHITE);
                    if(castle_black & CAN_CASTLE_OO) {
                        if(!(p_board->position.occupied & BlackKingsideClearance) && !(whiteAttacks & BlackKingsideCheck))
                            write_move(p_moves, move_full(0, 0, 1, 0, 0, 0, BLACK_KING, E8, G8));
                    }
                    if(castle_black & CAN_CASTLE_OOO) {
                        if(!(p_board->position.occupied & BlackQueensideClearance) && !(whiteAttacks & BlackQueensideCheck))
                            write_move(p_moves, move_full(0, 0, 1, 0, 0, 0, BLACK_KING, E8, C8));
                    }
                }
            }
        break;
    }
}

void generate_agnostic(Board *p_board, int color, U64 current_pieces,
int piece, MoveSet *p_moves, U64 (*moveGen)(Board *p_board, UCHAR origin, int color)) {
    int origin = -1, shift = 0, destination = 0;
    int count = 0;
    U64 generatedMoves = 0;
    while(current_pieces) {
        shift = LSB(current_pieces)+1;
        if (shift < 64) current_pieces >>= shift;
        else current_pieces = 0;
        origin += shift;
        generatedMoves = moveGen(p_board, origin, color);
        //if(generatedMoves) print_U64(generatedMoves);
        int i = 0;
        destination = -1;
        while(generatedMoves) {
            count += 1;
            i = LSB(generatedMoves)+1;
            if (i < 64) generatedMoves >>= i;
            else generatedMoves = 0;
            destination += i;
            write_move(p_moves, extractMove(piece, origin, destination));
        }
    }

    return;
}

void generate_timid(Board *p_board, MoveSet *p_moves) {
    U64 current_pieces = 0;
    int color = p_board->info.to_play;
    int piece, endpiece;

    if (color == BLACK) {
        piece = BP;
        endpiece = BQ;
    } else {
        piece = WP;
        endpiece = WQ;
    }

    for (; piece <= endpiece; piece++) {
        if (!is_piece(piece)) continue;
        current_pieces = p_board->position.piece_BB[piece];
        generate_agnostic(p_board, color, current_pieces, piece, p_moves, timidCB[piece]);
    }

    generateCastle(p_board, p_moves);

    return;
}

void generate_capture(Board *p_board, MoveSet *p_moves) {
    U64 current_pieces = 0;
    int color = p_board->info.to_play;
    int piece, endpiece;

    if (color == BLACK) {
        piece = BP;
        endpiece = BQ;
    } else {
        piece = WP;
        endpiece = WQ;
    }

    for (; piece <= endpiece; piece++) {
        if (!is_piece(piece)) continue;
        current_pieces = p_board->position.piece_BB[piece];
        generate_agnostic(p_board, color, current_pieces, piece, p_moves, captureCB[piece]);
    }

    // handle en_passant
    extractEnPassant(p_board, p_moves);

    return;
}

void generate_move(Board *p_board, MoveSet *p_moves) {
    U64 current_pieces = 0;
    int color = p_board->info.to_play;
    int piece, endpiece;

    if (color == BLACK) {
        piece = BP;
        endpiece = BQ;
    } else {
        piece = WP;
        endpiece = WQ;
    }

    for (; piece <= endpiece; piece++) {
        if (!is_piece(piece)) continue;
        current_pieces = p_board->position.piece_BB[piece];
        generate_agnostic(p_board, color, current_pieces, piece, p_moves, moveCB[piece]);
    }

    // promotions
    generatePromotions(p_board, p_moves);

    // castling
    generateCastle(p_board, p_moves);

    // handle en_passant
    extractEnPassant(p_board, p_moves);

    return;
}

U64 generatePieceAttacks(Board *p_board, int color, U64 current_pieces,
int piece) {
    int origin = -1, shift = 0;
    U64 generatedMoves = 0;
    while(current_pieces) {
        shift = LSB(current_pieces)+1;
        if (shift < 64) current_pieces >>= shift;
        else current_pieces = 0;
        origin += shift;
        generatedMoves |= (*captureCB[piece])(p_board, origin, color);
        if(piece % 8 == WHITE_PAWN) {
            generatedMoves |= pawn_promotion_capture_BB(p_board, origin, color);
        }
    }
    return generatedMoves;
}

// attacks ON a particular color (not BY the color)
U64 generate_all_attacks(Board *p_board, int color) {
    U64 attacks = 0;
    U64 current_pieces = 0;
    int piece, endpiece;

    if (color == BLACK) {
        color = WHITE;
        piece = WP;
        endpiece = WQ;
    } else {
        color = BLACK;
        piece = BP;
        endpiece = BQ;
    }

    for (; piece <= endpiece; piece++) {
        if (!is_piece(piece)) continue;
        current_pieces = p_board->position.piece_BB[piece];
        attacks |= generatePieceAttacks(p_board, color, current_pieces, piece);
    }
    return attacks;
}

U64 attacks(Board *p_board, int attackee_index, int side) {
    return BITSET(attackee_index) & generate_all_attacks(p_board, side);
}

bool checks(Board *p_board, int side) {
    int king_index;
    if (side == W) {
        king_index = LSB(p_board->position.piece_BB[WHITE_KING]);
    } else {
        king_index = LSB(p_board->position.piece_BB[BLACK_KING]);
    }
    return attacks(p_board, king_index, side) != 0;
}


//===============UNFINISHED===================

void generate_check(__attribute__((unused)) Board *p_board,
                   __attribute__((unused)) MoveSet *p_moves) {

    return;
}

void initialize_move_set(Board *p_board, MoveSet *p_moves) {
    reset_move_set(p_moves);
    printf("About to generate moves.\n");
    generate_capture(p_board, p_moves);
    generate_timid(p_board, p_moves);
    printf("Generated moves.\n");
    while(p_moves->move_iter < p_moves->total_moves)
        print_move(next_move(p_moves));
}

void initialize_move_set_quiet(Board *p_board, MoveSet *p_moves) {
    generate_move(p_board, p_moves);
}

void reset_move_set(MoveSet *p_moves) {
    p_moves->move_iter = 0;
    p_moves->current_move_index = 0;
    p_moves->timid_index = 0;
    p_moves->total_moves = 0;

    p_moves->killer_iter = 0;
    p_moves->total_killers = 0;
    p_moves->current_killer_index = 0;
}

Move next_move(MoveSet *p_moves) {
    assert(p_moves->move_iter < p_moves->total_moves);
    return p_moves->move_list[p_moves->move_iter++];
}

void write_move(MoveSet *p_moves, Move m) {
    assert(p_moves->current_move_index + 1 < MAX_MOVES_PER_PLY);
    p_moves->total_moves++;
    p_moves->move_list[p_moves->current_move_index] = m;
    p_moves->current_move_index++;
    if(captured_piece(m)) p_moves->timid_index++;
}

void write_killer(MoveSet *p_moves, Move killer) {
    assert(p_moves->current_killer_index + 1 < MAX_KILLERS_PER_PLY);
    p_moves->total_killers++;
    p_moves->killer_list[p_moves->current_killer_index++] = killer;
}
