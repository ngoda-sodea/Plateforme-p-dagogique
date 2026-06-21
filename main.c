#include "syllabus.h"
#include "menu.h"
#include "progression.h"
#include <stdio.h>

int main() {
    Chapitre *cours = NULL;
    initialiserProgression();
    int choix;
    do {
        printf("\n=== PLATEFORME LMS ===\n");
        printf("1. Espace Professeur\n");
        printf("2. Espace Étudiant\n");
        printf("0. Quitter\n");
        choix = 0;
        scanf("%d", &choix);
        while (getchar() != '\n');
        switch (choix) {
            case 1: menuProfesseur(&cours); break;
            case 2: menuEtudiant(cours); break;
        }
    } while (choix != 0);
    libererCours(cours);
    libererProgression();
    return 0;
}