#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        write(STDOUT_FILENO, "usage: rm <filename>\n", 22);
        exit(0);
    }
    char *file_name = argv[0];
    unlink(file_name);
    return 0;
}