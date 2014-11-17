#ifndef LOG_H
#define LOG_H

void set_logging_location(const char *loc);
void init_logging();
void cleanup_logging();
void log_string(char *s);

void log_printf(const char *fmt, ...) __attribute__((format (printf, 1, 2)));

#endif
