#include "log.h"

#include "yakpch.h"

void log_fatal(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    vfprintf(stderr, format, args);
    putc('\n', stderr);
    va_end(args);
    exit(1);
}

void log_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    vfprintf(stderr, format, args);
    putc('\n', stderr);
    va_end(args);
}

void log_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    vfprintf(stdout, format, args);
    putc('\n', stdout);
    va_end(args);
}
