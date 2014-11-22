#include <stdio.h>
#include <stdlib.h>

#include "commandlist.h"
#include "manage_time.h"
#include "defines.h"
#include "extglobals.h"
#include "move/move.h"
#include "move/movegen.h"
#include "move/notation.h"
#include "aux/random.h"
#include "test/movegentest.h"
#include "pgnfile.h"
#include "fen.h"
#include "search/search.h"

CommandCB commandCallbacks[] = {
    play_command,
    list_moves_command,
    random_move_command,
    performance_test_command,
    divide_command,
    help_command,
    move_command,
    unmove_command,
    about_command,
    display_command,
    load_pgn_command,
    load_FEN_command,
    debug_command,
    debug_notation_command,
    rotate_command,
    todo_command,
    quit_command
};

char *commandTriggers[] = {
    "play",
    "list-moves",
    "random-move",
    "performance-test",
    "divide",
    "help",
    "move",
    "unmove",
    "about",
    "display",
    "load-pgn",
    "load-fen",
    "debug-board",
    "debug-notation",
    "rotate",
    "todo",
    "quit"
};

char *commandDescription[] = {
    "play: plays the move that you specify. The computer will respond by playing a move back. Stupidly.",
    "list-moves: generates all the moves from the current board position and display them.",
    "random-move: generates all the moves from the current board position and applies one to the board.",
    "performance-test: will print the time to generate and the number of generated positions to a specified depth.",
    "divide: will print out all moves at depth one in algebraic notation, and run performance-test off each node to depth-1.",
    "help: prints the list of all commands.",
    "move: applies a move in algebraic notation.",
    "unmove: unapplies the last move.",
    "about: prints information about the chess engine",
    "display: prints the board state along with certain internal information",
    "load-pgn: loads a .pgn file specified by [FILENAME]\n",
    "load-fen: loads a .fen file specified by [FILENAME]\n",
    "debug-board: runs an internal board consistency check. No results are good results.",
    "debug-notation: generates all moves from the current board position, and runs a check by converting through the notation functions.",
    "rotate: rotates the board when displaying.",
    "todo: prints a todo list for the developers, largely unnecessary now they project is on Github",
    "quit: quits from the chess engine back to your ::1ly terminal"
};

bool play_command(int token_count, char **tokens) {
    if(token_count == 0) {
        printf("You have to specify a move as an additional parameter in algebraic chess notation.\n\n");
        return true;
    }
    Move m;
    if((m = notation_to_move(p_board, tokens[0]))) {
#ifdef DEBUG
        print_move(m);
#endif
        make_move(p_board, m);
        // have to set some time controls
        set_infinite_think(false);
        set_black_time_left(20000);
        set_white_time_left(20000);
        set_black_time_inc(0);
        set_white_time_inc(0);
        set_to_stop(false);

        Move reply = think(p_board);
        print_move(reply);
        char *replyString = move_to_notation(p_board, reply);
        if(replyString) {
            printf("%s\n", replyString);
            free(replyString);
        }
        make_move(p_board, reply);
        display_board(p_board);
    }

    return true;
}

bool list_moves_command(__attribute__((unused)) int token_count,
                      __attribute__((unused)) char **tokens) {
    MoveSet moves;
    reset_move_set(&moves);
    initialize_move_set_quiet(p_board, &moves);
    for(int i = 0; i < moves.total_moves; i++) {
        Move current_move = moves.move_list[i];
        char *notation = move_to_notation(p_board, current_move);
        printf("%s\n", notation);
        free(notation);
    }

    return true;
}

bool random_move_command(__attribute__((unused)) int token_count,
                       __attribute__((unused)) char **tokens) {
    MoveSet moves;
    reset_move_set(&moves);
    initialize_move_set_quiet(p_board, &moves);
    int moveIndex = randomi(0, moves.total_moves);
    Move randomMove = moves.move_list[moveIndex];
    print_move(randomMove);
    make_move(p_board, randomMove);
    display_board(p_board);
    return true;
}

