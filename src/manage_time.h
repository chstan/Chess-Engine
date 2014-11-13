#ifndef MANAGE_TIME_H
#define MANAGE_TIME_H

#include <stdbool.h>

void set_is_thinking(bool s);
bool is_thinking(void);

void set_infinite_think(bool s);

void start_search_clock(bool is_white);

void set_white_time_left(unsigned int w_time_left);
void set_black_time_left(unsigned int b_time_left);

void set_white_time_inc(unsigned int w_time_inc);
void set_black_time_inc(unsigned int b_time_inc);

bool should_continue_greater_depth(unsigned int last_search_time);
bool should_stop_on_time();

void init_manage_time();
void cleanup_manage_time();

void set_to_stop(bool should_stop);
bool should_stop_gui_request();

bool should_stop();

#endif
