#include "http.h"

#include "common.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int parse_http_req(char *req, http_req_t *http_req) {
    char *line = strtok(req, "\r\n");
    char *method = strtok(line, " ");
    if (strcmp(method, "GET") == 0) {
        http_req->line.method = GET;
    } else if (strcmp(method, "POST") == 0) {
        http_req->line.method = POST;
    } else if (strcmp(method, "HEAD") == 0) {
        http_req->line.method = HEAD;
    } else {
        log_warn("unkown method: %s", method);
        return -1;
    }

    http_req->line.uri = strtok(NULL, " ");
    http_req->line.version = strtok(NULL, " ");

    return 0;
}

int compose_http_res(http_res_t *http_res, char *res) {
    return 0;
}
