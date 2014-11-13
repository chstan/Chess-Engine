#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

#include "uci.h"
#include "manage_time.h"
#include "log.h"
#include "search/search.h"
#include "fen.h"
#include "move/move.h"
#include "move/notation.h"
#include "board/board.h"
#include "init.h"
#include "extglobals.h"
#include "zobrist.h"
#include "exit.h"
#include "eval/eval.h"

bool is_init = false;

void uci_info(int depth, int selective_depth, int score,
              __attribute__((unused)) unsigned char node_type,
              Move *pv_moves, size_t pv_move_count) {
    printf("info depth %d ", depth);
    if (selective_depth) {
        printf("seldepth %d ", selective_depth);
    }
    if (abs(score) > EVAL_MATE - 200) {
        if (score > 0) {
            // computer winning
            printf("score mate %d ", (EVAL_MATE - score + 1)/2);
        } else {
            // computer losing
            printf("score mate %d ", -(EVAL_MATE + score + 1)/2);
        }
    } else {
        printf("score cp %d ", score);
    }
    if (pv_move_count) {
        // print the principal variation
        printf("pv");
        for (size_t move_iter = 0; move_iter < pv_move_count && !should_stop(); move_iter++) {
            char *notation = move_to_coord_notation(pv_moves[move_iter]);
            assert(notation);
            printf(" %s", notation);
            free(notation);
        }
    }
    printf(" hashfull %llu", hash_per_million_full());
    printf("\n");
    fflush(stdout);
}

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

void uci_stop() {
    set_to_stop(true);
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
    uci_stop();
    sleep(1); // bad, should wait for think thread exit
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

void uci_go_test() {
    set_infinite_think(false);
    set_black_time_left(4000);
    set_white_time_left(4000);
    set_black_time_inc(0);
    set_white_time_inc(0);
    assert(is_init);
    set_to_stop(false);
    pthread_t search_thread;
    pthread_create(&search_thread, NULL, &threadable_think, pBoard);
}

void uci_go(char *go_options) {
    // Weirdness in Scid vs Mac. Something to look into...
    // Multiple go commands being sent.
    if (is_thinking()) {
        log_string("Woah there!");
        return;
    }

    // seachmoves -- restrict search
    // ponder
    // wtime in msec
    // btime in msec
    // winc in msec per move if positive
    // binc in msec per move if positive
    // overstock -- until next time control
    // depth -- restriction
    // nodes -- node count restriction
    // mate -- search for a mate in x
    // movetime -- search for x msec
    // infinite -- search until the stop command
    while (go_options[0] == ' ' || go_options[0] == '\t') {
        go_options++;
    }
    if(go_options[0] != '\n') {
        // some tokens still to parse
        char *tokens_buffer = strdup(go_options);
        assert(tokens_buffer[strlen(tokens_buffer) - 1] == '\n');
        tokens_buffer[strlen(tokens_buffer) - 1] = '\0';

        const char *sep = " ";
        char *next_token = strtok(tokens_buffer, sep);
        set_infinite_think(false);
        while (next_token) {
            if (!strcmp(next_token, "infinite")) {
                set_infinite_think(true);
                break;
            }
            if (!strcmp(next_token, "wtime")) {
                char *time_left_token = strtok(NULL, sep);
                assert(time_left_token);
                unsigned int w_time_left = (unsigned) atoi(time_left_token);
                set_white_time_left(w_time_left);
            }
            if (!strcmp(next_token, "btime")) {
                char *time_left_token = strtok(NULL, sep);
                assert(time_left_token);
                unsigned int b_time_left = (unsigned) atoi(time_left_token);

                set_black_time_left(b_time_left);
            }
            if (!strcmp(next_token, "winc")) {
                char *time_inc_token = strtok(NULL, sep);
                assert(time_inc_token);
                int w_time_inc = atoi(time_inc_token);
                if (w_time_inc > 0) {
                    set_white_time_inc(w_time_inc);
                } else {
                    set_white_time_inc(0);
                }
            }
            if (!strcmp(next_token, "binc")) {
                char *time_inc_token = strtok(NULL, sep);
                assert(time_inc_token);
                int b_time_inc = atoi(time_inc_token);
                if (b_time_inc > 0) {
                    set_black_time_inc(b_time_inc);
                } else {
                    set_black_time_inc(0);
                }
            }
            next_token = strtok(NULL, sep);
        }
        free(tokens_buffer);
    }

    assert(is_init);
    set_to_stop(false);

    pthread_t search_thread;
    pthread_create(&search_thread, NULL, &threadable_think, pBoard);
}

void uci_best_move(Move m) {
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
    if (strpref("stop", line)) {
        uci_stop();
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
    pre_init();
    char *buffer = NULL;
    size_t read_bytes = 0;
    bool should_exit = false;
    while ((read_bytes = getline(&buffer, &read_bytes, stdin)) > 0) {
        log_string(buffer);
        should_exit = uci_process_command(buffer);
        free(buffer);
        buffer = NULL;
        if (should_exit) {
            return;
        }
    }
}
