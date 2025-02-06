#include "server.h"
#include "cmd_options.h"

int main(int argc, char **argv) {

    parse_cmd_options(argc, argv);   
    server_create();
    server_listen();

    return 0;
}