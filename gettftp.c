#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {  // Vérifie si 2 arguments sont passés
        printf("Usage: %s <host> <file>\n", argv[0]);
        return 1;
    }

    char *host = argv[1];
    char *file = argv[2];

    printf("Host: %s, File: %s\n", host, file);
    return 0;
}
