// progression.c
#include "progression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Terminee {
    char *cle;
    struct Terminee *suivant;
} Terminee;

static Terminee *listeTerminees = NULL;

static char* construireCle(const char *chap, const char *lecon) {
    char *cle = malloc(strlen(chap) + strlen(lecon) + 3);
    sprintf(cle, "%s|%s", chap, lecon);
    return cle;
}

void initialiserProgression() {
    // rien à faire en mémoire, juste la liste vide
}

void libererProgression() {
    while (listeTerminees) {
        Terminee *tmp = listeTerminees;
        listeTerminees = listeTerminees->suivant;
        free(tmp->cle);
        free(tmp);
    }
}

void marquerLeconTerminee(Chapitre *cours, const char *chapTitre, const char *leconTitre) {
    char *cle = construireCle(chapTitre, leconTitre);
    // déjà présente ?
    Terminee *cur = listeTerminees;
    while (cur) {
        if (strcmp(cur->cle, cle) == 0) {
            free(cle);
            return;
        }
        cur = cur->suivant;
    }
    Terminee *nouv = (Terminee*)malloc(sizeof(Terminee));
    nouv->cle = cle;
    nouv->suivant = listeTerminees;
    listeTerminees = nouv;
}

int estLeconTerminee(const char *chapTitre, const char *leconTitre) {
    char *cle = construireCle(chapTitre, leconTitre);
    Terminee *cur = listeTerminees;
    while (cur) {
        if (strcmp(cur->cle, cle) == 0) {
            free(cle);
            return 1;
        }
        cur = cur->suivant;
    }
    free(cle);
    return 0;
}

int getNombreTotalLecons(Chapitre *cours) {
    int total = 0;
    Chapitre *c = cours;
    while (c) {
        Lecon *l = c->lecons;
        while (l) { total++; l = l->suivant; }
        c = c->suivant;
    }
    return total;
}

void afficherProgression(Chapitre *cours) {
    int total = getNombreTotalLecons(cours);
    int terminees = 0;
    Chapitre *c = cours;
    while (c) {
        Lecon *l = c->lecons;
        while (l) {
            if (estLeconTerminee(c->titre, l->titre)) terminees++;
            l = l->suivant;
        }
        c = c->suivant;
    }
    printf("Progression : %d / %d leçons terminées (%.1f%%)\n",
           terminees, total, total ? (terminees*100.0/total) : 0);
}