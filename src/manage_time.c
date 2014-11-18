#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>

#include "board/board.h"
#include "manage_time.h"

struct timeval search_start;
struct timeval search_target_end;
unsigned int white_time_left;
unsigned int white_time_inc;
unsigned int black_time_left;
unsigned int black_time_inc;

bool think_infinite;

bool v_is_thinking;
bool stop_status;

pthread_mutex_t lock;

void set_infinite_think(bool s) {
    think_infinite = s;
}

void start_search_clock(bool is_white) {
    gettimeofday(&search_start, NULL);

    // compute upper bound on search in msec
    unsigned int t_inc = is_white ? white_time_inc : black_time_inc;
    unsigned int t_left = is_white ? white_time_left : black_time_left;
    unsigned int longest_allowed_search = t_inc + (t_left / 30);
    longest_allowed_search = longest_allowed_search < t_left/2 ?
        longest_allowed_search :
        t_left/2;

    unsigned int target_usec = 1000 * (longest_allowed_search % 1000);
    unsigned int target_sec = 0;

    target_usec += search_start.tv_usec;

    if (target_usec > 1000000) {
        target_usec %= 1000000;
        target_sec += 1;
    }
    target_sec += longest_allowed_search / 1000;
    target_sec += search_start.tv_sec;

    search_target_end.tv_sec = target_sec;
    search_target_end.tv_usec = target_usec;
}

bool should_continue_greater_depth(clock_t last_search_ticks) {
    // this seems to be harmful for the moment, but it is possible that when
    // the effective branching factor is smaller that this will be a useful
    // thing to do
    #ifdef CONSERVATIVE_SEARCH_TIMING
    if (think_infinite) return true;

    // this is a guess
    clock_t effective_branching_factor = 8;

    // this is a little ugly because we are conflating
    // wall time with the processor time
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    long long int d_sec = search_target_end.tv_sec - current_time.tv_sec;
    long long int d_usec = search_target_end.tv_usec - current_time.tv_usec;

    // convert to ticks
    d_usec += d_sec * 1000000;
    d_usec = (d_usec * CLOCKS_PER_SEC) / (1000000);

    if (d_usec < (long long int) (last_search_ticks * effective_branching_factor)) {
        // not enough time remaining to complete an additional search
        return false;
    }
    #endif

    return !should_stop();
}

bool should_stop_on_time() {
    if (think_infinite) return false;
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    if (current_time.tv_sec < search_target_end.tv_sec)
        return false;
    if (current_time.tv_sec == search_target_end.tv_sec)
        if(current_time.tv_usec < search_target_end.tv_usec)
            return false;
    return true;
}


void init_manage_time() {
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_lock(&lock);
    v_is_thinking = false;
    pthread_mutex_unlock(&lock);
}

void cleanup_manage_time() {
    pthread_mutex_destroy(&lock);
}

void set_to_stop(bool should_stop) {
    pthread_mutex_lock(&lock);
    stop_status = should_stop;
    pthread_mutex_unlock(&lock);
}

void set_is_thinking(bool s) {
    pthread_mutex_lock(&lock);
    v_is_thinking = s;
    pthread_mutex_unlock(&lock);
}

bool is_thinking() {
    bool thinking_status;
    pthread_mutex_lock(&lock);
    thinking_status = v_is_thinking;
    pthread_mutex_unlock(&lock);
    return thinking_status;
}

bool should_stop() {
    return should_stop_gui_request() || should_stop_on_time();
}

bool should_stop_gui_request() {
    bool status;
    pthread_mutex_lock(&lock);
    status = stop_status;
    pthread_mutex_unlock(&lock);
    return status;
}

void set_white_time_left(unsigned int w_time_left) {
    white_time_left = w_time_left;
}
void set_black_time_left(unsigned int b_time_left) {
    black_time_left = b_time_left;
}

void set_white_time_inc(unsigned int w_time_inc) {
    white_time_inc = w_time_inc;
}
void set_black_time_inc(unsigned int b_time_inc) {
    black_time_inc = b_time_inc;
}
