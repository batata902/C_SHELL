#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("Error: Directory name was not given\n");
        return 1;
    }

    if (mkdir(argv[1], 0777) == 0) {
        printf("Created >> %s\n", argv[1]);
    } else {
        printf("Error: couldn't create %s dir\n", argv[1]);
        return 2;
    }
    return 0;
}