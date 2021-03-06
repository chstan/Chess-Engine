#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

#include "notation.h"
#include "../move/movegen.h"
#include "../move/movegenhelpers.h"
#include "../move/move.h"
#include "../defines.h"
#include "../extglobals.h"
#include "../bit.h"

extern MoveGenCB captureCB[];

Move queenSide(unsigned char color) {
    if(color == W) {
        return (move_full(0, 0, 0, 1, 0, 0, WHITE_KING, E1, C1));
    } else {
        return (move_full(0, 0, 1, 0, 0, 0, BLACK_KING, E8, C8));
    }
    return 0;
}

Move kingSide(unsigned char color) {
    if(color == W) {
        return move_full(0, 0, 0, 1, 0, 0, WHITE_KING, E1, G1);
    } else {
        return move_full(0, 0, 1, 0, 0, 0, BLACK_KING, E8, G8);
    }
    return 0;
}

int get_square(char *sq) {
    //if(strlen(sq) != 2) return INVALID_SQUARE;
    assert(strlen(sq) == 2);
    char file = sq[0];
    char rank = sq[1];
    return SQ(rank - '1'+1, file - 'a'+1);
}

char *extractFile(U64 bits) {
    char *file = malloc(2);
    if(!file) return NULL;
    file[0] = 'a' + (LSB(bits) % 8);
    file[1] = '\0';
    return file;
}

char *extractRank(U64 bits) {
    char *rank = malloc(2);
    if(!rank) return NULL;
    rank[0] = '1' + (LSB(bits) - (LSB(bits) % 8))/8;
    rank[1] = '\0';
    return rank;
}

char getPieceName(UCHAR piece) {
    switch(piece) {
        case WK:
        case BK:
            return 'K';
        break;
        case WQ:
        case BQ:
            return 'Q';
        break;
        case WR:
        case BR:
            return 'R';
        break;
        case WN:
        case BN:
            return 'N';
        break;
        case WB:
        case BB:
            return 'B';
        break;
    }
    printf("Unknown piece received %d\n", piece);
    return '?';
}

char *pawnMoveToNotation(Board *p_board, Move m) {
    UCHAR piece = moved_piece(m);
    int color = color(piece);
    U64 originCandidates;

    int length = 3;

    char *disambiguationString = NULL;

    if(captured_piece(m)) {
        length++; // we will need another character in the string to denote the capture
        originCandidates = p_board->position.piece_BB[piece] & (*captureCB[piece])(p_board, to(m), other_color(color));
        if(count_bits(originCandidates) != 1) {
            length++; // and another to disambiguate
            // we need only the file, as there are only ever two spots a pawn can capture from!
            disambiguationString = extractFile(originCandidates);
        }
    }
    // note that there is no need to disambiguate if the move was not a capture

    bool promote = false;
    char promoteChar;
    if(promote(m)) {
        length++;
        promoteChar = getPieceName(promote(m));
    }

    char *notation = malloc(length);
    memset(notation, 0, length);
    int idx = 0;
    if(disambiguationString) {
        strcpy(notation, disambiguationString);
        idx++;
        free(disambiguationString);
    }
    if(captured_piece(m)) {
        notation[idx] = 'x';
        idx++;
        notation[idx] = '\0';
    }
    strcat(notation, SQUARENAME[to(m)]);
    if(promote) {
        notation[length-2] = promoteChar;
    }
    notation[length-1] = '\0';
    return notation;
}

char *castleMoveToNotation(Move m) {
    if(from(m) == E1) {
        // white
        if(to(m) == C1)
            return strdup("O-O-O");
        else
            return strdup("O-O");
    } else {
        if(to(m) == C8)
            return strdup("O-O-O");
        else
            return strdup("O-O");
    }
    return NULL;
}

