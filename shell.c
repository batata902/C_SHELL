#include <unistd.h>
#include "argparser.h"
#include <stdlib.h>
#include <string.h>

int main(void) {
    system("clear");

    unsigned int max_line_lenght = get_max_cmd_lenght();
    char shell[] = "\njjshell@kali ~ $ ";

    while (1) {
        write(STDOUT_FILENO, shell, 18);

        char buffer[max_line_lenght];
        int n = read(STDIN_FILENO, buffer, max_line_lenght - 1);
        buffer[n] = '\0';

        CMD_LINE *line = parse_args(buffer);
        print_line(line);
        free_cmd_line(line);
    }
}