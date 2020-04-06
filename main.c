#include <getopt.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "ipc.h"
#include "pa1.h"
#include "process.h"
#include "channel.h"
#include "logger.h"

int r_ends[MAX_PROCESS_ID][MAX_PROCESS_ID]; // i - sender; j - receiver
int w_ends[MAX_PROCESS_ID][MAX_PROCESS_ID]; // i - sender; j - receiver
FILE *events_log_f;
FILE *pipes_log_f;

int get_child_process_num(int argc, char **argv);

int run_child(Process *p);

int synchronize(Process *p, MessageType msg_t, FILE *log_file, const char *msg_fmt, const char *log_receiver_fmt);

int work();

int main(int argc, char *argv[]) {

    int children_count = 0;
    if ((children_count = get_child_process_num(argc, argv)) == -1) {
        exit(EXIT_FAILURE);
    }

    init_log();
    create_channels(children_count + 1);

    Process process = {
            .id = PARENT_ID,
            .is_parent = true,
            .processes_count = children_count + 1
    };

    // create children
    for (int i = 0; i < children_count; i++) {

        if (process.is_parent) {
            pid_t fork_pid = fork();
            if (fork_pid == 0) { // if child
                process.id = i + 1; // set correct local id
                process.is_parent = false;
                close_unused_channels_ends(&process);
                run_child(&process);
                close_my_channels_ends(&process);
                exit(EXIT_SUCCESS);
            }
        }
    }

    close_unused_channels_ends(&process);

    // get all started
    Message msg;
    msg.s_header.s_type = STARTED;
    receive_any(&process, &msg);
    write_log(events_log_f, log_received_all_started_fmt, process.id);
    write_log(stdout, log_received_all_started_fmt, process.id);

    // get all done
    msg.s_header.s_type = DONE;
    receive_any(&process, &msg);
    write_log(events_log_f, log_received_all_done_fmt, process.id);
    write_log(stdout, log_received_all_done_fmt, process.id);

    close_my_channels_ends(&process);
    close_log();

    // wait for all children
    for (int i = 0; i < process.processes_count; i++) {
        wait(0);
    }

    return EXIT_SUCCESS;
}

int get_child_process_num(int argc, char **argv) {
    const char *opt_keys = "p:";
    int opt;
    while ((opt = getopt(argc, argv, opt_keys)) != -1) {
        if (opt == 'p') {
            return (int) strtol(optarg, 0, 10);
        }
        return -1;
    }
    return 0;
}

int run_child(Process *p) {

    write_log(events_log_f, log_started_fmt, p->id, getpid(), getppid());
    write_log(stdout, log_started_fmt, p->id, getpid(), getppid());

    // synchronize on start
    synchronize(p, STARTED, events_log_f, log_started_fmt, log_received_all_started_fmt);

    // do smth
    work();
    write_log(events_log_f, log_done_fmt, p->id);
    write_log(stdout, log_done_fmt, p->id);

    // synchronize on start
    synchronize(p, DONE, events_log_f, log_done_fmt, log_received_all_done_fmt);
    return 0;
}

int
synchronize(Process *p, MessageType msg_t, FILE *log_file, const char *msg_fmt, const char *log_receiver_fmt) {

    Message msg;
    sprintf(msg.s_payload, msg_fmt, p->id, getpid(), getppid());

    MessageHeader header = {
            .s_magic = MESSAGE_MAGIC,
            .s_type = msg_t,
            .s_local_time = time(0),
            .s_payload_len = strlen(msg.s_payload)
    };
    msg.s_header = header;

    // send to anyone who would listen
    send_multicast(p, &msg);

    // receive  from others
    Message oth_msg;
    MessageHeader oth_header = header;
    oth_msg.s_header = oth_header;
    receive_any(p, &oth_msg);

    write_log(log_file, log_receiver_fmt, p->id);
    write_log(stdout, log_receiver_fmt, p->id);

    return 0;
}

int work() {
    // in light of recent events this process would not risk it`s life going to work
    // and would stick to self isolation regime
    return 0;
}




