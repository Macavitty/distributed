#include "logger.h"

int init_log() {
    if ((events_log_f = fopen(events_log, "a")) == NULL ||
        (pipes_log_f = fopen(pipes_log, "a")) == NULL)
        return -1;
    return 0;
}

int close_log() {
    if (fclose(events_log_f) == 0 &&
        fclose(pipes_log_f) == 0)
        return 0;
    return -1;
}

int write_log(FILE *log_file, const char *log_format, ...) {

    va_list arg_ptr;
    va_start(arg_ptr, log_format);

        if (vfprintf(log_file, log_format, arg_ptr) < 0)
            return -1;

    va_end(arg_ptr);
    return 0;
}
