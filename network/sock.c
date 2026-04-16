#include <sys/socket.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sock.h"

sem_t sem;

Args* parse_args(int argc, char *argv[]) {
    sem_init(&sem, 0, 1);
    if (argc <= 1) {
        write(STDOUT_FILENO, "Error: NULL arguments\n", 23);
        return NULL;
    }

    Args *args = (Args*) malloc(sizeof(Args));
    if (!args) return NULL;
    
    args->target = NULL;

    args->port = -1;
    args->verbose = false;
    args->is_listening = false;
    args->mode = 1;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            args->mode = 0;
            args->is_listening = true;
            printf("Escuta ativa\n");
            continue;
        }
        if (strcmp(argv[i], "-p") == 0) {
            if (args->mode == 1) {
                write(STDOUT_FILENO, "Error: Client cannot choose port\n", 34);
                free(args);
                return NULL;
            }
            args->port = atoi(argv[i + 1]);
            continue;
        }
        if (strcmp(argv[i], "-v") == 0) {
            args->verbose = true;
            printf("verbose ativo\n");
            continue;
        }
        if (strcmp(argv[i], "-t") == 0) {
            if (args->is_listening == true) {
                printf("Error: you can't listen and connect at the same time\n");
                return NULL;
            }
            if (argv[i + 1][0] == '-' || argc >= i + 1) {
                write(STDOUT_FILENO, "Error: IP address was not given\n", 33);
                free(args);
                return NULL;
            }
            args->target = argv[i + 1];
            continue;
        }
    }
    if (args->port == -1) {
        write(STDOUT_FILENO, "Error: port was not given\n", 27);
        free(args);
        return NULL;
    }

    return args;
}

void free_args(Args *args) {
    free(args);
}

void print(char *line) {
    sem_wait(&sem);
    write(STDOUT_FILENO, line, strlen(line));
    sem_post(&sem);
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
        printf("listening at 0.0.0.0:%d...\n", args->port);
    }

    socklen_t len = sizeof(client);
    s->client_fd = accept(s->server_fd, (struct sockaddr *)&client, &len);

    printf("Connection received!\n");

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
    connect(client->client_fd, (struct sockaddr *)&server, len);

    return client;
}


void* recv_data(void *sock) {
    struct init_socket *s = (struct init_socket *) sock;
    while(1) {
        char buffer[1024];
        int n = read(s->client_fd, buffer, sizeof(buffer) - 1);
        buffer[n] = '\0';

        print(buffer);
        continue;

    }
}

void* send_data(void *sock) {
    struct init_socket *s = (struct init_socket *) sock;

    while (1) {
        char buffer[1024];
        
        int n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        buffer[n] = '\0';

        if (strcmp(buffer, "exit\n") == 0) exit(0); // falta liberar memoria aqui

        write(s->client_fd, buffer, strlen(buffer));
    }
}