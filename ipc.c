#include <unistd.h>
#include <stdio.h>
#include <asm/errno.h>
#include "ipc.h"
#include "process.h"
#include <sys/ioctl.h>
#include <errno.h>

#define RESULT_SUCCESS 0
#define RESULT_FAIL -1

int send(void *self, local_id dst, const Message *msg) {
    Process *p = ((Process *) self);
    size_t size = sizeof(MessageHeader) + msg->s_header.s_payload_len;
    write(w_ends[p->id][dst], msg, size);
    return RESULT_SUCCESS;
}


int send_multicast(void *self, const Message *msg) {
    Process *p = ((Process *) self);
    for (int i = 0; i < p->processes_count; i++) {
        if (i != p->id) {
            if (send(self, i, msg) < 0)
                return RESULT_FAIL;
        }
    }
    return RESULT_SUCCESS;
}

int receive(void *self, local_id from, Message *msg) {
    Process *p = ((Process *) self);

    size_t header_size = sizeof(MessageHeader);

    if (read(r_ends[from][p->id], &(msg->s_header), header_size) != header_size) {
        return RESULT_FAIL;
    }

    size_t msg_len = msg->s_header.s_payload_len;

    if ((read(r_ends[from][p->id], msg->s_payload, msg_len)) != msg_len) {
        return RESULT_FAIL;
    }

    return RESULT_SUCCESS;
}

int receive_any(void *self, Message *msg) {
    Process *p = ((Process *) self);

    int received = 0;
    MessageType type = msg->s_header.s_type; // type of message we are waiting for
    while(received < p->processes_count - 2){ // not counting this proc and parent
        for (int i = 1; i < p->processes_count; i++) {
            if (i != p->id)
                if (receive(self, i, msg) == RESULT_SUCCESS && msg->s_header.s_type == type)
                    received++;
        }
    }
    return RESULT_SUCCESS;
}