bool performance_test_command(int token_count, char **tokens) {
    if(token_count != 1) {
        printf("Performance test usage: This command takes one parameter, the depth to be searched.\n");
        return true;
    }
    int depth = atoi(tokens[0]);
    performance_test(p_board, depth, false);

    return true;
}

bool help_command(__attribute__((unused)) int token_count,
                 __attribute__((unused)) char **tokens) {
    printf("Command list:\n\n\n");
    for(int commandIndex = 0; commandIndex < COMMAND_COUNT; commandIndex++) {
        printf(" %s\n\n", commandDescription[commandIndex]);
    }
    return true;
}

bool move_command(int token_count, char **tokens) {
    if(token_count == 0) {
        printf("You have to specify a move as an additional parameter in algebraic chess notation.\n\n");
        return true;
    }
    Move m;
    if((m = notation_to_move(p_board, tokens[0]))) {
        print_move(m);
        make_move(p_board, m);
    }
    return true;
}

bool unmove_command(__attribute__((unused)) int token_count,
                   __attribute__((unused)) char **tokens) {
    unmake_last_move(p_board);
    return true;
}

bool about_command(__attribute__((unused)) int token_count,
                  __attribute__((unused)) char **tokens) {
    printf("\nThis is a computer chess engine being developed by Conrad and Tom (and Marc).\n%s\n\n", ENGINE_VERSION);
    return true;
}

bool display_command(__attribute__((unused)) int token_count,
                    __attribute__((unused)) char **tokens) {
    display_board(p_board);
    return true;
}

bool load_pgn_command(int token_count, char **tokens) {
    if (token_count >= 1) load_pgn(tokens[0], p_board);
    return true;
}

bool load_FEN_command(int token_count, char **tokens) {
    if (token_count != 1) {
        printf("load-fen usage: accepts one parameter specifying the filename.\n");
        return true;
    }
    FEN *record = get_FEN_from_file(tokens[0]);

    if(!record) {
        printf("The file you specified did not contain a valid FEN record.\n");
        return true;
    }

    load_from_FEN(p_board, record);
    free(record);
    return true;
}

bool debug_command(__attribute__((unused)) int token_count,
                  __attribute__((unused)) char **tokens) {
    debug_board(p_board);
    return true;
}

bool rotate_command(__attribute__((unused)) int token_count,
                   __attribute__((unused)) char **tokens) {
    rotate_board(p_board);
    return true;
}

bool todo_command(__attribute__((unused)) int token_count,
                 __attribute__((unused)) char **tokens) {
    printf("This is for the developers so they can check what there is to implement.\n");
    return true;
}

bool quit_command(__attribute__((unused)) int token_count,
                 __attribute__((unused)) char **tokens) {
    return false;
}

bool debug_notation_command(__attribute__((unused)) int token_count,
                          __attribute__((unused)) char **tokens) {
    MoveSet moves;
    reset_move_set(&moves);
    initialize_move_set_quiet(p_board, &moves);
    int passed = moves.total_moves;
    for(int i = 0; i < moves.total_moves; i++) {
        Move current_move = moves.move_list[i];
        char *notation = move_to_notation(p_board, current_move);
        Move checkMove = notation_to_move(p_board, notation);
        if(current_move != checkMove) {
            printf("Generated:\n");
            print_move(current_move);
            printf("Converted to: %s\n", notation);
            printf("Reconverted to:\n");
            print_move(checkMove);
            passed--;
        }
        free(notation);
    }
    printf("%d/%d passed.\n", passed, moves.total_moves);
    return true;
}

bool divide_command(int token_count, char **tokens) {
    if(token_count != 1) {
        printf("Divide usage: This command takes one parameter, the depth to be searched.\n");
        return true;
    }
    int depth = atoi(tokens[0]);
    performance_test(p_board, depth, true);

    return true;
}
