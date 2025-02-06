#ifndef CMD_OPTIONS_H_
#define CMD_OPTIONS_H_

typedef struct {
    int port;
    int threads_count;
    int log;
} cmd_options_t;

extern cmd_options_t options;

void parse_cmd_options(int argc, char **argv);

#endif