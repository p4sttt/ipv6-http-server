#include "cmd_options.h"

#include "common.h"
#include <getopt.h>
#include <stdlib.h>

cmd_options_t options;

void parse_cmd_options(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "p:t:l")) != -1) {
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
            options.log = 1;
            break;
        default:
            break;
        }
    }
}