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
    pawnMoveBB,
    kingMoveBB,
    knightMoveBB,
    NULL,
    bishopMoveBB,
    rookMoveBB,
    queenMoveBB,
    NULL,
    pawnMoveBB,
    kingMoveBB,
    knightMoveBB,
    NULL,
    bishopMoveBB,
    rookMoveBB,
    queenMoveBB
};

const MoveGenCB timidCB[] = {
    NULL,
    pawnTimidBB,
    kingTimidBB,
    knightTimidBB,
    NULL,
    bishopTimidBB,
    rookTimidBB,
    queenTimidBB,
    NULL,
    pawnTimidBB,
    kingTimidBB,
    knightTimidBB,
    NULL,
    bishopTimidBB,
    rookTimidBB,
    queenTimidBB
};

const MoveGenCB captureCB[] = {
    NULL,
    pawnCaptureBB,
    kingCaptureBB,
    knightCaptureBB,
    NULL,
    bishopCaptureBB,
    rookCaptureBB,
    queenCaptureBB,
    NULL,
    pawnCaptureBB,
    kingCaptureBB,
    knightCaptureBB,
    NULL,
    bishopCaptureBB,
    rookCaptureBB,
    queenCaptureBB
};

bool moveset_contains(MoveSet *moves, Move m) {
    for (int iter = 0; iter < moves->totalMoves; iter++) {
        if (moves->moveList[iter] == m)
            return true;
    }
    return false;
}

static void debugMoves(MoveSet *pMoves) {
    for(int currentMoveIndex = 0; currentMoveIndex < pMoves->totalMoves; currentMoveIndex++) {
        Move toCheck = pMoves->moveList[currentMoveIndex];
        for(int checkIndex = currentMoveIndex+1; checkIndex < pMoves->totalMoves; checkIndex++) {
            if(toCheck == pMoves->moveList[checkIndex]) {
                printf("Found duplicate move:\n");
                printMove(toCheck);
            }
        }
    }
}

static Move extractMove(int piece, int origin, int destination) {
    int occupant = 0;
    // we assume it's occupied by the appropriate color if this function is called
    if (pBoard->position.occupied & BITSET(destination)) {
        occupant = pBoard->position.square[destination];
    }

    return move(occupant, piece, origin, destination);
}

static void extractEnPassant(Board *pBoard, MoveSet *pMoves) {
    BitBoard generated = enPassantBB(pBoard, pBoard->info.toPlay);
    int color = pBoard->info.toPlay;
    int piece = (color == WHITE) ? WHITE_PAWN : BLACK_PAWN;
    int taken = (color == WHITE) ? BLACK_PAWN : WHITE_PAWN;
    int origin = -1, shift = 0;
    while(generated) {
        shift = LSB(generated)+1;
        if (shift < 64) generated >>= shift;
        origin += shift;
        int to = pBoard->info.state[pBoard->info.currentMove].enPassantSquare;
        Move m = moveF((color == BLACK) ? 1 : 0, (color == WHITE) ? 1 : 0, 0, 0, 0, taken, piece, origin, to);
        //printMove(m);
        writeMove(pMoves, m);
    }

    return;
}

