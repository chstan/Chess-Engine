#include <stdio.h>
#include <stdlib.h>

#include "commandlist.h"
#include "manage_time.h"
#include "defines.h"
#include "extglobals.h"
#include "alert.h"
#include "move/move.h"
#include "move/movegen.h"
#include "move/notation.h"
#include "aux/random.h"
#include "test/movegentest.h"
#include "pgnfile.h"
#include "fen.h"
#include "search/search.h"

CommandCB commandCallbacks[] = {
    playCommand,
    listMovesCommand,
    randomMoveCommand,
    performanceTestCommand,
    divideCommand,
    helpCommand,
    moveCommand,
    unmoveCommand,
    aboutCommand,
    displayCommand,
    loadpgnCommand,
    loadFENCommand,
    debugCommand,
    debugNotationCommand,
    rotateCommand,
    todoCommand,
    quitCommand
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

bool playCommand(int tokenCount, char **tokens) {
    if(tokenCount == 0) {
        printf("You have to specify a move as an additional parameter in algebraic chess notation.\n\n");
        return true;
    }
    Move m;
    if((m = notationToMove(pBoard, tokens[0]))) {
#ifdef DEBUG
        printMove(m);
#endif
        makeMove(pBoard, m);
        // have to set some time controls
        set_infinite_think(false);
        set_black_time_left(20000);
        set_white_time_left(20000);
        set_black_time_inc(0);
        set_white_time_inc(0);
        set_to_stop(false);

        Move reply = think(pBoard);
        printMove(reply);
        char *replyString = moveToNotation(pBoard, reply);
        if(replyString) {
            printf("%s\n", replyString);
            free(replyString);
        }
        makeMove(pBoard, reply);
        displayBoard(pBoard);
    }

    return true;
}

bool listMovesCommand(__attribute__((unused)) int tokenCount,
                      __attribute__((unused)) char **tokens) {
    MoveSet moves;
    resetMoveSet(&moves);
    initializeMoveSetQuiet(pBoard, &moves);
    for(int i = 0; i < moves.totalMoves; i++) {
        Move currentMove = moves.moveList[i];
        char *notation = moveToNotation(pBoard, currentMove);
        printf("%s\n", notation);
        free(notation);
    }

    return true;
}

bool randomMoveCommand(__attribute__((unused)) int tokenCount,
                       __attribute__((unused)) char **tokens) {
    MoveSet moves;
    resetMoveSet(&moves);
    initializeMoveSetQuiet(pBoard, &moves);
    int moveIndex = randomi(0, moves.totalMoves);
    Move randomMove = moves.moveList[moveIndex];
    printMove(randomMove);
    makeMove(pBoard, randomMove);
    displayBoard(pBoard);
    return true;
}

bool performanceTestCommand(int tokenCount, char **tokens) {
    if(tokenCount != 1) {
        printf("Performance test usage: This command takes one parameter, the depth to be searched.\n");
        return true;
    }
    int depth = atoi(tokens[0]);
    performanceTest(pBoard, depth, false);

    return true;
}

bool helpCommand(__attribute__((unused)) int tokenCount,
                 __attribute__((unused)) char **tokens) {
    printf("Command list:\n\n\n");
    for(int commandIndex = 0; commandIndex < COMMAND_COUNT; commandIndex++) {
        printf(" %s\n\n", commandDescription[commandIndex]);
    }
    return true;
}

bool moveCommand(int tokenCount, char **tokens) {
    if(tokenCount == 0) {
        printf("You have to specify a move as an additional parameter in algebraic chess notation.\n\n");
        return true;
    }
    Move m;
    if((m = notationToMove(pBoard, tokens[0]))) {
        printMove(m);
        makeMove(pBoard, m);
    }
    return true;
}

bool unmoveCommand(__attribute__((unused)) int tokenCount,
                   __attribute__((unused)) char **tokens) {
    unmakeLastMove(pBoard);
    return true;
}

bool aboutCommand(__attribute__((unused)) int tokenCount,
                  __attribute__((unused)) char **tokens) {
    printf("\nThis is a computer chess engine being developed by Conrad and Tom (and Marc).\n%s\n\n", ENGINE_VERSION);
    return true;
}

bool displayCommand(__attribute__((unused)) int tokenCount,
                    __attribute__((unused)) char **tokens) {
    displayBoard(pBoard);
    return true;
}

bool loadpgnCommand(int tokenCount, char **tokens) {
    if (tokenCount >= 1) loadpgn(tokens[0], pBoard);
    return true;
}

bool loadFENCommand(int tokenCount, char **tokens) {
    if (tokenCount != 1) {
        printf("load-fen usage: accepts one parameter specifying the filename.\n");
        return true;
    }
    FEN *record = getFENFromFile(tokens[0]);

    if(!record) {
        printf("The file you specified did not contain a valid FEN record.\n");
        return true;
    }

    loadFromFEN(pBoard, record);
    free(record);
    return true;
}

bool debugCommand(__attribute__((unused)) int tokenCount,
                  __attribute__((unused)) char **tokens) {
    debugBoard(pBoard);
    return true;
}

bool rotateCommand(__attribute__((unused)) int tokenCount,
                   __attribute__((unused)) char **tokens) {
    rotateBoard(pBoard);
    return true;
}

bool todoCommand(__attribute__((unused)) int tokenCount,
                 __attribute__((unused)) char **tokens) {
    printf("This is for the developers so they can check what there is to implement.\n");
    return true;
}

bool quitCommand(__attribute__((unused)) int tokenCount,
                 __attribute__((unused)) char **tokens) {
    return false;
}

bool debugNotationCommand(__attribute__((unused)) int tokenCount,
                          __attribute__((unused)) char **tokens) {
    MoveSet moves;
    resetMoveSet(&moves);
    initializeMoveSetQuiet(pBoard, &moves);
    int passed = moves.totalMoves;
    for(int i = 0; i < moves.totalMoves; i++) {
        Move currentMove = moves.moveList[i];
        char *notation = moveToNotation(pBoard, currentMove);
        Move checkMove = notationToMove(pBoard, notation);
        if(currentMove != checkMove) {
            printf("Generated:\n");
            printMove(currentMove);
            printf("Converted to: %s\n", notation);
            printf("Reconverted to:\n");
            printMove(checkMove);
            passed--;
        }
        free(notation);
    }
    printf("%d/%d passed.\n", passed, moves.totalMoves);
    return true;
}

bool divideCommand(int tokenCount, char **tokens) {
    if(tokenCount != 1) {
        printf("Divide usage: This command takes one parameter, the depth to be searched.\n");
        return true;
    }
    int depth = atoi(tokens[0]);
    performanceTest(pBoard, depth, true);

    return true;
}
