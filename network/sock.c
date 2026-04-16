#include <sys/socket.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "sock.h"

volatile sig_atomic_t stop = 0;
int global_server_fd = -1;
sem_t sem;

void handle_sigint(int sig) {
    close(global_server_fd);
    stop = 1;
}

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
            continue;
        }
        if (strcmp(argv[i], "-p") == 0) {
            if (argv[i + 1][0] == '-' || argc <= i + 1) {
                write(STDOUT_FILENO, "Error: Missing port number\n", 28);
                free(args);
                return NULL;
            }
            args->port = atoi(argv[i + 1]);
            continue;
        }
        if (strcmp(argv[i], "-v") == 0) {
            args->verbose = true;
            continue;
        }
        if (strcmp(argv[i], "-t") == 0) {
            if (args->is_listening == true) {
                printf("Error: you can't listen and connect at the same time\n");
                return NULL;
            }
            if (argv[i + 1][0] == '-' || argc <= i + 1) {
                write(STDOUT_FILENO, "Error: IP address was not given\n", 33);
                free(args);
                return NULL;
            }
            args->target = argv[i + 1];
            printf("TARGET: %s\n", args->target);
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
    global_server_fd = s->server_fd;
    struct sockaddr_in server, client;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(args->port);

    bind(s->server_fd, (struct sockaddr *)&server, sizeof(server));
    listen(s->server_fd, 1);

    if (args->verbose) {
        printf("listening on [any] at port %d...\n", args->port);
    }

    socklen_t len = sizeof(client);
    s->client_fd = accept(s->server_fd, (struct sockaddr *)&client, &len);
    if (s->client_fd < 0) {
        if (stop) {
            free(s);
            return NULL;
        }
        perror("Error");
        free(s);
        return NULL;
    }

    printf("Connection received from [%s] port %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

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
    server.sin_port = htons(args->port);

    socklen_t len = sizeof(server);
    int status = connect(client->client_fd, (struct sockaddr *)&server, len);
    if (status < 0) {
        printf("Error: Couldn't connect\n");
        free(client);
        return NULL;
    }
    printf("Connected to %s at %d port\n", args->target, args->port);

    return client;
}


void* recv_data(void *sock) {
    struct init_socket *s = (struct init_socket *) sock;
    while(!stop) {
        char buffer[1024];
        int n = read(s->client_fd, buffer, sizeof(buffer) - 1);
        if (n <= 0) break;
        buffer[n] = '\0';

        print(buffer);
        continue;

    }
    return NULL;
}

void* send_data(void *sock) {
    struct init_socket *s = (struct init_socket *) sock;

    while (!stop) {
        char buffer[1024];
        
        int n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        if (n <= 0) break;
        
        buffer[n] = '\0';

        if (strcmp(buffer, "exit\n") == 0) {
            stop = 1;
            break;
        } 

        write(s->client_fd, buffer, strlen(buffer));
    }
    return NULL;
}

void close_sock(int sock) {
    close(sock);
}

void shutdown_sock(int sock_fd) {
    shutdown(sock_fd, SHUT_RDWR);
}

bool already_stoped() {
    if (stop) return true;
    return false;
}