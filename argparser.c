#include "argparser.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

const unsigned int MAX_COMMAND_LINE_SIZE = 5028;
const unsigned short int MAX_ARGS = 65;

unsigned int get_max_cmd_lenght() {
    return MAX_COMMAND_LINE_SIZE;
}

char* init_env() {
    setenv("BINS", "/home/bins", 1);
    char *dir = getenv("BINS");
    if (dir == NULL) {
        exit(1);
    }
    return dir;
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
    line->argv[counter] = NULL;
    return line;
}

void execute_command(CMD_LINE *line, char *env) {
    if (line->argc == 0) return;

    if (strcmp(line->argv[0], "exit") == 0) exit(0);

    if (strcmp(line->argv[0], "clear") == 0) {
        system("clear");
        return;
    }

    if (strcmp(line->argv[0], "cd") == 0) {
        if (line->argc < 2) {
            chdir(getenv("HOME")); // se nenhum argumento
        } else {
            if (chdir(line->argv[1]) != 0) {
                perror("cd failed");
                return;
            }
        }
    } else {
        pid_t pid = fork();
        if (pid < 0) {
            write(STDOUT_FILENO, "couldn't execute\n", 18);
        }
        else if (pid == 0) {
            char full_path[1024];
            if (env[strlen(env) - 1] == '/')
                snprintf(full_path, sizeof(full_path), "%s%s", env, line->argv[0]);
            else 
                snprintf(full_path, sizeof(full_path), "%s/%s", env, line->argv[0]);
            execv(full_path, line->argv);
            perror("error"); 
            exit(1);
        } else {
            waitpid(pid, NULL, 0);
        }
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