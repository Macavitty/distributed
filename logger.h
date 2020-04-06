#ifndef DISTRIBUTED_N_LOGGER_H
#define DISTRIBUTED_N_LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include "common.h"

/*
 * %ld      - local if
 * from %d  - sender id
 * to %d    - receiver id
 * %s       - pipe end type: e.g. r-end or w-end
 */

static const char *const log_opened_channel_fmt =
        "[+] channel from %ld to %ld: r-end - %d, w-end - %d\n";

static const char *const log_closed_mine_channels_fmt =
        "[-] mine in %d: from %d to %d: %s - %d\n";

static const char *const log_closed_foreign_channels_fmt =
        "[-] not mine in %ld: from %ld to %ld: %s - %d\n";

extern FILE *events_log_f;
extern FILE *pipes_log_f;

int init_log();

int close_log();

int write_log(FILE *log_file, const char *log_format, ...);

#endif //DISTRIBUTED_N_LOGGER_H
