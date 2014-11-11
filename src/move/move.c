#include <stdio.h>
#include <assert.h>

#include "move.h"
#include "../board/board.h"
#include "../defines.h"
#include "../extglobals.h"

/* As a word of note:
 *   These functions assume that the move
 * is valid for the board and that the movement
 * obeys all piece laws. The rest is up to
 * the move generator itself. Furthermore
 * it assumes that the data contained in
 * m is coherent.
 */

void makeMove(Board *pBoard, Move m) {
    if(whiteCastle(m) | blackCastle(m)) {
        // CASTLING
        if(whiteCastle(m)) {
            castle(pBoard, to(m), WHITE);
        } else {
            castle(pBoard, to(m), BLACK);
        }
    } else {
        setEmptyAt(pBoard, from(m), movedPiece(m));

        if(!(whiteEnPassant(m) | blackEnPassant(m))) {
            setPieceAt(pBoard, to(m), movedPiece(m), capturedPiece(m));
        } else {
            // EN PASSANT
            setPieceAt(pBoard, to(m), movedPiece(m), 0);
            enPassant(pBoard, color(movedPiece(m)));
        }

        if(promote(m)) {
            // PROMOTION
            doPromote(pBoard, to(m), promote(m));
            removeMaterial(pBoard, movedPiece(m));
            addMaterial(pBoard, promote(m));
        }
        if (capturedPiece(m))
            removeMaterial(pBoard, capturedPiece(m));
    }
    advanceState(pBoard, m);
    if(false && !debugBoard(pBoard)) {
            printMove(m);
    }
}

void unmakeMove(Board *pBoard, Move m) {
    if(whiteCastle(m) | blackCastle(m)) {
        // CASTLING
        if(whiteCastle(m)) {
            unCastle(pBoard, to(m), WHITE);
        } else {
            unCastle(pBoard, to(m), BLACK);
        }
    } else {
        if(promote(m)) {
            unPromote(pBoard, to(m));
            removeMaterial(pBoard, promote(m));
            addMaterial(pBoard, movedPiece(m));
        }

        setEmptyAt(pBoard, to(m), movedPiece(m));
        if(!(whiteEnPassant(m) | blackEnPassant(m))) {
            if(capturedPiece(m)) {
                setPieceAt(pBoard, to(m), capturedPiece(m), 0);
            }
            setPieceAt(pBoard, from(m), movedPiece(m), 0);
        } else {
            setPieceAt(pBoard, from(m), movedPiece(m), 0);
            unPassant(pBoard, to(m), color(movedPiece(m)));
        }
        if (capturedPiece(m))
            addMaterial(pBoard, capturedPiece(m));
    }
    rewindState(pBoard, m);
    if(false && !debugBoard(pBoard)) {
                    printMove(m);
    }
}

void unmakeLastMove(Board *pBoard) {
    if(pBoard->info.currentMove > 0)
        unmakeMove(pBoard, pBoard->info.state[pBoard->info.currentMove].move);
}

