/**
 * @file syllabus.h
 * @brief Fichier d'en-tête commun à tous les étudiants du projet LMS.
 * Contient les définitions des structures et les prototypes des fonctions.
 * À ne pas modifier.
 */

#ifndef SYLLABUS_H
#define SYLLABUS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------------------------
 * STRUCTURES
 * ------------------------------------------------------------------------- */

/* Structure d'un mot-clé (liste chaînée) */
typedef struct MotCle {
    char *mot;                  /* Le mot-clé (chaîne allouée) */
    struct MotCle *suivant;
} MotCle;

/* Structure d'une question QCM (liste chaînée) */
typedef struct Question {
    char *texte;                /* Énoncé de la question */
    char *propositions[4];      /* Tableau de 4 chaînes (propositions) */
    int bonne_reponse;          /* Indice 0,1,2,3 de la bonne réponse */
    struct Question *suivant;
} Question;

/* Structure d'une leçon (liste chaînée) */
typedef struct Lecon {
    char *titre;                /* Titre de la leçon */
    MotCle *mots_cles;          /* Liste chaînée des mots-clés */
    Question *questions;        /* Liste chaînée des questions QCM */
    struct Lecon *suivant;
} Lecon;

/* Structure d'un chapitre (liste chaînée) */
typedef struct Chapitre {
    char *titre;                /* Titre du chapitre */
    Lecon *lecons;              /* Liste chaînée des leçons */
    struct Chapitre *suivant;
} Chapitre;

/* -------------------------------------------------------------------------
 * PROTOTYPES DES FONCTIONS (chaque étudiant implémente les siennes)
 * ------------------------------------------------------------------------- */

/* ---------- ÉTUDIANT 2 : Gestion des chapitres ---------- */
Chapitre* creerChapitre(const char *titre);
void ajouterChapitre(Chapitre **tete, const char *titre);
void supprimerChapitre(Chapitre **tete, const char *titre);
void renommerChapitre(Chapitre *chapitre, const char *nouveau_titre);
Chapitre* trouverChapitre(Chapitre *cours, const char *titre);

/* ---------- ÉTUDIANT 3 : Gestion des leçons et affichage récursif ---------- */
Lecon* creerLecon(const char *titre);
void ajouterLecon(Chapitre *chapitre, const char *titre);
void supprimerLecon(Chapitre *chapitre, const char *titre);
void renommerLecon(Lecon *lecon, const char *nouveau_titre);
void afficherCoursRecursif(Chapitre *cours, int niveau);
void libererCours(Chapitre *cours);      /* Récursif : libère tout le cours */

/* ---------- ÉTUDIANT 4 : Mots-clés et recherche ---------- */
void ajouterMotCle(Lecon *lecon, const char *mot);
void supprimerMotCle(Lecon *lecon, const char *mot);
void libererMotsCles(MotCle *liste);     /* Libère une liste de mots-clés */
int rechercherMotCle(Chapitre *cours, const char *mot);  /* Retourne 1 si trouvé */

/* ---------- ÉTUDIANT 5 : QCM (côté professeur et évaluation) ---------- */
Question* creerQuestion(const char *texte, const char *prop0, const char *prop1, const char *prop2, const char *prop3, int bonne);
void ajouterQuestion(Lecon *lecon, const char *texte, const char *p0, const char *p1, const char *p2, const char *p3, int bonne);
void supprimerQuestion(Lecon *lecon, int index);
int getNombreQuestions(Lecon *lecon);                   /* Retourne le nombre de questions */
Question* getQuestion(Lecon *lecon, int index);         /* Retourne la question à l'index donné */
int evaluerReponse(Question *q, int reponse_etudiant);  /* Retourne 1 si correct */
void libererQuestions(Question *liste);                  /* Libère une liste de questions */

/* ---------- ÉTUDIANT 6 : Menu étudiant et progression (n'utilise que les fonctions ci-dessus) ---------- */
/* L'étudiant 6 n'ajoute pas de prototypes dans syllabus.h, il crée ses propres fichiers.
   Il a juste besoin des fonctions des autres, déjà prototypées. */

/* ---------- ÉTUDIANT 1 : Intégrateur – prototypes pour la persistance et menus ---------- */
/* Ces fonctions sont implémentées par l'étudiant 3 et l'étudiant 1 ? Non :
   - exporterJSON, importerJSON : Étudiant 3 (mais on les met ici pour que tout le monde les connaisse)
   - menuProfesseur, menuEtudiant : respectivement Étudiant 1 et Étudiant 6
   On les déclare ici pour que le main.c puisse les appeler.
*/
int exporterJSON(const char *chemin, Chapitre *cours);
Chapitre* importerJSON(const char *chemin);
void menuProfesseur(Chapitre **cours);
void menuEtudiant(Chapitre *cours);

#endif /* SYLLABUS_H */