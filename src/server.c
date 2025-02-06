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

#define REQUEST_SIZE 1024

server_t *srv;

void server_create() {
    struct sockaddr_in6 srv_addr;
    int errno;
    srv = malloc(sizeof(*srv));

    srv->sockfd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    srv->thrdpool = malloc(sizeof(d4thrd_t) * 100);

    if (srv->sockfd == -1) {
        error("Failed to create socket");
    }

    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin6_family = AF_INET6;
    srv_addr.sin6_port = htons(options.port);
    srv_addr.sin6_addr = in6addr_loopback;

    errno = bind(srv->sockfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if (errno == -1) {
        error("Failed to bind sockfd to srv_addr");
    }
}

void server_listen() {
    int cnfd, errno;
    struct sockaddr_in6 cnaddr;
    socklen_t socklen;
    char cnaddrp[INET6_ADDRSTRLEN];
    char reqbuf[REQUEST_SIZE];

    errno = listen(srv->sockfd, SOMAXCONN);
    if (errno == -1) {
        error("Failed to change socket status to 'LISTEN'");
    }

    while (1) {
        cnfd = accept(srv->sockfd, (struct sockaddr *)&cnaddr, &socklen);
        if (cnfd == -1) {
            error("Failed to accept connection");
        }

        inet_ntop(AF_INET6, &cnaddr.sin6_addr, cnaddrp, sizeof(cnaddrp));
        printf("Connection accepted: %s\n", cnaddrp);

        recv(cnfd, reqbuf, REQUEST_SIZE, 0);
        printf("Request: %s\n", reqbuf);

        close(cnfd);
    }

    close(srv->sockfd);
}