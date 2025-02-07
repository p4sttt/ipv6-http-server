#include "common.h"

#include "cmd_options.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void error(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);

    exit(EXIT_FAILURE);
}

void log_msg(const char *msg, log_level_t log_level, va_list args) {
    if (log_level > options.log || options.log == -1)
        return;
    vprintf(msg, args);
    printf("\n");
}

void log_info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_msg(fmt, INFO, args);
    va_end(args);
}

void log_warn(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_msg(fmt, WARN, args);
    va_end(args);
}
