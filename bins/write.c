#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Error: Missing parameters\nUsage: write file.txt \"hello world!\"");
        return 1;
    }

    FILE *file = fopen(argv[1], "w");
    if (file == NULL) {
        printf("Error: Couldn't open file %s\n", argv[1]);
        return 1;
    }
    char buffer[5048];

    for (int i = 2; i < argc; i++) {
        size_t remaining = sizeof(buffer) - strlen(buffer) - 1;

        if (remaining > 0 && i > 2) {
            strncat(buffer, " ", remaining);
        }
        
        remaining = sizeof(buffer) - strlen(buffer) - 1;

        if (remaining > 0) {
            strncat(buffer, argv[i], remaining);
        }
    }

    fprintf(file, buffer);
    fclose(file);

    return 0;
}