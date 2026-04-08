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

void execute_command(CMD_LINE *line) {
    if (strcmp(line->argv[0], "cd") == 0) {
        int dir_len = strlen(line->argv[1]);

        if (line->argv[1][dir_len - 1] == '\n') line->argv[1][dir_len - 1] = '\0';
        
        char local_dir_buffer[MAX_COMMAND_LINE_SIZE - 100];
        getcwd(local_dir_buffer, sizeof(local_dir_buffer));
        strcat(local_dir_buffer, "/");
        strcat(local_dir_buffer, line->argv[1]);
        
        chdir(local_dir_buffer);
    }
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