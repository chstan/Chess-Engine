#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "movegentest.h"
#include "../move/notation.h"
#include "../move/movegen.h"
#include "../defines.h"

static void doPerfTest(Board *p_board, int depth);

void performance_test(Board *p_board, int depth, bool divide) {
    if (depth < 1) return;

    if (divide)
        printf("%-8s ", "Move");
    printf("%-16s %-8s", "# Moves", "Time (s)");
    #ifdef DEBUG
    printf(" %-8s %-8s %-8s %-8s %-8s", "Caps", "EPs", "Castles", "Proms", "Checks");
    #endif
    printf("\n");

    if (divide){
        MoveSet moves;
        reset_move_set(&moves);
        generate_move(p_board, &moves);

        for (int i = 0; i < moves.total_moves; i++) {
            Move m = moves.move_list[i];
            char *notation = move_to_notation(p_board, m);

            make_move(p_board, m);
            if (!checks(p_board, other_color(p_board->info.to_play))) {
                printf("%-8s ", notation);
                doPerfTest(p_board, depth-1);
            }
            unmake_move(p_board, m);

            free(notation);
        }
    } else {
        doPerfTest(p_board, depth);
    }
}

static void doPerfTest(Board *p_board, int depth) {
    MoveCount count;
    memset(&count, 0, sizeof(MoveCount));
    int startTime = clock();
    U64 total_moves = recursive_move_count(p_board, depth, &count);
    int endTime = clock();
    double seconds = ((double)(endTime - startTime))/CLOCKS_PER_SEC;
    printf("%-16llu %-8.3f", total_moves, seconds);
    #ifdef DEBUG
    printf(" %-8llu %-8llu %-8llu %-8llu %-8llu", count.captures,
        count.en_passants, count.castles, count.promotions, count.checks);
    #endif
    printf("\n");
}

U64 recursive_move_count(Board *p_board, int depth, MoveCount *p_count) {
    MoveSet moves;
    U64 countedMoves = 0;
    reset_move_set(&moves);

    if(depth == 0) return 1;

    // now find all the nodes immediately off the current board state
    generate_move(p_board, &moves);
    if(depth == 1) {

        U64 count = 0;
        for (int i = 0; i < moves.total_moves; i++) {
            Move m = moves.move_list[i];
            make_move(p_board, m);
            if (!checks(p_board, other_color(p_board->info.to_play))) {
                count++;
                #ifdef DEBUG
                if (captured_piece(m)) p_count->captures++;
                if (white_en_passant(m) || black_en_passant(m)) p_count->en_passants++;
                if (promote(m)) p_count->promotions++;
                if (white_castle(m) || black_castle(m)) p_count->castles++;
                if (checks(p_board, p_board->info.to_play)) p_count->checks++;
                #endif
            }
            unmake_move(p_board, m);
        }
        return count;
    }

    for(int i = 0; i < moves.total_moves; i++) {
        Move m = moves.move_list[i];
        make_move(p_board, m);
        if (!checks(p_board, other_color(p_board->info.to_play))) {
            countedMoves += recursive_move_count(p_board, depth-1, p_count);
        }
        unmake_move(p_board, m);
    }
    return countedMoves;
}
