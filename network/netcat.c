#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include "sock.h"


int main(int argc, char *argv[]) {
    signal(SIGINT, handle_sigint);

    Args *args = parse_args(argc, argv);
    if (args == NULL) exit(1);

    if (args->mode == 0) {
        struct init_socket *server = get_server_socket(args);
        if (server) {
            pthread_t t1;
            pthread_t t2;

            pthread_create(&t1, NULL, recv_data, server);
            pthread_create(&t2, NULL, send_data, server);

            while (!already_stoped()) {
                sleep(1);
            }

            shutdown_sock(server->client_fd);

            pthread_join(t1, NULL);
            pthread_join(t2, NULL);

            close_sock(server->client_fd);
            if (server->server_fd != -1)
                close_sock(server->server_fd);

            free(server);
            free_args(args);
            return 0;
        } else {
            free_args(args);
            return 0;
        }
    } else {
        struct init_socket *client = connect_to_server(args);
        if (client) {
            pthread_t t1;
            pthread_t t2;

            pthread_create(&t1, NULL, recv_data, client);
            pthread_create(&t2, NULL, send_data, client);

            while (!already_stoped()) {
                sleep(1);
            }

            shutdown_sock(client->client_fd);

            pthread_join(t1, NULL);
            pthread_join(t2, NULL);

            close_sock(client->client_fd);

            free(client);
            free_args(args);
        } else {
            free_args(args);
            return 0;
        }
    }
}