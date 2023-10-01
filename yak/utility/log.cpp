#include "log.h"

#include "common.h"

static void color_reset(FILE *stream) {
	printf("\x1b[0m");
}

void log_fatal(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
	printf("\x1b[31m[Fatal] ");

    vfprintf(stderr, format, args);
    putc('\n', stderr);
    va_end(args);

	color_reset(stderr);
    exit(1);
}

void log_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
	printf("\x1b[31m[Error] ");

    vfprintf(stderr, format, args);
    putc('\n', stderr);
    va_end(args);

	color_reset(stderr);
}

void log_dev(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
	printf("\x1b[33m");

    vfprintf(stdout, format, args);
    putc('\n', stdout);
    va_end(args);

	color_reset(stdout);
}

void log_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
	printf("\x1b[37m");

    vfprintf(stdout, format, args);
    putc('\n', stdout);
    va_end(args);

	color_reset(stdout);
}
