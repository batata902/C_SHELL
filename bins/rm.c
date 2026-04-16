#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

typedef struct args {
    bool is_folder;
} Args;

typedef struct fileinfos {
    char *path;
    bool is_dir;
    bool empty_dir;
    bool is_reg;

} FileInfos;

Args* parse_args(int argc, char *argv[]);
FileInfos* get_file_infos(char *path);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        write(STDOUT_FILENO, "error: missing file name. \nusage: rm <filename>\n", 49);
        exit(0);
    }

    if (argc == 2) {
        char *file_name = argv[1];
        FileInfos *infos = get_file_infos(file_name);
        if (!infos) return 0;
        if (infos->is_reg) {
            unlink(file_name);
            free(infos);
            return 0;
        }

        else {
            if (infos->empty_dir) {
                rmdir(file_name);
            } else {
                write(STDOUT_FILENO, "Ainda vamos implementar\n", 25);
                free(infos);
                return 0;
            }
        }
        free(infos);
    } else {
        Args *args = parse_args(argc, argv);
        
        free(args);
    }
    return 0;
}

Args* parse_args(int argc, char *argv[]) {
    Args* args = (Args*) malloc(sizeof(Args));
    if (!args) {
        write(STDOUT_FILENO, "Error: unknown error\n", 22);
        return NULL;
    }
    if (argc == 2) {
        args->is_folder = false;
        return args;
    }
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            args->is_folder = true;
            break;
        }
    }

}

FileInfos* get_file_infos(char *path) {
    struct stat buffer;
    FileInfos *infos = (FileInfos *) malloc(sizeof(FileInfos));
    if (!infos) return NULL;

    infos->path = path;
    infos->empty_dir = true;

    if (stat(path, &buffer) != 0) {
        write(STDOUT_FILENO, "Error: no such a file or a directory\n", 23);
        return NULL;
    }

    if (S_ISDIR(buffer.st_mode)) {
        infos->is_dir = true;
        infos->is_reg = false;

        DIR *dir = opendir(path);
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            } else {
                infos->empty_dir = false;
                break;
            }
        }
        return infos;
    }
    if (S_ISREG(buffer.st_mode)) {
        infos->is_dir = false;
        infos->is_reg = true;
        return infos;
    }
}