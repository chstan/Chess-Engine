#include <stdio.h>
#include <pthread.h>

#include "log.h"

const char *logging_location = "/Users/chstansbury/engine.log";

pthread_mutex_t log_lock;

void init_logging() {
    pthread_mutex_init(&log_lock, NULL);
}

void cleanup_logging() {
    pthread_mutex_destroy(&log_lock);
}

void log_string(char *s) {
    pthread_mutex_lock(&log_lock);
    FILE *f = fopen(logging_location, "a+");
    fprintf(f, "%s\n", s);
    fflush(f);
    fclose(f);
    pthread_mutex_unlock(&log_lock);
}
