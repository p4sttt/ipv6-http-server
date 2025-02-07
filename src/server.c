#include "server.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdalign.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cmd_options.h"
#include "common.h"
#include "http.h"

server_t *server_create() {
    int err;
    server_t *srv;
    struct sockaddr_in6 srv_addr;
    char srv_addrp[INET6_ADDRSTRLEN];

    srv = malloc(sizeof(*srv));

    srv->sockfd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    srv->thrdpool = malloc(sizeof(d4thrd_t) * options.threads_count);

    if (srv->sockfd == -1) {
        error("Failed to create socket");
    }

    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin6_family = AF_INET6;
    srv_addr.sin6_port = htons(options.port);
    srv_addr.sin6_addr = in6addr_loopback;

    err = bind(srv->sockfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if (err == -1) {
        error("Failed to bind sockfd to srv_addr");
    }

    inet_ntop(AF_INET6, &srv_addr.sin6_addr, srv_addrp, sizeof(srv_addrp));
    log_info("server created on address: [%s]:%d", srv_addrp, options.port);
    return srv;
}

void server_listen(server_t *srv) {
    int cnfd, err;
    struct sockaddr_in6 cnaddr;
    http_req_t http_req;
    http_res_t http_res;
    socklen_t socklen;
    char cnaddrp[INET6_ADDRSTRLEN], reqbuf[REQUEST_SIZE], resbuf[RESPONSE_SIZE] = "";

    err = listen(srv->sockfd, SOMAXCONN);
    if (err == -1) {
        error("Failed to change socket status to 'LISTEN'");
    }

    log_info("server is listening for connections");

    while (1) {
        cnfd = accept(srv->sockfd, (struct sockaddr *)&cnaddr, &socklen);
        if (cnfd == -1) {
            error("Failed to accept connection");
        }

        inet_ntop(AF_INET6, &cnaddr.sin6_addr, cnaddrp, sizeof(cnaddrp));
        log_info("accepted connection from: %s:%d", cnaddrp, ntohs(cnaddr.sin6_port));

        recv(cnfd, reqbuf, REQUEST_SIZE, 0);

        err = parse_http_req(reqbuf, &http_req);
        if (err == -1) {
            close(cnfd);
            continue;
        }

        http_res.line.version = "HTTP/1.0";
        http_res.line.status_code = 200;
        http_res.line.reason_phrase = "OK";

        err = load_html("index.html", &http_res.body);
        if (err == -1) {
            log_warn("failed to create body");
            send(cnfd, "HTTP/1.0 500 Internal Server Error\r\n\r\n", 36, 0);
            close(cnfd);
            continue;
        }

        err = compose_http_res(&http_res, resbuf);
        if (err == -1) {
            log_warn("failed to create response");
            send(cnfd, "HTTP/1.0 500 Internal Server Error\r\n\r\n", 36, 0);
        } else {
            send(cnfd, resbuf, RESPONSE_SIZE, 0);
        }

        close(cnfd);
    }

    close(srv->sockfd);
}
