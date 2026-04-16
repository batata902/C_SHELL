#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "sock.h"

int main(int argc, char *argv[]) {
    Args *args = parse_args(argc, argv);
    if (args == NULL) exit(1);

    if (args->mode == 0) {
        struct init_socket *server = get_server_socket(args);

        pthread_t t1;
        pthread_t t2;

        pthread_create(&t1, NULL, recv_data, server);
        pthread_create(&t2, NULL, send_data, server);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        free(server);
        free_args(args);
        return 0;
    } else {
        struct init_socket *client = connect_to_server(args);

        pthread_t t1;
        pthread_t t2;

        pthread_create(&t1, NULL, recv_data, client);
        pthread_create(&t2, NULL, send_data, client);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        free(client);
        free_args(args);
        return 0;
    }
}