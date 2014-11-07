#include <assert.h>
#include <stdio.h>

#include "board.h"
#include "../bit.h"
#include "../defines.h"
#include "../extglobals.h"
#include "../move/move.h"
#include "../move/movegen.h"

void resetBoard(Board *pBoard) {
    for(int i = 0; i < 64; i++) pBoard->position.square[i] = EMPTY;
    for(int i = 0; i < MAX_MOVES_PER_GAME; i++) {
        pBoard->info.state[i].enPassantSquare = INVALID_SQUARE;
    }

    pBoard->position.square[A1] = WHITE_ROOK;
    pBoard->position.square[B1] = WHITE_KNIGHT;
    pBoard->position.square[C1] = WHITE_BISHOP;
    pBoard->position.square[D1] = WHITE_QUEEN;
    pBoard->position.square[E1] = WHITE_KING;
    pBoard->position.square[F1] = WHITE_BISHOP;
    pBoard->position.square[G1] = WHITE_KNIGHT;
    pBoard->position.square[H1] = WHITE_ROOK;
    pBoard->position.square[A2] = WHITE_PAWN;
    pBoard->position.square[B2] = WHITE_PAWN;
    pBoard->position.square[C2] = WHITE_PAWN;
    pBoard->position.square[D2] = WHITE_PAWN;
    pBoard->position.square[E2] = WHITE_PAWN;
    pBoard->position.square[F2] = WHITE_PAWN;
    pBoard->position.square[G2] = WHITE_PAWN;
    pBoard->position.square[H2] = WHITE_PAWN;

    pBoard->position.square[A8] = BLACK_ROOK;
    pBoard->position.square[B8] = BLACK_KNIGHT;
    pBoard->position.square[C8] = BLACK_BISHOP;
    pBoard->position.square[D8] = BLACK_QUEEN;
    pBoard->position.square[E8] = BLACK_KING;
    pBoard->position.square[F8] = BLACK_BISHOP;
    pBoard->position.square[G8] = BLACK_KNIGHT;
    pBoard->position.square[H8] = BLACK_ROOK;
    pBoard->position.square[A7] = BLACK_PAWN;
    pBoard->position.square[B7] = BLACK_PAWN;
    pBoard->position.square[C7] = BLACK_PAWN;
    pBoard->position.square[D7] = BLACK_PAWN;
    pBoard->position.square[E7] = BLACK_PAWN;
    pBoard->position.square[F7] = BLACK_PAWN;
    pBoard->position.square[G7] = BLACK_PAWN;
    pBoard->position.square[H7] = BLACK_PAWN;

    pBoard->info.displayRotated = false;
    pBoard->info.currentMove = 0;

    initBoardFromSquares(pBoard, WHITE, 0, CAN_CASTLE, CAN_CASTLE, INVALID_SQUARE, 0);
    return;
}

U64 fullZobristKey(Board *pBoard) {
    U64 key = 0;
    for (int sq_idx = 0; sq_idx < 64; sq_idx++) {
        UCHAR at_sq = pBoard->position.square[sq_idx];
        if(isPiece(at_sq)) {
            key ^= piece_keys[at_sq][sq_idx];
        }
    }
    MoveInfo m_info = pBoard->info.state[pBoard->info.currentMove];
    key ^= castling_rights_keys[(m_info.castleWhite + 4*m_info.castleBlack)];

    if (pBoard->info.toPlay == W) key ^= white_to_move_key;

    int ep_square = m_info.enPassantSquare;
    if (ep_square != INVALID_SQUARE) key ^= ep_file_keys[FILE(ep_square)];

    return key;
}

void initBoardFromSquares(Board* pBoard, unsigned char toPlay, int staleMoves, int castleW,
int castleB, int enPassantSquare, int turnCount) {

    resetBB(pBoard);
    updateBBFromSquares(pBoard);
    updateAggregateBB(pBoard);
    updateMaterialFromBB(pBoard);
    updateKingsFromBB(pBoard);
    updatePieceCountsFromBB(pBoard);

    pBoard->info.currentMove = turnCount;
    pBoard->info.toPlay = toPlay;
    pBoard->info.state[pBoard->info.currentMove].castleWhite = castleW;
    pBoard->info.state[pBoard->info.currentMove].castleBlack = castleB;
    pBoard->info.state[pBoard->info.currentMove].enPassantSquare = enPassantSquare;
    pBoard->info.state[pBoard->info.currentMove].staleMoves = staleMoves;

    // rebuild Zobrist key
    pBoard->info.state[pBoard->info.currentMove]._zobrist_key = fullZobristKey(pBoard);

    return;
}

