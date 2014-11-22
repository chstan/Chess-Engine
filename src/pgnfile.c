#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "extglobals.h"
#include "defines.h"
#include "move/move.h"
#include "pgnfile.h"
#include "move/notation.h"

#define NONE -1

// TODO better enforce turn order

static int checkComment(FILE* file) {
    char temp;
    int result;
    while (1) {
        result = fscanf(file, ";%c", &temp);
        if (result == EOF || result == 0) break;
        if (temp != '\n') {
            result = fscanf(file, "%*[^\n] ");
            if (result == EOF) break;
        }
    }
    return result == EOF;
}

int load_pgn(const char* filename, Board *p_board) {
    int turn, newturn, color = NONE;
    int result = 0, ret = 0;
    Move m;
    char periodstr[4], movestr[16], temp;
    periodstr[3] = 0;
    movestr[15] = 0;

    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "load_pgn: unable to open %s\n", filename);
        return 1;
    }

    do result = fscanf(file, "[%*[^]]%c ", &temp);
    while (result > 0 && result != EOF);

    while (1) {

        if (checkComment(file)) break;

        do result = fscanf(file, "{%*[^}]%c ", &temp);
        while (result > 0 && result != EOF);

        if (checkComment(file)) break;

        result = fscanf(file, "%d%3s ", &newturn, periodstr);
        if (result == EOF) break;
        else if (result == 0) {
            if (color == NONE) {
                fprintf(stderr, "load_pgn: expected turn number\n");
                ret = 1;
                break;
            }
        } else {
            if (!strcmp(periodstr, "."))
                color = W;
            else if (!strcmp(periodstr, "..."))
                color = B;
            else if (periodstr[0] == '/' || periodstr[0] == '-')
                break;
            else {
                fprintf(stderr, "load_pgn: expected 1 or 3 periods following turn number\n");
                ret = 1;
                break;
            }
            turn = newturn;
        }

        if (checkComment(file)) break;

        result = fscanf(file, "%15s ", movestr);
        if (result == EOF) {
            if (turn != newturn) {
                fprintf(stderr, "load_pgn: unexpected EOF after turn number\n");
                ret = 1;
            }
            break;
        }
        if (!isalpha(movestr[0])) {
            fprintf(stderr, "load_pgn: unexpected character in move string\n");
            ret = 1;
            break;
        }

        // TODO get m and make_move(p_board, m);
        printf("Turn %d color %s string %s\n", turn, (color == W)? "white" :
            (color == B)? "black" : "none", movestr);
        m = notation_to_move(p_board, movestr);
        print_move(m);
        make_move(p_board, m);
        display_board(p_board);
        if (color == W) color = B;
        else color = NONE;

    }
    fclose(file);
    return ret;
}
