#include "argparser.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

const unsigned int MAX_COMMAND_LINE_SIZE = 5028;
const unsigned short int MAX_ARGS = 65;

unsigned int get_max_cmd_lenght() {
    return MAX_COMMAND_LINE_SIZE;
}

CMD_LINE* parse_args(char str[]) {
    CMD_LINE *line = (CMD_LINE *)malloc(sizeof(CMD_LINE) + MAX_ARGS * sizeof(char *));
    line->argc = 0;

    int counter = 0;
    for (char *buffer = strtok(str, " "); buffer != NULL; buffer = strtok(NULL, " ")) {
        line->argv[counter] = (char *)malloc(strlen(buffer) + 1);
        strcpy(line->argv[counter], buffer);
        line->argc++;
        counter++;
    }
    return line;
}

void print_line(CMD_LINE *line) {
    for (int i = 0; i < line->argc; i++) {
        write(STDOUT_FILENO, line->argv[i], strlen(line->argv[i]));
        write(STDOUT_FILENO, " ", 2);
    }
}

void free_cmd_line(CMD_LINE *line) {
    for (int i = 0; i < line->argc; i++) {
        free(line->argv[i]);
    }
    free(line);
}