char *disambiguateOriginFromMove(Board *p_board, UCHAR piece, int destination, Move m) {
    U64 originCandidates = p_board->position.piece_BB[piece] & (*captureCB[piece])(p_board, destination, (color(piece) == WHITE) ? BLACK : WHITE);

    // algebraic chess notation requires we disambiguate by file, then rank, then both if needed
    // try disambiguate from file (a-h)
    if(shared_file(originCandidates))
        return extractRank(BITSET(from(m)));

    // try disambiguate from rank (1-8)
    if(shared_rank(originCandidates))
        return extractFile(BITSET(from(m)));

    // we have to return both the file and rank, which is simple enough
    return strdup(SQUARENAME[from(m)]);
}

char *move_to_notation(Board *p_board, Move m) {
    if(white_castle(m) || black_castle(m))
        return castleMoveToNotation(m);

    UCHAR piece = moved_piece(m);
    int color = color(piece);
    if(piece == WHITE_PAWN || piece == BLACK_PAWN)
        return pawnMoveToNotation(p_board, m);

    char pieceChar = getPieceName(piece);
    int dest = to(m);

    char *disambiguationString = NULL;
    U64 originCandidates = p_board->position.piece_BB[piece] & (*captureCB[piece])(p_board, dest, other_color(color));
    if(count_bits(originCandidates) != 1) {
        disambiguationString = disambiguateOriginFromMove(p_board, piece, dest, m);
    }

    int len = (1 + ((disambiguationString) ? strlen(disambiguationString) : 0) + ((captured_piece(m)) ? 1 : 0) + 2);
    // one character for the moved piece, up to two to disambiguate the origin, potentially an 'x' for a capture, and a 2 character destination
    char *notation = malloc(len);
    notation[0] = pieceChar;
    notation[1] = '\0';
    if(disambiguationString) {
        strcat(notation, disambiguationString);
        free(disambiguationString);
    }
    if(captured_piece(m))
        strcat(notation, "x");
    strcat(notation, SQUARENAME[dest]);

    return notation;
}

bool isCapture(char *notation) {
    return strchr(notation, 'x') != NULL;
}

unsigned int getDestination(char *notation) {
    char *end = notation + strlen(notation) - 1;
    while(end != notation && !isalpha(*end)) end--;
    //if((end == notation && strlen(end) != 2) || strlen(end) < 2) return INVALID_SQUARE;
    assert(strlen(end) >= 2);
    char destinationSquare[3];
    strncpy(destinationSquare, end, 2);
    destinationSquare[2] = '\0';
    return get_square(destinationSquare);
}

unsigned int getPawnOrigin(Board *p_board, UCHAR piece, int destination, char *notation) {
    // This might not work in chess variants with fully randomized starting positions.
    if(isCapture(notation)) {
        U64 originCandidates = p_board->position.piece_BB[piece] & (*captureCB[piece])(p_board, destination, other_color(p_board->info.to_play));
        if(count_bits(originCandidates) == 1) return LSB(originCandidates);
        originCandidates &= fileBB[notation[0] - 'a'];
        return LSB(originCandidates);
    } else {
        U64 originHint = pawn_timid_BB(p_board, destination, other_color(p_board->info.to_play));
        if(originHint) {
            // the piece must be two back
            return (p_board->info.to_play == WHITE) ? destination - 16 : destination + 16;
        } else {
            // the piece must be one back
            return (p_board->info.to_play == WHITE) ? destination - 8 : destination + 8;
        }
    }
}

unsigned int getOrigin(Board *p_board, UCHAR piece, int destination, char *notation) {
    U64 originCandidates = p_board->position.piece_BB[piece] & (*captureCB[piece])(p_board, destination, other_color(p_board->info.to_play));
    //if(!originCandidates) return INVALID_SQUARE;
    assert(originCandidates);
    if(count_bits(originCandidates) == 1) return LSB(originCandidates);
    char disambiguateChar = *notation;
    if(isalpha(disambiguateChar)) {
        // we were given a file
        int index = disambiguateChar - 'a';
        originCandidates &= fileBB[index];
        if(count_bits(originCandidates) == 1) return LSB(originCandidates);
        disambiguateChar = *(notation+1);
        assert(isdigit(disambiguateChar));
        originCandidates &= rankBB[disambiguateChar-'1'];
        return LSB(originCandidates);
    } else {
        // a rank
        assert(isdigit(disambiguateChar));
        int index = disambiguateChar - '1';
        originCandidates &= rankBB[index];
        return LSB(originCandidates);
    }
}

