#ifndef DISTRIBUTED_N_CHANNEL_H
#define DISTRIBUTED_N_CHANNEL_H

#include "process.h"
#include "logger.h"

int create_channels(int processes_count);

int close_my_channels_ends(Process *me);

int close_unused_channels_ends(Process *me);

#endif //DISTRIBUTED_N_CHANNEL_H
