#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
    char local_path[1024];
    getcwd(local_path, sizeof(local_path));

    write(STDOUT_FILENO, local_path, strlen(local_path));

    return 0;
} 