#include "server.h"
#include "cmd_options.h"

int main(int argc, char **argv) {
    server_t *srv;

    parse_cmd_options(argc, argv);   

    srv = server_create();
    server_listen(srv);

    return 0;
}

