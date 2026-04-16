#include <unistd.h>
#include "argparser.h"
#include <stdlib.h>
#include <string.h>

int main(void) {
    system("clear");

    unsigned int max_line_lenght = get_max_cmd_lenght();
    char shell[] = "\n\033[34mjjshell@kali\033[m \033[32m~\033[m \033[34m$\033[m ";
    char *env = init_env();

    while (1) {
        write(STDOUT_FILENO, shell, 43);

        char buffer[max_line_lenght];
        int n = read(STDIN_FILENO, buffer, max_line_lenght - 1);
        if (n <= 0) continue;
        if (buffer[n - 1] == '\n') buffer[n - 1] = '\0';
        else buffer[n] = '\0';

        CMD_LINE *line = parse_args(buffer);

        execute_command(line, env);

        free_cmd_line(line);
    }
}