void generatePromotions(Board *pBoard, MoveSet *pMoves) {
    static int whitePromotionRank = 6;
    static int blackPromotionRank = 1;
    int color = pBoard->info.toPlay;
    BitBoard timidCandidates;
    BitBoard captureCandidates;
    switch(color) {
        case W:
            {
                // by removing all pawns that naturally cannot promote we speed this up greatly
                timidCandidates = pBoard->position.pieceBB[WHITE_PAWN] & rankBB[whitePromotionRank];
                captureCandidates = timidCandidates;

                int origin = -1, shift = 0, destination = 0;
                BitBoard generatedMoves = 0;
                while(timidCandidates) {
                    shift = LSB(timidCandidates) + 1;
                    if(shift < 64) timidCandidates >>= shift;
                    else timidCandidates = 0;
                    origin += shift;
                    generatedMoves = pawnPromotionBB(pBoard, origin, color);
                    int i = 0;
                    destination = -1;
                    while(generatedMoves) {
                        i = LSB(generatedMoves) + 1;
                        if(i < 64) generatedMoves >>= i;
                        else generatedMoves = 0;
                        destination += i;
                        writeMove(pMoves, moveF(0, 0, 0, 0, WHITE_QUEEN, 0, WHITE_PAWN, origin, destination));
                        writeMove(pMoves, moveF(0, 0, 0, 0, WHITE_ROOK, 0, WHITE_PAWN, origin, destination));
                        writeMove(pMoves, moveF(0, 0, 0, 0, WHITE_BISHOP, 0, WHITE_PAWN, origin, destination));
                        writeMove(pMoves, moveF(0, 0, 0, 0, WHITE_KNIGHT, 0, WHITE_PAWN, origin, destination));
                    }
                }

                origin = -1;
                while(captureCandidates) {
                    shift = LSB(captureCandidates) + 1;
                    if(shift < 64) captureCandidates >>= shift;
                    else captureCandidates = 0;
                    origin += shift;
                    generatedMoves = pawnPromotionCaptureBB(pBoard, origin, color);
                    int i = 0;
                    destination = -1;
                    while(generatedMoves) {
                        i = LSB(generatedMoves) + 1;
                        if(i < 64) generatedMoves >>= i;
                        else generatedMoves = 0;
                        destination += i;
                        writeMove(pMoves, moveF(0, 0, 0, 0, WHITE_QUEEN, pBoard->position.square[destination], WHITE_PAWN, origin, destination));
                        writeMove(pMoves, moveF(0, 0, 0, 0, WHITE_ROOK, pBoard->position.square[destination], WHITE_PAWN, origin, destination));
                        writeMove(pMoves, moveF(0, 0, 0, 0, WHITE_BISHOP, pBoard->position.square[destination], WHITE_PAWN, origin, destination));
                        writeMove(pMoves, moveF(0, 0, 0, 0, WHITE_KNIGHT, pBoard->position.square[destination], WHITE_PAWN, origin, destination));
                    }
                }
            }
        break;
        case B:
            {
                timidCandidates = pBoard->position.pieceBB[BLACK_PAWN] & rankBB[blackPromotionRank];
                captureCandidates = timidCandidates;

                int origin = -1, shift = 0, destination = 0;
                BitBoard generatedMoves = 0;
                while(timidCandidates) {
                    shift = LSB(timidCandidates) + 1;
                    if(shift < 64) timidCandidates >>= shift;
                    else timidCandidates = 0;
                    origin += shift;
                    generatedMoves = pawnPromotionBB(pBoard, origin, color);
                    int i = 0;
                    destination = -1;
                    while(generatedMoves) {
                        i = LSB(generatedMoves) + 1;
                        if(i < 64) generatedMoves >>= i;
                        else generatedMoves = 0;
                        destination += i;
                        writeMove(pMoves, moveF(0, 0, 0, 0, BLACK_QUEEN, 0, BLACK_PAWN, origin, destination));
                        writeMove(pMoves, moveF(0, 0, 0, 0, BLACK_ROOK, 0, BLACK_PAWN, origin, destination));
                        writeMove(pMoves, moveF(0, 0, 0, 0, BLACK_BISHOP, 0, BLACK_PAWN, origin, destination));
                        writeMove(pMoves, moveF(0, 0, 0, 0, BLACK_KNIGHT, 0, BLACK_PAWN, origin, destination));
                    }
                }

                origin = -1;
                while(captureCandidates) {
                    shift = LSB(captureCandidates) + 1;
                    if(shift < 64) captureCandidates >>= shift;
                    else captureCandidates = 0;
                    origin += shift;
                    generatedMoves = pawnPromotionCaptureBB(pBoard, origin, color);
                    int i = 0;
                    destination = -1;
                    while(generatedMoves) {
                        i = LSB(generatedMoves) + 1;
                        if(i < 64) generatedMoves >>= i;
                        else generatedMoves = 0;
                        destination += i;
                        writeMove(pMoves, moveF(0, 0, 0, 0, BLACK_QUEEN, pBoard->position.square[destination], BLACK_PAWN, origin, destination));
                        writeMove(pMoves, moveF(0, 0, 0, 0, BLACK_ROOK, pBoard->position.square[destination], BLACK_PAWN, origin, destination));
                        writeMove(pMoves, moveF(0, 0, 0, 0, BLACK_BISHOP, pBoard->position.square[destination], BLACK_PAWN, origin, destination));
                        writeMove(pMoves, moveF(0, 0, 0, 0, BLACK_KNIGHT, pBoard->position.square[destination], BLACK_PAWN, origin, destination));
                    }
                }
            }
        break;
    }
}

