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
    if(!debugBoard(pBoard)) {
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
    if(!debugBoard(pBoard)) {
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

    pBoard->info.state[currentMove+1].move = m;

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

    // castling
    // white
    pBoard->info.state[currentMove+1].castleWhite = pBoard->info.state[currentMove].castleWhite;
    if (movedPiece(m) == WHITE_ROOK || capturedPiece(m) == WHITE_ROOK) {
        if (movedPiece(m) == WHITE_ROOK) square = from(m);
        else square = to(m);
        if(square == A1) {
            pBoard->info.state[currentMove+1].castleWhite &= ~CAN_CASTLE_OOO;
        } else if (square == H1) {
            pBoard->info.state[currentMove+1].castleWhite &= ~CAN_CASTLE_OO;
        }
    } else if (movedPiece(m) == WHITE_KING && from(m) == E1) {
        pBoard->info.state[currentMove+1].castleWhite = CANNOT_CASTLE;
    }
    // black
    pBoard->info.state[currentMove+1].castleBlack = pBoard->info.state[currentMove].castleBlack;
    if (movedPiece(m) == BLACK_ROOK || capturedPiece(m) == BLACK_ROOK) {
        if (movedPiece(m) == BLACK_ROOK) square = from(m);
        else square = to(m);
        if (square == A8) {
            pBoard->info.state[currentMove+1].castleBlack &= ~CAN_CASTLE_OOO;
        } else if (square == H8) {
            pBoard->info.state[currentMove+1].castleBlack &= ~CAN_CASTLE_OO;
        }
    } else if(movedPiece(m) == BLACK_KING && from(m) == E8) {
        pBoard->info.state[currentMove+1].castleBlack = CANNOT_CASTLE;
    }

    // update the move index and switch colors to play
    pBoard->info.currentMove++;
    assert(pBoard->info.currentMove < MAX_MOVES_PER_GAME);
    pBoard->info.toPlay ^= 1;
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
