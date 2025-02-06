#ifndef SERVER_H_
#define SERVER_H_

typedef enum { FREE = 0, BUSY = 1 } d4thrd_status_t;

typedef struct {
    int sockfd;
    d4thrd_status_t status;
} d4thrd_t;

typedef struct {
    int sockfd;
    d4thrd_t *thrdpool;
} server_t;

extern server_t *srv;

void server_create();
void server_listen();

#endif