static BitBoard attackedSquaresByPiece(Board *pBoard, int piece, int side) {
    static int whitePromotionRank = 6;
    static int blackPromotionRank = 1;

    // I hate repeating this everywhere... makes you wish for a more expressive language
    BitBoard currentPieces = pBoard->position.pieceBB[piece];
    int origin = -1, shift = 0;
    BitBoard generatedMoves = 0;
    while(currentPieces) {
        shift = LSB(currentPieces)+1;
        if(shift < 64) currentPieces >>= shift;
        else currentPieces = 0;
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
                generatedMoves |= bishopMoveBB(pBoard, origin, side);
            break;
            case WR:
            case BR:
                generatedMoves |= rookMoveBB(pBoard, origin, side);
            break;
            case WK:
            case BK:
                generatedMoves |= kingMove[origin];
            break;
            case WQ:
            case BQ:
                generatedMoves |= queenMoveBB(pBoard, origin, side);
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
BitBoard attackedSquares(Board *pBoard, int side) {
    BitBoard attacked = 0;
    int piece, endpiece;
    if(side == BLACK) {
        piece = BP;
        endpiece = BQ;
    } else {
        piece = WP;
        endpiece = WQ;
    }

    for(; piece <= endpiece; piece++) {
        attacked |= attackedSquaresByPiece(pBoard, piece, side);
    }
    return attacked;
}

void generateCastle(Board *pBoard, MoveSet *pMoves) {
    U64 WhiteKingsideCheck = BITSET(E1) | BITSET(F1) | BITSET(G1);
    U64 WhiteKingsideClearance = BITSET(F1) | BITSET(G1);
    U64 BlackKingsideCheck = BITSET(E8) | BITSET(F8) | BITSET(G8);
    U64 BlackKingsideClearance = BITSET(F8) | BITSET(G8);

    U64 WhiteQueensideCheck = BITSET(E1) | BITSET(D1) | BITSET(C1);
    U64 WhiteQueensideClearance =  BITSET(D1) | BITSET(C1) | BITSET(B1);
    U64 BlackQueensideCheck = BITSET(E8) | BITSET(D8) | BITSET(C8);
    U64 BlackQueensideClearance = BITSET(D8) | BITSET(C8) | BITSET(B8);


    switch(pBoard->info.toPlay) {
        case W:
            {
                // check if white can castle
                int castleWhite = pBoard->info.state[pBoard->info.currentMove].castleWhite;
                if(castleWhite) {
                    BitBoard blackAttacks = attackedSquares(pBoard, BLACK);
                    if(castleWhite & CAN_CASTLE_OO) {
                        if(!(pBoard->position.occupied & WhiteKingsideClearance) && !(blackAttacks & WhiteKingsideCheck))
                            writeMove(pMoves, moveF(0, 0, 0, 1, 0, 0, WHITE_KING, E1, G1));
                    }
                    if(castleWhite & CAN_CASTLE_OOO) {
                        if(!(pBoard->position.occupied & WhiteQueensideClearance) && !(blackAttacks & WhiteQueensideCheck))
                            writeMove(pMoves, moveF(0, 0, 0, 1, 0, 0, WHITE_KING, E1, C1));
                    }
                }
            }
        break;
        case B:
            {
                // check if black can castle
                int castleBlack = pBoard->info.state[pBoard->info.currentMove].castleBlack;
                if(castleBlack) {
                    BitBoard whiteAttacks = attackedSquares(pBoard, WHITE);
                    if(castleBlack & CAN_CASTLE_OO) {
                        if(!(pBoard->position.occupied & BlackKingsideClearance) && !(whiteAttacks & BlackKingsideCheck))
                            writeMove(pMoves, moveF(0, 0, 1, 0, 0, 0, BLACK_KING, E8, G8));
                    }
                    if(castleBlack & CAN_CASTLE_OOO) {
                        if(!(pBoard->position.occupied & BlackQueensideClearance) && !(whiteAttacks & BlackQueensideCheck))
                            writeMove(pMoves, moveF(0, 0, 1, 0, 0, 0, BLACK_KING, E8, C8));
                    }
                }
            }
        break;
    }
}

void generateAgnostic(Board *pBoard, int color, BitBoard currentPieces,
int piece, MoveSet *pMoves, BitBoard (*moveGen)(Board *pBoard, UCHAR origin, int color)) {
    int origin = -1, shift = 0, destination = 0;
    int count = 0;
    BitBoard generatedMoves = 0;
    while(currentPieces) {
        shift = LSB(currentPieces)+1;
        if (shift < 64) currentPieces >>= shift;
        else currentPieces = 0;
        origin += shift;
        generatedMoves = moveGen(pBoard, origin, color);
        //if(generatedMoves) printBitMap(generatedMoves);
        int i = 0;
        destination = -1;
        while(generatedMoves) {
            count += 1;
            i = LSB(generatedMoves)+1;
            if (i < 64) generatedMoves >>= i;
            else generatedMoves = 0;
            destination += i;
            writeMove(pMoves, extractMove(piece, origin, destination));
        }
    }

    return;
}

void generateTimid(Board *pBoard, MoveSet *pMoves) {
    BitBoard currentPieces = 0;
    int color = pBoard->info.toPlay;
    int piece, endpiece;

    if (color == BLACK) {
        piece = BP;
        endpiece = BQ;
    } else {
        piece = WP;
        endpiece = WQ;
    }

    for (; piece <= endpiece; piece++) {
        if (!isPiece(piece)) continue;
        currentPieces = pBoard->position.pieceBB[piece];
        generateAgnostic(pBoard, color, currentPieces, piece, pMoves, timidCB[piece]);
    }

    generateCastle(pBoard, pMoves);

    return;
}

void generateCapture(Board *pBoard, MoveSet *pMoves) {
    BitBoard currentPieces = 0;
    int color = pBoard->info.toPlay;
    int piece, endpiece;

    if (color == BLACK) {
        piece = BP;
        endpiece = BQ;
    } else {
        piece = WP;
        endpiece = WQ;
    }

    for (; piece <= endpiece; piece++) {
        if (!isPiece(piece)) continue;
        currentPieces = pBoard->position.pieceBB[piece];
        generateAgnostic(pBoard, color, currentPieces, piece, pMoves, captureCB[piece]);
    }

    // handle enPassant
    extractEnPassant(pBoard, pMoves);

    return;
}

void generateMove(Board *pBoard, MoveSet *pMoves) {
    BitBoard currentPieces = 0;
    int color = pBoard->info.toPlay;
    int piece, endpiece;

    if (color == BLACK) {
        piece = BP;
        endpiece = BQ;
    } else {
        piece = WP;
        endpiece = WQ;
    }

    for (; piece <= endpiece; piece++) {
        if (!isPiece(piece)) continue;
        currentPieces = pBoard->position.pieceBB[piece];
        generateAgnostic(pBoard, color, currentPieces, piece, pMoves, moveCB[piece]);
    }

    // promotions
    generatePromotions(pBoard, pMoves);

    // castling
    generateCastle(pBoard, pMoves);

    // handle enPassant
    extractEnPassant(pBoard, pMoves);

    return;
}

BitBoard generatePieceAttacks(Board *pBoard, int color, BitBoard currentPieces,
int piece) {
    int origin = -1, shift = 0;
    BitBoard generatedMoves = 0;
    while(currentPieces) {
        shift = LSB(currentPieces)+1;
        if (shift < 64) currentPieces >>= shift;
        else currentPieces = 0;
        origin += shift;
        generatedMoves |= (*captureCB[piece])(pBoard, origin, color);
        if(piece % 8 == WHITE_PAWN) {
            generatedMoves |= pawnPromotionCaptureBB(pBoard, origin, color);
        }
    }
    return generatedMoves;
}

// attacks ON a particular color (not BY the color)
BitBoard generateAllAttacks(Board *pBoard, int color) {
    BitBoard attacks = 0;
    BitBoard currentPieces = 0;
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
        if (!isPiece(piece)) continue;
        currentPieces = pBoard->position.pieceBB[piece];
        attacks |= generatePieceAttacks(pBoard, color, currentPieces, piece);
    }
    return attacks;
}

BitBoard attacks(Board *pBoard, int attackeeIndex, int side) {
    return BITSET(attackeeIndex) & generateAllAttacks(pBoard, side);
}

bool checks(Board *pBoard, int side) {
    return attacks(pBoard, pBoard->position.kings[side], side) != 0;
}


//===============UNFINISHED===================

void generateCheck(Board *pBoard, MoveSet *pMoves) {

    return;
}

void initializeMoveSet(Board *pBoard, MoveSet *pMoves) {
    resetMoveSet(pMoves);
    printf("About to generate moves.\n");
    generateCapture(pBoard, pMoves);
    generateTimid(pBoard, pMoves);
    printf("Generated moves.\n");
    while(pMoves->moveIter < pMoves->totalMoves)
        printMove(nextMove(pMoves));
}

void initializeMoveSetQuiet(Board *pBoard, MoveSet *pMoves) {
    generateMove(pBoard, pMoves);
}

void resetMoveSet(MoveSet *pMoves) {
    pMoves->moveIter = 0;
    pMoves->currentMoveIndex = 0;
    pMoves->timidIndex = 0;
    pMoves->totalMoves = 0;

    pMoves->killerIter = 0;
    pMoves->totalKillers = 0;
    pMoves->currentKillerIndex = 0;
}

Move nextMove(MoveSet *pMoves) {
    assert(pMoves->moveIter < pMoves->totalMoves);
    return pMoves->moveList[pMoves->moveIter++];
}

void writeMove(MoveSet *pMoves, Move m) {
    assert(pMoves->currentMoveIndex + 1 < MAX_MOVES_PER_PLY);
    pMoves->totalMoves++;
    pMoves->moveList[pMoves->currentMoveIndex++] = m;
    if(capturedPiece(m)) pMoves->timidIndex++;
}

void writeKiller(MoveSet *pMoves, Move killer) {
    assert(pMoves->currentKillerIndex + 1 < MAX_KILLERS_PER_PLY);
    pMoves->totalKillers++;
    pMoves->killerList[pMoves->currentKillerIndex++] = killer;
}
