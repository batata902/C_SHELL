#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "argparser.h"
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>

char *generator(const char *text, int state) {
    static DIR *dir;
    static struct dirent *entry;

    if (state == 0) {
        dir = opendir(".");
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(entry->d_name, text, strlen(text)) == 0) {
            return strdup(entry->d_name);
        }
    }

    closedir(dir);
    return NULL;
}

// conecta o autocomplete
char **my_completion(const char *text, int start, int end) {
    return rl_completion_matches(text, generator);
}

int main(void) {
    signal(SIGINT, SIG_IGN);

    rl_attempted_completion_function = my_completion;

    system("clear");
    write(STDOUT_FILENO, "Welcome to JJ Shell - v1.0\n", 28);

    unsigned int max_line_lenght = get_max_cmd_lenght();
    char shell[] = "\n\033[34mjjshell@kali\033[m \033[32m~\033[m \033[34m$\033[m ";
    char *env = init_env();

    while (1) {
        char *buffer = readline(shell);
        if (!buffer) continue;

        if (strlen(buffer) == 0) {
            free(buffer);
            continue;
        }

        add_history(buffer);

        CMD_LINE *line = parse_args(buffer);

        execute_command(line, env);

        free_cmd_line(line);
        free(buffer);
    }
}