#include "syllabus.h"

void ajouterMotCle(Lecon *lecon, const char *mot) {
    if (!lecon) return;
    MotCle *nouveau = (MotCle*)malloc(sizeof(MotCle));
    nouveau->mot = strdup(mot);
    nouveau->suivant = lecon->mots_cles;
    lecon->mots_cles = nouveau;
}

void supprimerMotCle(Lecon *lecon, const char *mot) {
    if (!lecon || !lecon->mots_cles) return;
    MotCle *courant = lecon->mots_cles, *prec = NULL;
    while (courant && strcmp(courant->mot, mot) != 0) {
        prec = courant;
        courant = courant->suivant;
    }
    if (!courant) return;
    if (prec) prec->suivant = courant->suivant;
    else lecon->mots_cles = courant->suivant;
    free(courant->mot);
    free(courant);
}

void libererMotsCles(MotCle *liste) {
    while (liste) {
        MotCle *tmp = liste;
        liste = liste->suivant;
        free(tmp->mot);
        free(tmp);
    }
}

int rechercherMotCle(Chapitre *cours, const char *mot) {
    if (!cours) return 0;
    Lecon *l = cours->lecons;
    while (l) {
        MotCle *m = l->mots_cles;
        while (m) {
            if (strcmp(m->mot, mot) == 0) return 1;
            m = m->suivant;
        }
        l = l->suivant;
    }
    return rechercherMotCle(cours->suivant, mot);
}