#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_THREADS_COUNT 2

struct my_thread {
    pthread_t pthread;
    int is_active;
};

struct handle_conn_args {
    int connfd;
    struct sockaddr_in6 conn_addr;
    int* is_active;
};

void error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(EXIT_FAILURE);
}

void *handle_conn(void *arg) {
    struct handle_conn_args *args = arg;
    char addrp[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &args->conn_addr.sin6_addr, addrp, INET6_ADDRSTRLEN);
    printf("Created socket connection: %s:%d\n", addrp, ntohs(args->conn_addr.sin6_port));
    sleep(15); // tipa chto-to proishodit
    *args->is_active = 0;
    close(args->connfd);
    return NULL;
}

int main(int argc, char **argv) {
    int srvfd, port, connfd;
    struct my_thread *thread_pool;
    struct sockaddr_in6 srv_addr;

    if (argc < 2 || (port = atoi(argv[1])) == 0) {
        error("Usage:\n\t./srv [port]");
    }

    if ((srvfd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        error("Failed to create socket");
    }

    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin6_family = AF_INET6;
    srv_addr.sin6_port = htons(port);
    srv_addr.sin6_addr = in6addr_loopback;

    if (bind(srvfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) == -1) {
        error("Failed to bind sockfd to srv_addr");
    }

    if (listen(srvfd, SOMAXCONN) == -1) {
        error("Failed to change socket status to 'LISTEN'");
    }

    thread_pool = malloc(sizeof(*thread_pool) * MAX_THREADS_COUNT);

    while (1) {
        struct sockaddr_in6 conn_addr;
        socklen_t conn_addr_sz = sizeof(conn_addr);
        connfd = accept(srvfd, (struct sockaddr *)&conn_addr, &conn_addr_sz);
        if (connfd == -1) {
            error("Failed to accept connection");
        } else {
            int free_thread_idx = 0;
            while (thread_pool[free_thread_idx].is_active == 1)
                free_thread_idx = (free_thread_idx + 1) % MAX_THREADS_COUNT;

            struct handle_conn_args *args = malloc(sizeof(*args)); 
            args->connfd = connfd;
            args->conn_addr = conn_addr;
            args->is_active = &thread_pool[free_thread_idx].is_active;
            thread_pool[free_thread_idx].is_active = 1;
            pthread_create(&thread_pool[free_thread_idx].pthread, NULL, handle_conn, (void *)args);
        }
    }

    free(thread_pool);
    close(srvfd);
    return 0;
}
