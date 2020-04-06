#include "channel.h"

int create_channels(int processes_count) {
    for (int sender = 0; sender < processes_count; sender++) {
        for (int receiver = 0; receiver < processes_count; receiver++) {
            if (sender != receiver) {
                int pipe_fd[2];
                if (pipe(pipe_fd) < 0)
                    return -1;

                w_ends[sender][receiver] = pipe_fd[1];
                r_ends[sender][receiver] = pipe_fd[0];

                write_log(pipes_log_f, log_opened_channel_fmt, sender, receiver, pipe_fd[0], pipe_fd[1]);
            }
        }
    }
    return 0;
}

int close_my_channels_ends(Process *me) {
    for (int sender = 0; sender < me->processes_count; sender++) {
        if (sender != me->id) {
            close(r_ends[sender][me->id]);
            write_log(pipes_log_f, log_closed_mine_channels_fmt, me->id, sender, me->id, "r-end",
                      r_ends[sender][me->id]);
        }
    }
    for (int receiver = 0; receiver < me->processes_count; receiver++) {
        if (receiver != me->id) {
            close(w_ends[me->id][receiver]);
            write_log(pipes_log_f, log_closed_mine_channels_fmt, me->id, me->id, receiver, "w-end",
                      w_ends[me->id][receiver]);
        }
    }
    return 0;
}

int close_unused_channels_ends(Process *me) {
    for (int sender = 0; sender < me->processes_count; sender++) {
        for (int receiver = 0; receiver < me->processes_count; receiver++) {
            if (sender == receiver)
                continue;
            if ((sender != me->id && receiver != me->id)) {
                close(r_ends[sender][receiver]);
                close(w_ends[sender][receiver]);
                write_log(pipes_log_f,
                          log_closed_foreign_channels_fmt,
                          me->id, sender, receiver, "r-end", r_ends[sender][receiver]);
                write_log(pipes_log_f,
                          log_closed_foreign_channels_fmt,
                          me->id, sender, receiver, "w-end", w_ends[sender][receiver]);
            }
            if (sender == me->id) {
                close(r_ends[sender][receiver]);
                write_log(pipes_log_f,
                          log_closed_foreign_channels_fmt,
                          me->id, sender, receiver, "r-end", r_ends[sender][receiver]);
            }
            if (receiver == me->id) {
                close(w_ends[sender][receiver]);
                write_log(pipes_log_f,
                          log_closed_foreign_channels_fmt,
                          me->id, sender, receiver, "w-end", w_ends[sender][receiver]);
            }
        }
    }
    return 0;
}