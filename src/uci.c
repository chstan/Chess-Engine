#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "search/search.h"
#include "fen.h"
#include "move/move.h"
#include "move/notation.h"
#include "board/board.h"
#include "uci.h"
#include "init.h"
#include "extglobals.h"
#include "exit.h"

bool is_init = false;

void apply_moves_from_str(char *moves_str) {
    char *move_token;
    const char *sep = " ";
    move_token = strtok(moves_str, sep);
    while (move_token) {
        Move m = coord_notation_to_move(pBoard, move_token);
        assert(m && "Invalid move parsed.");
        makeMove(pBoard, m);
        move_token = strtok(NULL, sep);
    }
}

bool strpref(const char *pre, const char *str) {
    size_t pre_len = strlen(pre);
    size_t str_len = strlen(str);
    return str_len < pre_len ? false : strncmp(pre, str, pre_len) == 0;
}

void uci_prep_newgame() {
    if (!is_init) {
        is_init = true;
        init();
    }
    resetBoard(pBoard);
    // also reset transposition tables when they are written
}

void uci_init() {
    if (!is_init) {
        is_init = true;
        init();
    }
    printf("readyok\n");
    fflush(stdout);
}

void uci_quit() {
    if (is_init)
        prepareForExit();
}

void uci_position(const char *position_str) {
    const char *startpos_str = "startpos";
    // note the spaces in moves_str!
    const char *moves_str = " moves ";

    if (strpref(startpos_str, position_str)) {
        // we are to use the start position
        const char *rest = position_str + strlen(startpos_str);

        assert(is_init);

        resetBoard(pBoard);
        if (strpref(moves_str, rest)) {
            // there were additional moves
            const char *moves_start = rest + strlen(moves_str);

            // remove the very last character which is a newline
            size_t size_of_i_moves_str = strlen(moves_start) - 1;
            char *i_moves_str = malloc(size_of_i_moves_str + 1);
            strncpy(i_moves_str, moves_start, size_of_i_moves_str);
            i_moves_str[size_of_i_moves_str] = '\0';

            apply_moves_from_str(i_moves_str);

            free(i_moves_str);
        }
    }
    else {
        // parse FEN
        char *moves_loc = strstr(position_str, moves_str);
        const char *fen_delim = "fen ";
        char *fen_start = strstr(position_str, fen_delim) + strlen(fen_delim);
        if(!moves_loc) {
            // Just FEN
            size_t size_of_fen = strlen(fen_start) - 1;
            char *fen_str = malloc(size_of_fen + 1);
            strncpy(fen_str, fen_start, size_of_fen);
            fen_str[size_of_fen] = '\0';
            FEN *parsed_fen = FENFromLine(fen_str);
            if (!parsed_fen) {
                free(fen_str);
                return;
            }
            loadFromFEN(pBoard, parsed_fen);
            free(fen_str);
            free(parsed_fen);
        } else {
            // FEN and moves
            // size of fen string
            size_t size_of_fen = moves_loc - fen_start;
            // need to save room for zero delimiter
            char *fen_str = malloc(size_of_fen + 1);
            strncpy(fen_str, fen_start, size_of_fen);
            // zero delimit the string since strncpy does not
            fen_str[size_of_fen] = '\0';
            FEN *parsed_fen = FENFromLine(fen_str);
            if (!parsed_fen) {
                free(fen_str);
                return;
            }
            loadFromFEN(pBoard, parsed_fen);
            free(fen_str);
            free(parsed_fen);

            // don't bother if there's no listed moves
            if (strlen(moves_loc) - strlen(moves_str) <= 2)
                return;

            // don't copy the newline
            size_t size_of_i_moves_str = strlen(moves_loc) - strlen(moves_str) - 1;
            char *i_moves_str = malloc(size_of_i_moves_str + 1);
            strncpy(i_moves_str, moves_loc + strlen(moves_str), size_of_i_moves_str);
            i_moves_str[size_of_i_moves_str] = '\0';

            apply_moves_from_str(i_moves_str);

            free(i_moves_str);
        }
    }
}

void uci_go(__attribute__((unused)) char *go_options) {
    // seachmoves -- restrict search
    // ponder
    // wtime in msec
    // btime in msec
    // winc in msec per move if positive
    // binc in msec per move if positive
    // movestogo -- until next time control
    // depth -- restriction
    // nodes -- node count restriction
    // mate -- search for a mate in x
    // movetime -- search for x msec
    // infinite -- search until the stop command
    Move m = think(pBoard);
    char *notation = move_to_coord_notation(m);
    printf("bestmove %s\n", notation);
    fflush(stdout);
    free(notation);
}

void uci_greet() {
    printf("id name Dev\n");
    printf("id author Conrad Stansbury, Tom Mullins\n");
    printf("uciok\n");
    fflush(stdout);
}

bool uci_process_command(char *line) {
    if (strpref("ucinewgame", line)) {
        uci_prep_newgame();
        return false;
    }
    if (strpref("uci", line)) {
        uci_greet();
        return false;
    }
    if (strpref("isready", line)) {
        uci_init();
        return false;
    }
    if (strpref("quit", line)) {
        uci_quit();
        return true;
    }

    // note the space prefixes
    const char* position_pref = "position ";
    if (strpref(position_pref, line)) {
        char *position_str = line + strlen(position_pref);
        uci_position(position_str);
    }

    const char *go_pref = "go";
    if (strpref(go_pref, line)) {
        char *go_str = line + strlen(go_pref);
        uci_go(go_str);
    }

    return false;
}

void uci_loop() {
    printf("info Dev Engine 0.0.1\n");
    fflush(stdout);

    char *buffer = NULL;
    size_t read_bytes = 0;
    bool should_exit = false;
    FILE *f = fopen("/Users/chstansbury/src/Chess-Engine/engine.log", "w+");
    if (!f) {
        return;
    }
    while ((read_bytes = getline(&buffer, &read_bytes, stdin)) > 0) {
        fprintf(f, "%s", buffer);
        should_exit = uci_process_command(buffer);
        free(buffer);
        buffer = NULL;
        if (should_exit) {
            fclose(f);
            return;
        }
    }
    fclose(f);
}
