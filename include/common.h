#ifndef COMMON_H_
#define COMMON_H_

#include <stdarg.h>

typedef enum {
    WARN = 0,
    INFO = 1
} log_level_t;

void error(const char *fmt, ...);
void log_info(const char *fmt, ...);
void log_warn(const char *fmt, ...);

#endif