void displayBoard(Board *pBoard) {
    const char *header  = "   A     B     C     D     E     F     G     H   ";
    const char *reverse = "   H     G     F     E     D     C     B     A   ";
    const char *divider = "+-----+-----+-----+-----+-----+-----+-----+-----+";

    printf("\n\t%s\n", pBoard->info.toPlay ? reverse : header);

    int i, j;
    for(i = 0; i < 8; i++) {
        printf("\t%s\n\t", divider);
        for(j = 0; j < 8; j++) {
            switch(pBoard->info.toPlay) {
                case W:
                    printf("| %s ", PIECE_NAMES[pBoard->position.square[SQ(8-i, j+1)]]);
                break;
                case B:
                    printf("| %s ", PIECE_NAMES[pBoard->position.square[SQ(i+1, 8-j)]]);
                break;
            }
        }
        printf("|  %d\n", pBoard->info.toPlay ? i+1 : 8-i);
    }
    printf("\t%s\n\n", divider);
    printf("Material counts:\n\tWhite: %d\n\tBlack: %d\n", pBoard->info.whiteMaterial, pBoard->info.blackMaterial);
    if(pBoard->info.state[pBoard->info.currentMove].castleWhite == CANNOT_CASTLE) printf("White cannot castle.\n");
    else {
        printf("White can castle");
        if(pBoard->info.state[pBoard->info.currentMove].castleWhite == CAN_CASTLE_OOO)
            printf(" queenside");
        if(pBoard->info.state[pBoard->info.currentMove].castleWhite == CAN_CASTLE_OO)
            printf(" kingside");
        printf(".\n");
    }
    if(pBoard->info.state[pBoard->info.currentMove].castleBlack == CANNOT_CASTLE) printf("Black cannot castle.\n");
    else {
        printf("Black can castle");
        if(pBoard->info.state[pBoard->info.currentMove].castleBlack == CAN_CASTLE_OOO)
            printf(" queenside");
        if(pBoard->info.state[pBoard->info.currentMove].castleBlack == CAN_CASTLE_OO)
            printf(" kingside");
        printf(".\n\n");
    }
    if(checks(pBoard, WHITE)) {
        printf("White is in check.\n");
    } else if(checks(pBoard, BLACK)) {
        printf("Black is in check.\n\n");
    }
    int epSquare = pBoard->info.state[pBoard->info.currentMove].enPassantSquare;
    if(epSquare == INVALID_SQUARE)
        printf("EP Square : --\n");
    else
        printf("EP Square : %s\n", SQUARENAME[epSquare]);
}

void rotateBoard(Board *pBoard) {
    pBoard->info.displayRotated = !pBoard->info.displayRotated;
}

