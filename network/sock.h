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
    bool is_listening;
    bool verbose; // true: mostra | false: Nao mostra
    char *target;
} Args;

void handle_sigint(int sig);

struct init_socket* get_server_socket(Args* args);

struct init_socket* connect_to_server(Args* args);

void* recv_data(void *sock);

void* send_data(void *sock);

Args* parse_args(int argc, char *argv[]);

void free_args(Args *args);

void shutdown_sock(int sock_fd);

void close_sock(int sock);

bool already_stoped();

#endif