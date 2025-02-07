#include "cmd_options.h"

#include "common.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

cmd_options_t options;

void print_help() {
    printf("Usage: srv -p [port] -l [log_level] -t [thread_count]\n");
    printf("\tport: natural number from 1024 to 65535\n");
    printf("\tlog_level: 0 - INFO, 1 - WARN\n");
    printf("\tthreads_count: natural number from 1 to 100\n");
}

void parse_cmd_options(int argc, char **argv) {
    int opt;

    options.log = -1;
    options.port = 6969;
    options.threads_count = 10;

    optind = 1;
    while ((opt = getopt(argc, argv, "hp:t:l:")) != -1) {
        switch (opt) {
        case 'p':
            options.port = atoi(optarg);
            if (options.port < 1024 || options.port > 65535) {
                error("Invalid port number");
            }
            break;
        case 't':
            options.threads_count = atoi(optarg);
            if (options.threads_count < 1 || options.threads_count > 100) {
                error("Invalid threads count");
            }
            break;
        case 'l':
            options.log = (log_level_t) atoi(optarg);
            if (options.log < 0 || options.log > 1) {
                error("Invalid log_level number");
            }
            break;
        case 'h':
        default:
            print_help();
            exit(EXIT_SUCCESS);
            break;
        }
    }

    log_info("cmd options:\n\tport: %d\n\tlog_level: %d\n\tthreads_count: %d", options.port,
             options.log, options.threads_count);
}
