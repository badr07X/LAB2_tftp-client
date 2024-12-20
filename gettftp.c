#include <stdio.h>
#include <stdlib.h>




int main(int argc, char *argv[]) {
    // Vérification du nombre d'arguments
    if (argc != 3) {
        printf("Usage: %s <host> <file>\n", argv[0]);
        return 1;  // Retourne une erreur si les arguments sont manquants
    }

    // Récupération des arguments
    char *host = argv[1];
    char *file = argv[2];

    // Affichage des arguments pour validation
    printf("Host: %s\n", host);
    printf("File: %s\n", file);

    return 0;  
}
