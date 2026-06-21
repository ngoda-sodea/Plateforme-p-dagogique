#include "syllabus.h"

Question* creerQuestion(const char *texte, const char *prop0, const char *prop1,
                        const char *prop2, const char *prop3, int bonne) {
    Question *q = (Question*)malloc(sizeof(Question));
    if (!q) return NULL;
    q->texte = strdup(texte);
    q->propositions[0] = strdup(prop0);
    q->propositions[1] = strdup(prop1);
    q->propositions[2] = strdup(prop2);
    q->propositions[3] = strdup(prop3);
    q->bonne_reponse = bonne;
    q->suivant = NULL;
    return q;
}

void ajouterQuestion(Lecon *lecon, const char *texte, const char *p0,
                     const char *p1, const char *p2, const char *p3, int bonne) {
    if (!lecon) return;
    Question *nouveau = creerQuestion(texte, p0, p1, p2, p3, bonne);
    if (!nouveau) return;
    if (!lecon->questions) {
        lecon->questions = nouveau;
        return;
    }
    Question *tmp = lecon->questions;
    while (tmp->suivant) tmp = tmp->suivant;
    tmp->suivant = nouveau;
}

void supprimerQuestion(Lecon *lecon, int index) {
    if (!lecon || !lecon->questions || index < 0) return;
    Question *courant = lecon->questions, *prec = NULL;
    for (int i = 0; courant && i < index; i++) {
        prec = courant;
        courant = courant->suivant;
    }
    if (!courant) return;
    if (prec) prec->suivant = courant->suivant;
    else lecon->questions = courant->suivant;
    free(courant->texte);
    for (int i = 0; i < 4; i++) free(courant->propositions[i]);
    free(courant);
}

int getNombreQuestions(Lecon *lecon) {
    int count = 0;
    Question *q = lecon ? lecon->questions : NULL;
    while (q) { count++; q = q->suivant; }
    return count;
}

Question* getQuestion(Lecon *lecon, int index) {
    if (!lecon) return NULL;
    Question *q = lecon->questions;
    for (int i = 0; q && i < index; i++) q = q->suivant;
    return q;
}

int evaluerReponse(Question *q, int reponse_etudiant) {
    return (q && reponse_etudiant == q->bonne_reponse) ? 1 : 0;
}

void libererQuestions(Question *liste) {
    while (liste) {
        Question *tmp = liste;
        liste = liste->suivant;
        free(tmp->texte);
        for (int i = 0; i < 4; i++) free(tmp->propositions[i]);
        free(tmp);
    }
}