unsigned int getPromo(UCHAR piece, char *notation) {
    char *end = notation + strlen(notation) - 1;
    while(end != notation && !isalpha(*end)) end--;
    char promoChar = *end;
    if(promoChar != tolower(promoChar)) printf("Unknown promo character encountered: %c\n", promoChar);
    switch(promoChar) {
        case 'Q':
            return (color(piece) == WHITE) ? WHITE_QUEEN : BLACK_QUEEN;
        break;
        case 'R':
            return (color(piece) == WHITE) ? WHITE_ROOK : BLACK_ROOK;
        break;
        case 'B':
            return (color(piece) == WHITE) ? WHITE_BISHOP : BLACK_BISHOP;
        break;
        case 'N':
            return (color(piece) == WHITE) ? WHITE_KNIGHT : BLACK_KNIGHT;
        break;
    }
    return NO_PIECE;
}

Move coord_notation_to_move(Board *p_board, char *notation) {
    assert(strlen(notation) >= 4);
    char from_str[3];
    char to_str[3];
    from_str[0] = notation[0];
    from_str[1] = notation[1];
    from_str[2] = '\0';
    to_str[0] = notation[2];
    to_str[1] = notation[3];
    to_str[2] = '\0';

    int from_sq_idx = get_square(from_str);
    int to_sq_idx = get_square(to_str);
    UCHAR moved_piece = p_board->position.square[from_sq_idx];
    UCHAR capt_piece = p_board->position.square[to_sq_idx];

    // could be an en passant
    if (p_board->info.state[p_board->info.current_move].en_passant_square == to_sq_idx) {
        // the piece captured 'like a pawn'
        if (FILE(to_sq_idx) != FILE(from_sq_idx)) {
            // the piece is a pawn
            if (moved_piece == WP || moved_piece == BP) {
                // actually an en passant capture
                capt_piece = moved_piece == WP ? BP : WP;
                int color = color(moved_piece);
                return move_full(color == WHITE, color == BLACK, 0, 0, 0,
                             capt_piece, moved_piece, from_sq_idx, to_sq_idx);
            }
        }
    }

    // now we correctly have computed the from square field, the to square
    // field, and the captured piece field. Only have to worry about captures
    // and promotions
    if (strlen(notation) == 5) {
        // a promotion
        char promo_char = toupper(notation[4]);
        UCHAR promo_piece = NO_PIECE;
        if (color(moved_piece) == W) {
            switch (promo_char) {
            case 'Q':
                promo_piece = WQ;
                break;
            case 'R':
                promo_piece = WR;
                break;
            case 'B':
                promo_piece = WB;
                break;
            case 'N':
                promo_piece = WN;
                break;
            }
        }
        if (color(moved_piece) == B) {
            switch (promo_char) {
            case 'Q':
                promo_piece = BQ;
                break;
            case 'R':
                promo_piece = BR;
                break;
            case 'B':
                promo_piece = BB;
                break;
            case 'N':
                promo_piece = BN;
                break;
            }
        }
        return move_full(0, 0, 0, 0, promo_piece, capt_piece,
                     moved_piece, from_sq_idx, to_sq_idx);
    }
    // castling
    if (from_sq_idx == E1 && moved_piece == WK) {
        // could be white castling
        if (to_sq_idx == C1) {
            return queenSide(W);
        } else if (to_sq_idx == G1) {
            return kingSide(W);
        }

    } else if (from_sq_idx == E8 && moved_piece == BK) {
        // could be black castling
        if (to_sq_idx == C8) {
            return queenSide(B);
        } else if (to_sq_idx == G8) {
            return kingSide(B);
        }
    }

    // nothing special
    return move_full(0, 0, 0, 0, 0, capt_piece, moved_piece, from_sq_idx, to_sq_idx);
}