bool debugBoard(Board *pBoard) {
    // we first check if the board is in a consistent state, and report to the user
    // certain problems cannot be found at runtime however, and require prior knowledge of the
    // state of the board, for instance in the case of castling.
    bool boardConsistent = true;
    for(int i = 0; i < 64; i++) {
        UCHAR currentPiece = pBoard->position.square[i];
        if(currentPiece != EMPTY) {
            if(!(BITSET(i) & pBoard->position.pieceBB[currentPiece])) {
                boardConsistent = false;
                printf("The array of squares reports a %s at %s,"
                       " but the bitboard is empty.\n",
                       PIECE_NAMES_FULL[currentPiece], SQUARENAME[i]);
            }
        }
    }

    // material checks
    int materialB = pBoard->info.blackMaterial;
    int materialW = pBoard->info.whiteMaterial;

    updateMaterialFromBB(pBoard);

    if (materialB != pBoard->info.blackMaterial ||
        materialW != pBoard->info.whiteMaterial) {
        printf("Material values are not consistent. WP%d WA%d BP%d BA%d",
               materialW, pBoard->info.whiteMaterial,
               materialB, pBoard->info.blackMaterial);
    }
    //assert(materialB == pBoard->info.blackMaterial);
    //assert(materialW == pBoard->info.whiteMaterial);

    // array of kings
    if(pBoard->position.pieces[WHITE_KING] == 1) {
        if(!(BITSET(pBoard->position.kings[BLACK]) & pBoard->position.pieceBB[BLACK_KING])) {
            boardConsistent = false;
            printf("The array of kings reports a black king at %s, but the bitboard does not.\n", SQUARENAME[pBoard->position.kings[BLACK]]);
        }
    }

    if(pBoard->position.pieces[BLACK_KING] == 1) {
        if(!(BITSET(pBoard->position.kings[WHITE]) & pBoard->position.pieceBB[WHITE_KING])) {
            boardConsistent = false;
            printf("The array of kings reports a white king at %s, but the bitboard does not.\n", SQUARENAME[pBoard->position.kings[WHITE]]);
        }
    }

    // array of pieces
    // no real check needed here at the moment, though one could be added for material consistency

    // bitboards
    for(int currentPieceType = 0; currentPieceType < TOTAL_PIECE_TYPES; currentPieceType++) {
        if(isPiece(currentPieceType)) {
            BitBoard currentBitBoard = pBoard->position.pieceBB[currentPieceType];
            int index = -1, shift = 0;
            while(currentBitBoard) {
                shift = LSB(currentBitBoard)+1;
                if (shift < 64) currentBitBoard >>= shift;
                else currentBitBoard = 0;
                index += shift;
                if(pBoard->position.square[index] != currentPieceType) {
                    boardConsistent = false;
                    printf("The bitboards report a %s at %s, but the array of pieces does not.\n", PIECE_NAMES_FULL[currentPieceType], SQUARENAME[index]);
                }
            }
        }
    }

    // rudimentary castling checks
    switch(pBoard->info.state[pBoard->info.currentMove].castleWhite) {
        break;
        case CAN_CASTLE_OO:
            if(pBoard->position.square[E1] != WHITE_KING || pBoard->position.square[H1] != WHITE_ROOK) {
                boardConsistent = false;
                printf("The board info incorrectly reports that white can castle kingside.");
            }
        break;
        case CAN_CASTLE_OOO:
            if(pBoard->position.square[E1] != WHITE_KING || pBoard->position.square[A1] != WHITE_ROOK) {
                boardConsistent = false;
                printf("The board info incorrectly reports that white can castle queenside.");
            }
        break;
        case CAN_CASTLE_OO | CAN_CASTLE_OOO:
            if(pBoard->position.square[E1] != WHITE_KING || pBoard->position.square[A1] != WHITE_ROOK || pBoard->position.square[H1] != WHITE_ROOK) {
                boardConsistent = false;
                printf("The board info incorrectly reports that white can castle kingside or queenside.");
            }
        break;
    }

    switch(pBoard->info.state[pBoard->info.currentMove].castleBlack) {
        case CAN_CASTLE_OO:
            if(pBoard->position.square[E8] != BLACK_KING || pBoard->position.square[H8] != BLACK_ROOK) {
                boardConsistent = false;
                printf("The board info incorrectly reports that black can castle kingside.");
            }
        break;
        case CAN_CASTLE_OOO:
            if(pBoard->position.square[E8] != BLACK_KING || pBoard->position.square[A8] != BLACK_ROOK) {
                boardConsistent = false;
                printf("The board info incorrectly reports that black can castle queenside.");
            }
        break;
        case CAN_CASTLE_OO | CAN_CASTLE_OOO:
            if(pBoard->position.square[E8] != BLACK_KING || pBoard->position.square[A8] != BLACK_ROOK || pBoard->position.square[H8] != BLACK_ROOK) {
                boardConsistent = false;
                printf("The board info incorrectly reports that black can castle kingside or queenside.");
            }
        break;
    }

    // if it is not, we will print out a diagnostic
    if(!boardConsistent) {
        displayBoard(pBoard);
        // other info would be nice too, but this is fine for the moment
        // leave a breakpoint for debugging, not that much can be found here...
        #ifdef __clang__
        asm("int3");
        #elif (__GNUC__ || __cplusplus)
        __asm__("int3");
        #endif
    }
    return boardConsistent;
}

