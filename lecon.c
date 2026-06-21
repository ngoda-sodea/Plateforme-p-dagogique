#include "syllabus.h"

Lecon* creerLecon(const char *titre) {
    Lecon *l = (Lecon*)malloc(sizeof(Lecon));
    if (!l) return NULL;
    l->titre = strdup(titre);
    l->mots_cles = NULL;
    l->questions = NULL;
    l->suivant = NULL;
    return l;
}

void ajouterLecon(Chapitre *chapitre, const char *titre) {
    if (!chapitre) return;
    Lecon *nouveau = creerLecon(titre);
    if (!nouveau) return;
    if (chapitre->lecons == NULL) {
        chapitre->lecons = nouveau;
        return;
    }
    Lecon *tmp = chapitre->lecons;
    while (tmp->suivant) tmp = tmp->suivant;
    tmp->suivant = nouveau;
}

void supprimerLecon(Chapitre *chapitre, const char *titre) {
    if (!chapitre || !chapitre->lecons) return;
    Lecon *courant = chapitre->lecons, *prec = NULL;
    while (courant && strcmp(courant->titre, titre) != 0) {
        prec = courant;
        courant = courant->suivant;
    }
    if (!courant) return;
    if (prec == NULL) chapitre->lecons = courant->suivant;
    else prec->suivant = courant->suivant;
    libererMotsCles(courant->mots_cles);
    libererQuestions(courant->questions);
    free(courant->titre);
    free(courant);
}

void renommerLecon(Lecon *lecon, const char *nouveau_titre) {
    if (lecon) {
        free(lecon->titre);
        lecon->titre = strdup(nouveau_titre);
    }
}

void afficherCoursRecursif(Chapitre *cours, int niveau) {
    if (!cours) return;
    for (int i = 0; i < niveau; i++) printf("  ");
    printf("- Chapitre : %s\n", cours->titre);
    Lecon *l = cours->lecons;
    while (l) {
        for (int i = 0; i < niveau + 1; i++) printf("  ");
        printf("- Leçon : %s\n", l->titre);
        l = l->suivant;
    }
    afficherCoursRecursif(cours->suivant, niveau);
}

void libererCours(Chapitre *cours) {
    if (!cours) return;
    libererCours(cours->suivant);
    Lecon *l = cours->lecons;
    while (l) {
        Lecon *tmp = l;
        l = l->suivant;
        libererMotsCles(tmp->mots_cles);
        libererQuestions(tmp->questions);
        free(tmp->titre);
        free(tmp);
    }
    free(cours->titre);
    free(cours);
}