char *move_to_coord_notation(Move m) {
    // just have to generate the from and to squares and potentially
    // a promotion delimiter
    char *from_str = strdup(SQUARENAME[from(m)]);
    char *to_str = strdup(SQUARENAME[to(m)]);
    char *notation = NULL;
    if (promote(m)) {
        // need to make a five character long string
        notation = malloc(6);
        notation[0] = from_str[0];
        notation[1] = from_str[1];
        notation[2] = to_str[0];
        notation[3] = to_str[1];
        char promo_char = tolower(getPieceName(promote(m)));
        assert(promo_char != '?');

        notation[4] = promo_char;
        notation[5] = '\0';
    } else {
        notation = malloc(5);
        notation[0] = from_str[0];
        notation[1] = from_str[1];
        notation[2] = to_str[0];
        notation[3] = to_str[1];
        notation[4] = '\0';
    }

    free(from_str);
    free(to_str);
    return notation;
}

Move notation_to_move(Board *p_board, char *notation) {
    unsigned char color = p_board->info.to_play;
    if(!strcmp(notation, "O-O-O")) {
        return queenSide(color);
    } else if (!strcmp(notation, "O-O")) {
        return kingSide(color);
    }
    unsigned int piece, orig, dest;

    char controlChar = notation[0];
    if(controlChar == 'K') {
        piece = (color == WHITE) ? WHITE_KING : BLACK_KING;
        orig = LSB(p_board->position.piece_BB[piece]);
        dest = getDestination(notation+1);
        return move(p_board->position.square[dest], piece, orig, dest);
    } else if(controlChar == 'Q') {
        piece = (color == WHITE) ? WHITE_QUEEN : BLACK_QUEEN;
        dest = getDestination(notation+1);
        orig = getOrigin(p_board, piece, dest, notation+1);
        return move(p_board->position.square[dest], piece, orig, dest);
    } else if(controlChar == 'B') {
        piece = (color == WHITE) ? WHITE_BISHOP : BLACK_BISHOP;
        dest = getDestination(notation+1);
        orig = getOrigin(p_board, piece, dest, notation+1);
        return move(p_board->position.square[dest], piece, orig, dest);
    } else if(controlChar == 'R') {
        piece = (color == WHITE) ? WHITE_ROOK : BLACK_ROOK;
        dest = getDestination(notation+1);
        orig = getOrigin(p_board, piece, dest, notation+1);
        return move(p_board->position.square[dest], piece, orig, dest);
    } else if(controlChar == 'N') {
        piece = (color == WHITE) ? WHITE_KNIGHT : BLACK_KNIGHT;
        dest = getDestination(notation+1);
        orig = getOrigin(p_board, piece, dest, notation+1);
        return move(p_board->position.square[dest], piece, orig, dest);
    } else {
        piece = (color == WHITE) ? WHITE_PAWN : BLACK_PAWN;
        dest = getDestination(notation);
        orig = getPawnOrigin(p_board, piece, dest, notation);
        unsigned int promo = 0;
        unsigned int capture = 0;
        unsigned int ep = 0;
        if((color == WHITE && RANK(dest) == 7) || (color == BLACK && RANK(dest) == 2)) {
            // promotion
            promo = getPromo(piece, notation);
        }
        if(isCapture(notation)) {
            capture = p_board->position.square[dest];
            if(!capture) {
                // en passant, could do additional checks, but if the move is coeherent, this works
                capture = (color == WHITE) ? BLACK_PAWN : WHITE_PAWN;
                ep = 1;
            }
        }
        return move_full((color == BLACK && ep) ? 1 : 0, (color == WHITE && ep) ? 1 : 0, 0, 0, promo, capture, piece, orig, dest);
    }
}