//---------------------MAKE FUNCTIONS------------------------
void setEmptyAt(Board *pBoard, UCHAR index, UCHAR lastOccupant) {
    //DOES NOT UPDATE ANY MATERIAL COUNTS!!!
    pBoard->position.pieceBB[lastOccupant] &= ~BITSET(index);
    pBoard->position.occupied &= ~BITSET(index);
    pBoard->position.square[index] = EMPTY;
    if(lastOccupant == WHITE_KING || lastOccupant == BLACK_KING) {
        pBoard->position.kings[color(lastOccupant)] = INVALID_SQUARE;
    }
    if(color(lastOccupant) == B) {
        pBoard->position.blackOccupied &= ~BITSET(index);
    } else {
        pBoard->position.whiteOccupied &= ~BITSET(index);
    }
}

void setPieceAt(Board *pBoard, UCHAR index, UCHAR movedPiece, UCHAR capturedPiece) {
    if(capturedPiece) {
        setEmptyAt(pBoard, index, capturedPiece);
    }
    pBoard->position.pieceBB[movedPiece] |= BITSET(index);
    pBoard->position.occupied |= BITSET(index);
    pBoard->position.square[index] = movedPiece;
    if(movedPiece == WHITE_KING || movedPiece == BLACK_KING) {
        pBoard->position.kings[color(movedPiece)] = index;
    }
    if(color(movedPiece) == B) {
        pBoard->position.blackOccupied |= BITSET(index);
    } else {
        pBoard->position.whiteOccupied |= BITSET(index);
    }
}

void updateEnPassantSquare(Board *pBoard, int index) {
    pBoard->info.state[pBoard->info.currentMove].enPassantSquare = index;
}

void enPassant(Board *pBoard, int color) {
    // the color should be the color of the *capturing* pawn
    // find where the en passant will take place and
    // make sure the board is updated so that no further enpassant is possible
    int index = pBoard->info.state[pBoard->info.currentMove].enPassantSquare;

    // the piece will either be a white pawn or a black pawn, but
    // we can get it either way here
    if(color == WHITE) {
        setEmptyAt(pBoard, index - 8, BLACK_PAWN);
    } else {
        setEmptyAt(pBoard, index + 8, WHITE_PAWN);
    }
    return;
}

void unPassant(Board *pBoard, int index, int color) {
    // a horrible corruption of the French language... mon Dieu!
    if(color == BLACK) {
        setPieceAt(pBoard, index + 8, WHITE_PAWN, NO_PIECE);
    } else {
        setPieceAt(pBoard, index - 8, BLACK_PAWN, NO_PIECE);
    }
}

void castle(Board *pBoard, UCHAR index, UCHAR whichKing) {
    switch(whichKing) {
        case W:
            assert(index == C1 || index == G1);
            if(index == C1) {
                setEmptyAt(pBoard, E1, WHITE_KING);
                setPieceAt(pBoard, C1, WHITE_KING, 0);
                setEmptyAt(pBoard, A1, WHITE_ROOK);
                setPieceAt(pBoard, D1, WHITE_ROOK, 0);
            } else {
                setEmptyAt(pBoard, E1, WHITE_KING);
                setPieceAt(pBoard, G1, WHITE_KING, 0);
                setEmptyAt(pBoard, H1, WHITE_ROOK);
                setPieceAt(pBoard, F1, WHITE_ROOK, 0);
            }
        break;
        case B:
            assert(index == C8 || index == G8);
            if(index == C8) {
                setEmptyAt(pBoard, E8, BLACK_KING);
                setPieceAt(pBoard, C8, BLACK_KING, 0);
                setEmptyAt(pBoard, A8, BLACK_ROOK);
                setPieceAt(pBoard, D8, BLACK_ROOK, 0);
            } else {
                setEmptyAt(pBoard, E8, BLACK_KING);
                setPieceAt(pBoard, G8, BLACK_KING, 0);
                setEmptyAt(pBoard, H8, BLACK_ROOK);
                setPieceAt(pBoard, F8, BLACK_ROOK, 0);
            }
        break;
    }
    return;
}

