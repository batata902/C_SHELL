#ifndef SOCK_H
#define SOCK_H

#include <stdbool.h>

struct init_socket {
    int server_fd;
    int client_fd;
};

typedef struct args {
    int mode; // 0 -> Server | 1 -> Client
    int port;
    bool verbose; // true: mostra | false: Nao mostra
    char *target;
} Args;

struct init_socket* get_server_socket(Args* args);

struct init_socket* connect_to_server(Args* args);

Args* parse_args(int argc, char *argv[]);


#endif