#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>

#include "log.h"

#define LOGGING

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
    pthread_mutex_lock(&log_lock);

    va_list arg_f, arg_o;

    #ifdef LOGGING
    FILE *log_file = fopen(logging_location, "a+");
    assert(log_file);
    #endif

    va_start(arg_f, fmt);
    va_copy(arg_o, arg_f);

    vfprintf(stdout, fmt, arg_o);
    #ifdef LOGGING
    vfprintf(log_file, fmt, arg_f);
    #endif
    va_end(arg_o);
    va_end(arg_f);

    #ifdef LOGGING
    fflush(log_file);
    fclose(log_file);
    #endif
    fflush(stdout);

    pthread_mutex_unlock(&log_lock);
}
