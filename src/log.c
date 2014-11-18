#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

#include "log.h"

//#define LOGGING

char *logging_location = NULL;

pthread_mutex_t log_lock;

void set_logging_location(const char *loc) {
    if (!logging_location)
        logging_location = strdup(loc);
}

void init_logging() {
    pthread_mutex_init(&log_lock, NULL);
}

void cleanup_logging() {
    if (logging_location)
        free(logging_location);
    pthread_mutex_destroy(&log_lock);
}

void log_string(__attribute__((unused)) char *s) {
    #ifdef LOGGING
    pthread_mutex_lock(&log_lock);
    FILE *f = fopen(logging_location, "a+");
    assert(f);
    fprintf(f, "%s", s);
    fflush(f);
    fclose(f);
    pthread_mutex_unlock(&log_lock);
    #endif
}

void log_printf(const char *fmt, ...) {
    va_list arg_o;
    va_start(arg_o, fmt);

    #ifdef LOGGING
    va_list arg_f;
    va_copy(arg_f, arg_o);
    pthread_mutex_lock(&log_lock);
    FILE *log_file = fopen(logging_location, "a+");
    assert(log_file);
    vfprintf(log_file, fmt, arg_f);
    fflush(log_file);
    fclose(log_file);
    pthread_mutex_unlock(&log_lock);
    va_end(arg_f);
    #endif

    vfprintf(stdout, fmt, arg_o);
    fflush(stdout);

    va_end(arg_o);
}