void advanceState(Board *pBoard, Move m) {
    int currentMove = pBoard->info.currentMove;
    int square;

    U64 old_z_key = pBoard->info.state[currentMove]._zobrist_key;
    // still need to update the Zobrist key

    pBoard->info.state[currentMove+1].move = m;

    // handle piece movement Zobrist modifications
    if (whiteCastle(m) | blackCastle(m)) {
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
        old_z_key ^= piece_keys[movedPiece(m)][from(m)];

        if(!(whiteEnPassant(m) | blackEnPassant(m))) {
            if (capturedPiece(m)) old_z_key ^= piece_keys[capturedPiece(m)][to(m)];
            if (promote(m)) {
                // only add the new piece, remember we removed the
                // moved piece already
                old_z_key ^= piece_keys[promote(m)][to(m)];
            } else {
                // can just set the moved piece at the movement location
                old_z_key ^= piece_keys[movedPiece(m)][to(m)];
            }
        } else {
            // en passant
            // the to field contains the destination of the capturing pawn
            old_z_key ^= piece_keys[movedPiece(m)][to(m)];
            // remove the captured pawn
            int ep_capt_idx = (color(movedPiece(m)) == WHITE) ? to(m) - 8 : to(m) + 8;
            old_z_key ^= piece_keys[capturedPiece(m)][ep_capt_idx];
        }
    }

    // update according to the fifty move rule
    if(capturedPiece(m) || movedPiece(m) % 8 == WHITE_PAWN) {
        pBoard->info.state[currentMove+1].staleMoves = 0;
    } else {
        pBoard->info.state[currentMove+1].staleMoves = pBoard->info.state[currentMove].staleMoves;
    }

    // en passant
    if(movedPiece(m) % 8 == WHITE_PAWN) {
        if(((from(m) - to(m) == 16) || (to(m) - from(m)) == 16)) {
            pBoard->info.state[currentMove+1].enPassantSquare = (to(m) + from(m)) / 2;
        }
    } else {
        pBoard->info.state[currentMove+1].enPassantSquare = INVALID_SQUARE;
    }

    // en passant Zobrist modification
    if (pBoard->info.state[currentMove].enPassantSquare != INVALID_SQUARE) {
        old_z_key ^= ep_file_keys[FILE(pBoard->info.state[currentMove].enPassantSquare)];
    }
    if (pBoard->info.state[currentMove+1].enPassantSquare != INVALID_SQUARE) {
        old_z_key ^= ep_file_keys[FILE(pBoard->info.state[currentMove+1].enPassantSquare)];
    }

    // castling
    // white
    UCHAR old_white_castling_rights = pBoard->info.state[currentMove].castleWhite;
    UCHAR old_black_castling_rights = pBoard->info.state[currentMove].castleBlack;
    bool invalidated_zobrist = false;

    pBoard->info.state[currentMove+1].castleWhite = old_white_castling_rights;

    if (movedPiece(m) == WHITE_ROOK || capturedPiece(m) == WHITE_ROOK) {
        if (movedPiece(m) == WHITE_ROOK) square = from(m);
        else square = to(m);
        if(square == A1) {
            pBoard->info.state[currentMove+1].castleWhite &= ~CAN_CASTLE_OOO;
            invalidated_zobrist = true;
        } else if (square == H1) {
            pBoard->info.state[currentMove+1].castleWhite &= ~CAN_CASTLE_OO;
            invalidated_zobrist = true;
        }
    } else if (movedPiece(m) == WHITE_KING && from(m) == E1) {
        pBoard->info.state[currentMove+1].castleWhite = CANNOT_CASTLE;
        invalidated_zobrist = true;
    }
    // black
    pBoard->info.state[currentMove+1].castleBlack = old_black_castling_rights;
    if (movedPiece(m) == BLACK_ROOK || capturedPiece(m) == BLACK_ROOK) {
        if (movedPiece(m) == BLACK_ROOK) square = from(m);
        else square = to(m);
        if (square == A8) {
            pBoard->info.state[currentMove+1].castleBlack &= ~CAN_CASTLE_OOO;
            invalidated_zobrist = true;
        } else if (square == H8) {
            pBoard->info.state[currentMove+1].castleBlack &= ~CAN_CASTLE_OO;
            invalidated_zobrist = true;
        }
    } else if(movedPiece(m) == BLACK_KING && from(m) == E8) {
        pBoard->info.state[currentMove+1].castleBlack = CANNOT_CASTLE;
        invalidated_zobrist = true;
    }
    if (invalidated_zobrist) {
        old_z_key ^= castling_rights_keys[old_white_castling_rights +
                                          4 * old_black_castling_rights];
        UCHAR new_white_castling_rights = pBoard->info.state[currentMove+1].castleWhite;
        UCHAR new_black_castling_rights = pBoard->info.state[currentMove+1].castleBlack;
        old_z_key ^= castling_rights_keys[new_white_castling_rights +
                                          4 * new_black_castling_rights];
    }


    // update the move index and switch colors to play
    pBoard->info.toPlay ^= 1;

    // this needs to be fixed
    old_z_key ^= white_to_move_key;

    pBoard->info.currentMove++;
    old_z_key = fullZobristKey(pBoard);
    pBoard->info.state[currentMove+1]._zobrist_key = old_z_key;

    // some debugging stuff
    assert(pBoard->info.currentMove < MAX_MOVES_PER_GAME);

    if (old_z_key != fullZobristKey(pBoard)) {
        printf("Man you gotta fix this.\n");
        printf("\n");
        printMove(m);
        displayBoard(pBoard);
        old_z_key = fullZobristKey(pBoard);
        pBoard->info.state[pBoard->info.currentMove]._zobrist_key = old_z_key;
    }

    assert(old_z_key == fullZobristKey(pBoard));
}

void rewindState(Board *pBoard, Move m) {
    // we don't need to do anything else, the board simply forgets about what
    // happened in the future board states and we roll back to the last
    pBoard->info.state[pBoard->info.currentMove].enPassantSquare = INVALID_SQUARE;
    pBoard->info.currentMove--;
    assert(pBoard->info.currentMove >= 0);
    pBoard->info.toPlay ^= 1;
}

void printMove(Move m) {
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
}

int compMove (const void* p_move_a, const void* p_move_b) {
    const Move move_a = *(const Move *)p_move_a;
    const Move move_b = *(const Move *)p_move_b;

    int cap_val_a = capturedPiece(move_a);
    cap_val_a = (cap_val_a > 8) ? cap_val_a - 8 : cap_val_a;

    int cap_val_b = capturedPiece(move_b);
    cap_val_b = (cap_val_b > 8) ? cap_val_b - 8 : cap_val_b;


    return -(cap_val_a - cap_val_b);
}
