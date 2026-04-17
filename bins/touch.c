#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("Error: file name was not specified\n");
        return 1;
    }
    FILE *file = fopen(argv[1], "w");
    if (file == NULL) {
        printf("Error: error while creating file \"%s\"\n", argv[1]);
        return 1;
    }

    char local_path[256];
    getcwd(local_path, sizeof(local_path));

    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", local_path, argv[1]);

    printf("Created >> %s", full_path);
    fclose(file);
    
    return 0;
}