void unCastle(Board *pBoard, UCHAR index, UCHAR whichKing) {
    switch(whichKing) {
        case W:
            assert(index == C1 || index == G1);
            if(index == C1) {
                setEmptyAt(pBoard, C1, WHITE_KING);
                setPieceAt(pBoard, E1, WHITE_KING, 0);
                setEmptyAt(pBoard, D1, WHITE_ROOK);
                setPieceAt(pBoard, A1, WHITE_ROOK, 0);
            } else {
                setEmptyAt(pBoard, G1, WHITE_KING);
                setPieceAt(pBoard, E1, WHITE_KING, 0);
                setEmptyAt(pBoard, F1, WHITE_ROOK);
                setPieceAt(pBoard, H1, WHITE_ROOK, 0);
            }
        break;
        case B:
            assert(index == C8 || index == G8);
            if(index == C8) {
                setEmptyAt(pBoard, C8, BLACK_KING);
                setPieceAt(pBoard, E8, BLACK_KING, 0);
                setEmptyAt(pBoard, D8, BLACK_ROOK);
                setPieceAt(pBoard, A8, BLACK_ROOK, 0);
            } else {
                setEmptyAt(pBoard, G8, BLACK_KING);
                setPieceAt(pBoard, E8, BLACK_KING, 0);
                setEmptyAt(pBoard, F8, BLACK_ROOK);
                setPieceAt(pBoard, H8, BLACK_ROOK, 0);
            }
        break;
    }
    return;
}

void doPromote(Board *pBoard, UCHAR index, UCHAR whichPromote) {
    switch(color(whichPromote)) {
        case W:
            setEmptyAt(pBoard, index, WHITE_PAWN);
            setPieceAt(pBoard, index, whichPromote, 0);
        break;
        case B:
            setEmptyAt(pBoard, index, BLACK_PAWN);
            setPieceAt(pBoard, index, whichPromote, 0);
        break;
    }
    return;
}

void unPromote(Board *pBoard, UCHAR index) {
    switch(color(pBoard->position.square[index])) {
        case W:
            setEmptyAt(pBoard, index, pBoard->position.square[index]);
            setPieceAt(pBoard, index, WHITE_PAWN, 0);
        break;
        case B:
            setEmptyAt(pBoard, index, pBoard->position.square[index]);
            setPieceAt(pBoard, index, BLACK_PAWN, 0);
        break;
    }
    return;
}

void addMaterial(Board *pBoard, UCHAR piece) {
    pBoard->position.pieces[piece]++;
    if(color(piece) == WHITE) {
        pBoard->position.totalWhite++;
    } else {
        pBoard->position.totalBlack++;
    }
    pBoard->position.totalPieces++;
    if(color(piece) == BLACK) {
        pBoard->info.blackMaterial += PIECEVALUES[piece];
    } else {
        pBoard->info.whiteMaterial += PIECEVALUES[piece];
    }
    pBoard->info.material = pBoard->info.whiteMaterial - pBoard->info.blackMaterial;
    return;
}

void removeMaterial(Board *pBoard, UCHAR piece) {
    pBoard->position.pieces[piece]--;
    if(color(piece) == WHITE) {
        pBoard->position.totalWhite--;
    } else {
        pBoard->position.totalBlack--;
    }
    pBoard->position.totalPieces--;
    if(color(piece) == BLACK) {
        pBoard->info.blackMaterial -= PIECEVALUES[piece];
    } else {
        pBoard->info.whiteMaterial -= PIECEVALUES[piece];
    }
    pBoard->info.material = pBoard->info.whiteMaterial - pBoard->info.blackMaterial;
    return;
}

//---------------------AUX FUNCTIONS-------------------------
void updatePieceCountsFromBB(Board *pBoard) {
    // Reset everything
    for(int currentPieceType = 0; currentPieceType < TOTAL_PIECE_TYPES; currentPieceType++) {
        pBoard->position.pieces[currentPieceType] = 0;
    }

    pBoard->position.totalWhite = 0;
    pBoard->position.totalBlack = 0;

    // now iterate across the squares in order to update piece counts
    for(int currentSquare = 0; currentSquare < 64; currentSquare++) {
        UCHAR currentPiece = pBoard->position.square[currentSquare];
        if(isPiece(currentPiece)) {
            int color = color(currentPiece);
            pBoard->position.pieces[currentPiece]++;
            if(color == WHITE) {
                pBoard->position.totalWhite++;
            } else {
                pBoard->position.totalBlack++;
            }
        }
    }

    pBoard->position.totalPieces = pBoard->position.totalWhite + pBoard->position.totalBlack;
    return;
}

