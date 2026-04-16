#include <stdio.h>
#include "sock.h"

int main(int argc, char *argv[]) {
    Args *args = parse_args(argc, argv);
    if (args == NULL) exit(1);

    if (args->mode == 0) {
        struct init_socket *server = get_server_socket(args);

        free(server);
        return 0;
    } else {
        struct init_socket *client = connect_to_server(args);

        free(client);
        return 0;
    }
}