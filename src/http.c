#include "http.h"

#include "common.h"
#include "server.h"
#include <stddef.h>
#include <stdio.h>
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
    int pos = 0;

    pos += snprintf(res + pos, RESPONSE_SIZE - pos, "%s %d %s\r\n", http_res->line.version,
                    http_res->line.status_code, http_res->line.reason_phrase);
    pos += snprintf(res + pos, RESPONSE_SIZE - pos, "\r\n");
    if (http_res->body != 0) {
        pos += snprintf(res + pos, RESPONSE_SIZE - pos, "%s", http_res->body);
    }

    return 0;
}

int load_html(const char *name, char **body) {
    long size;
    FILE *file;

    char *path = malloc(strlen(name) + 7);
    if (path == NULL) {
        log_warn("failed to allocate memory for path");
        return -1;
    }

    sprintf(path, "public/%s", name);

    file = fopen(path, "rb");
    if (file == NULL) {
        log_warn("failed to open file: %s", path);
        return -1;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    (*body) = malloc(size + 1);
    if (*body == NULL) {
        log_warn("failed to allocate memory for body");
        return -1;
    }

    fread(*body, 1, size, file);
    (*body)[size] = '\0';

    fclose(file);

    return 0;
}
