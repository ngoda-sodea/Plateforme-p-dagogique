#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void nettoyerBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void lireChaine(char *buffer, int taille) {
    fgets(buffer, taille, stdin);
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n')
        buffer[len-1] = '\0';
    else
        nettoyerBuffer();  // vide ce qui reste si la ligne est trop longue
}

char* lireLigne() {
    char *str = NULL;
    size_t size = 0;
    size_t len = 0;
    char buffer[256];

    do {
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            if (str) free(str);
            return NULL;
        }
        size_t buffer_len = strlen(buffer);
        // Allouer ou réallouer la mémoire
        char *new_str = realloc(str, size + buffer_len + 1);
        if (!new_str) {
            free(str);
            return NULL;
        }
        str = new_str;
        // Copier le buffer
        memcpy(str + size, buffer, buffer_len + 1);
        size += buffer_len;
        // Vérifier si on a lu toute la ligne (présence de '\n')
        if (buffer_len > 0 && buffer[buffer_len-1] == '\n') {
            str[size-1] = '\0';  // remplacer '\n' par '\0'
            break;
        }
    } while (1);

    return str;
}

int lireEntier() {
    int val;
    scanf("%d", &val);
    nettoyerBuffer();
    return val;
}