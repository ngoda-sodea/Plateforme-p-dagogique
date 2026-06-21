#include "syllabus.h"

Chapitre* creerChapitre(const char *titre) {
    Chapitre *c = (Chapitre*)malloc(sizeof(Chapitre));
    if (!c) return NULL;
    c->titre = strdup(titre);
    c->lecons = NULL;
    c->suivant = NULL;
    return c;
}

void ajouterChapitre(Chapitre **tete, const char *titre) {
    Chapitre *nouveau = creerChapitre(titre);
    if (!nouveau) return;
    if (*tete == NULL) {
        *tete = nouveau;
        return;
    }
    Chapitre *tmp = *tete;
    while (tmp->suivant) tmp = tmp->suivant;
    tmp->suivant = nouveau;
}

void supprimerChapitre(Chapitre **tete, const char *titre) {
    if (!tete || !*tete) return;
    Chapitre *courant = *tete, *prec = NULL;
    while (courant && strcmp(courant->titre, titre) != 0) {
        prec = courant;
        courant = courant->suivant;
    }
    if (!courant) return;
    if (prec == NULL) *tete = courant->suivant;
    else prec->suivant = courant->suivant;
    libererCours(courant);  // libère récursivement le chapitre et son contenu
}

void renommerChapitre(Chapitre *chapitre, const char *nouveau_titre) {
    if (chapitre) {
        free(chapitre->titre);
        chapitre->titre = strdup(nouveau_titre);
    }
}

Chapitre* trouverChapitre(Chapitre *cours, const char *titre) {
    Chapitre *tmp = cours;
    while (tmp) {
        if (strcmp(tmp->titre, titre) == 0) return tmp;
        tmp = tmp->suivant;
    }
    return NULL;
}