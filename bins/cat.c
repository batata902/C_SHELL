#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void read_file(char *file_path) {
    FILE *file = fopen(file_path, "r");

    fseek(file, 0, SEEK_END);

    long file_len = ftell(file);

    rewind(file);

    char *buffer = (char *) malloc(file_len);
    fread(buffer, 1, file_len, file);

    write(STDOUT_FILENO, buffer, file_len);
    write(STDOUT_FILENO, "\n", 2);

    free(buffer);
    fclose(file);
}


int main(int argc, char *argv[]) {
    if (argc <= 1) {
        write(STDOUT_FILENO, "No such file\n", 14);
        return 1;
    }

    char *filename = argv[1];
    read_file(filename);
    return 0;
}