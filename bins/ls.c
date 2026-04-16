#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct args {
    bool list;
} Args;

typedef struct file {
    char file_name[100];
    struct file *next;
} File;

Args* parse_args(int argc, char *argv[]) {
    Args *args = (Args*) malloc(sizeof(Args));
    for (int i = 1; i < argc; i++) {
        argv[i][strcspn(argv[i], "\n")] = 0;
        if (strcmp(argv[i], "-l") == 0) args->list = true;
    }

    return args;
}

int starts_with(char *str, char c) {
    if (str == NULL) return 0;
    if (str[0] == c) return 1;
    return 0;
}

File* push_f(File *f, char *filename) {
    File *next = (File*) malloc(sizeof(File));
    if (next == NULL) {
        write(STDOUT_FILENO, "memory alloc failed\n", 21);
        exit(1);
    }

    strcpy(next->file_name, filename);
    next->next = NULL;

    if (f == NULL) {
        return next;
    }

    File *tmp = f;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    tmp->next = next;
    return f;
}

void print(File *f, char *color) {
    File *tmp = f;
    while (tmp != NULL) {
        char formated_buff[500];
        strcpy(formated_buff, color);
        strcat(formated_buff, tmp->file_name);
        strcat(formated_buff, "     ");
        strcat(formated_buff, "\033[m");
        write(STDOUT_FILENO, formated_buff, strlen(formated_buff));
        tmp = tmp->next;
    }
}

void free_f(File *f) {
    File *tmp;
    while (f != NULL) {
        tmp = f;
        f = f->next;
        free(tmp);
    }
}

int main(int argc, char *argv[]) {
    char *path;

    File *files = NULL;
    File *dirs = NULL;

    if (argc > 1) {
        if (starts_with(argv[1], '/'))
            path = argv[1];
        else
            path = ".";
    } else {
        path = ".";
    }

    DIR *dir = opendir(path);
    struct dirent *entry;

    Args *args = parse_args(argc, argv);

    if (args->list) {
        dirs = push_f(dirs, ".");
        dirs = push_f(dirs, "..");
    }

    while((entry = readdir(dir)) != NULL) {
        char *file_name = entry->d_name;

        if (entry->d_type == 4) {
            if (strcmp(file_name, ".") == 0 || strcmp(file_name, "..") == 0) continue;
            if (file_name[0] == '.') {
                if (args->list) {
                    push_f(dirs, file_name);
                } else {
                    continue;
                }
            }
            dirs = push_f(dirs, file_name);
            continue;
        }
        if (file_name[0] == '.') {
            if (args->list) {
                push_f(dirs, file_name);
            } else {
                continue;
            }
        }

        files = push_f(files, file_name);
    }

    print(dirs, "\033[34m");
    print(files, "\033[32m");

    free_f(files);
    free_f(dirs);
    closedir(dir);
    free(args);
    return 0;
}