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

        if (strcmp(line->argv[0], "cd") == 0) {
            int dir_len = strlen(line->argv[1]);

            if (line->argv[1][dir_len - 1] == '\n') line->argv[1][dir_len - 1] = '\0';
            
            char local_dir_buffer[max_line_lenght - 100];
            getcwd(local_dir_buffer, sizeof(local_dir_buffer));
            strcat(local_dir_buffer, "/");
            strcat(local_dir_buffer, line->argv[1]);
            
            chdir(local_dir_buffer);
        }

        // Apenas para testes - Apagar depois
        char diretorio[100];
        getcwd(diretorio, 99);
        write(STDOUT_FILENO, diretorio, strlen(diretorio));

        free_cmd_line(line);
    }
}