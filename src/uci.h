#ifndef UCI_H
#define UCI_H

#include <stdlib.h>

#include "move/movegen.h"

// waits on input from GUI and proceeds accordingly
void uci_loop();

void uci_best_move(Move m);

void uci_info(int depth, int selective_depth, int score, unsigned char node_type,
              Move *pv_moves, size_t pv_move_count);

#endif
