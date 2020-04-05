#ifndef __IFMO_DISTRIBUTED_CLASS_COMMON__INFO_H
#define __IFMO_DISTRIBUTED_CLASS_COMMON__INFO_H

#include <unistd.h>
#include <stdbool.h>
#include "ipc.h"

extern int r_ends[MAX_PROCESS_ID][MAX_PROCESS_ID];
extern int w_ends[MAX_PROCESS_ID][MAX_PROCESS_ID];

typedef struct {
    local_id    id;                /// identifier of a process, parent has 0, children - [1 ; n]
    bool        is_parent;
    int         processes_count;   /// amount of processes in system including parent
} __attribute__((packed)) Process;

typedef enum {
    READ,
    WRITE
} EndType;

typedef struct {
    EndType type;
    int sender_id;
    int receiver_id;

} __attribute__((packed)) ChannelEnd;

#endif //__IFMO_DISTRIBUTED_CLASS_COMMON__INFO_H
