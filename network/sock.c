#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sock.h"

Args* parse_args(int argc, char *argv[]) {
    if (argc <= 1) {
        write(STDOUT_FILENO, "Error: NULL arguments\n", 23);
        return NULL;
    }

    Args *args = (Args*) malloc(sizeof(Args));
    if (!args) return NULL;

    args->target = NULL;
    args->port = -1;
    args->verbose = false;
    args->mode = 1;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            args->mode = 0;
        }
        if (strcmp(argv[i], "-p") == 0) {
            if (args->mode == 1) {
                write(STDOUT_FILENO, "Error: Client cannot choose port\n", 34);
                free(args);
                return NULL;
            }
            args->port = argv[i + 1];
        }
        if (strcmp(argv[i], "-v") == 0) {
            args->verbose = true;
        }
        if (strcmp(argv[i], "-t") == 0) {
            if (argv[i + 1][0] == '-') {
                write(STDOUT_FILENO, "Error: IP address was not given\n", 33);
                free(args);
                return NULL;
            }
            args->target = argv[i + 1];
        }
    }
    if (args->target != NULL && args->port == -1) {
        write(STDOUT_FILENO, "Error: target given but port was not given\n", 44);
        free(args);
        return NULL;
    }

    return args;
}

struct init_socket* get_server_socket(Args *args) {
    struct init_socket *s = (struct init_socket *) malloc(sizeof(struct init_socket));
    s->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server, client;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(args->port);

    bind(s->server_fd, (struct sockaddr *)&server, sizeof(server));
    listen(s->server_fd, 1);

    if (args->verbose) {
        printf("listening at 0.0.0.0:%d...", args->port);
    }

    socklen_t len = sizeof(client);
    s->client_fd = accept(s->server_fd, (struct sockaddr *)&client, &len);

    return s;
}

struct init_socket* connect_to_server(Args* args) {
    struct init_socket *client = (struct init_socket *) malloc(sizeof(struct init_socket));
    if (client == NULL) return NULL;

    client->server_fd = -1;

    client->client_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(args->target);
    server.sin_port = args->port;

    socklen_t len = sizeof(server);
    connect(client->client_fd, (struct sockaddr *)&server, &len);

    return client;
}