void updateKingsFromBB(Board *pBoard) {
    assert(countBits(pBoard->position.pieceBB[WHITE_KING]) == 1
            && countBits(pBoard->position.pieceBB[BLACK_KING]) == 1);
    pBoard->position.kings[WHITE] = LSB(pBoard->position.pieceBB[WHITE_KING]);
    pBoard->position.kings[BLACK] = LSB(pBoard->position.pieceBB[BLACK_KING]);
    return;
}

void resetBB(Board *pBoard) {
    for(int currentPieceType = 0; currentPieceType < TOTAL_PIECE_TYPES; currentPieceType++) {
        pBoard->position.pieceBB[currentPieceType] = 0;
    }

    pBoard->position.whiteOccupied = 0;
    pBoard->position.blackOccupied = 0;
    pBoard->position.occupied = 0;
    return;
}

void updateAggregateBB(Board *pBoard) {
    pBoard->position.whiteOccupied = pBoard->position.pieceBB[WHITE_PAWN] |
                                                                    pBoard->position.pieceBB[WHITE_KING] |
                                                                    pBoard->position.pieceBB[WHITE_QUEEN] |
                                                                    pBoard->position.pieceBB[WHITE_BISHOP] |
                                                                    pBoard->position.pieceBB[WHITE_KNIGHT] |
                                                                    pBoard->position.pieceBB[WHITE_ROOK];

    pBoard->position.blackOccupied = pBoard->position.pieceBB[BLACK_PAWN] |
                                                                    pBoard->position.pieceBB[BLACK_KING] |
                                                                    pBoard->position.pieceBB[BLACK_QUEEN] |
                                                                    pBoard->position.pieceBB[BLACK_BISHOP] |
                                                                    pBoard->position.pieceBB[BLACK_KNIGHT] |
                                                                    pBoard->position.pieceBB[BLACK_ROOK];

    pBoard->position.occupied = pBoard->position.whiteOccupied |
                                                            pBoard->position.blackOccupied;
    return;
}

void updateBBFromSquares(Board *pBoard) {
    resetBB(pBoard);

    for(int currentSquare = 0; currentSquare < 64; currentSquare++) {
        UCHAR currentPiece = pBoard->position.square[currentSquare];
        if(isPiece(currentPiece)) {
            pBoard->position.pieceBB[currentPiece] |= BITSET(currentSquare);
        }
    }

    updateAggregateBB(pBoard);
    return;
}

void updateMaterialFromBB(Board *pBoard) {
    pBoard->info.whiteMaterial =
        countBits(pBoard->position.pieceBB[WHITE_PAWN]) * PAWN_VALUE
      + countBits(pBoard->position.pieceBB[WHITE_BISHOP]) * BISHOP_VALUE
      + countBits(pBoard->position.pieceBB[WHITE_KNIGHT]) * KNIGHT_VALUE
      + countBits(pBoard->position.pieceBB[WHITE_ROOK]) * ROOK_VALUE
      + countBits(pBoard->position.pieceBB[WHITE_QUEEN]) * QUEEN_VALUE;

    pBoard->info.blackMaterial =
        countBits(pBoard->position.pieceBB[BLACK_PAWN]) * PAWN_VALUE
      + countBits(pBoard->position.pieceBB[BLACK_BISHOP]) * BISHOP_VALUE
      + countBits(pBoard->position.pieceBB[BLACK_KNIGHT]) * KNIGHT_VALUE
      + countBits(pBoard->position.pieceBB[BLACK_ROOK]) * ROOK_VALUE
      + countBits(pBoard->position.pieceBB[BLACK_QUEEN]) * QUEEN_VALUE;

    pBoard->info.material =
        pBoard->info.whiteMaterial
      - pBoard->info.blackMaterial;

    return;
}
