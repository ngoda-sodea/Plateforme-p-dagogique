// menu.c
#include "menu.h"
#include "utils.h"
#include "progression.h"
#include <stdio.h>
#include <stdlib.h>

static void afficherChapitres(Chapitre *cours) {
    if (!cours) { printf("Aucun chapitre.\n"); return; }
    Chapitre *tmp = cours;
    int idx = 1;
    while (tmp) {
        printf("%d. %s\n", idx++, tmp->titre);
        tmp = tmp->suivant;
    }
}

static void afficherLecons(Chapitre *ch) {
    if (!ch || !ch->lecons) { printf("Aucune leçon dans ce chapitre.\n"); return; }
    Lecon *l = ch->lecons;
    int idx = 1;
    while (l) {
        printf("%d. %s\n", idx++, l->titre);
        l = l->suivant;
    }
}

static void menuGererLecons(Chapitre *ch) {
    if (!ch) return;
    int choix;
    do {
        printf("\n--- Gestion des leçons pour '%s' ---\n", ch->titre);
        printf("1. Ajouter une leçon\n2. Supprimer une leçon\n3. Renommer une leçon\n");
        printf("4. Gérer mots-clés d'une leçon\n5. Gérer QCM d'une leçon\n0. Retour\n");
        choix = lireEntier();
        switch (choix) {
            case 1: {
                printf("Titre de la leçon : ");
                char titre[100];
                lireChaine(titre, 100);
                ajouterLecon(ch, titre);
                break;
            }
            case 2: {
                afficherLecons(ch);
                printf("Numéro de la leçon à supprimer : ");
                int n = lireEntier();
                Lecon *l = ch->lecons;
                for (int i = 1; l && i < n; i++) l = l->suivant;
                if (l) supprimerLecon(ch, l->titre);
                break;
            }
            case 3: {
                afficherLecons(ch);
                printf("Numéro de la leçon à renommer : ");
                int n = lireEntier();
                Lecon *l = ch->lecons;
                for (int i = 1; l && i < n; i++) l = l->suivant;
                if (l) {
                    printf("Nouveau titre : ");
                    char nt[100];
                    lireChaine(nt, 100);
                    renommerLecon(l, nt);
                }
                break;
            }
            case 4: {
                afficherLecons(ch);
                printf("Numéro de la leçon : ");
                int n = lireEntier();
                Lecon *l = ch->lecons;
                for (int i = 1; l && i < n; i++) l = l->suivant;
                if (l) {
                    int sc;
                    do {
                        printf("Mots-clés : 1. Ajouter  2. Supprimer  0. Retour\n");
                        sc = lireEntier();
                        if (sc == 1) {
                            printf("Mot : ");
                            char mot[100];
                            lireChaine(mot, 100);
                            ajouterMotCle(l, mot);
                        } else if (sc == 2) {
                            printf("Mot à supprimer : ");
                            char mot[100];
                            lireChaine(mot, 100);
                            supprimerMotCle(l, mot);
                        }
                    } while (sc != 0);
                }
                break;
            }
            case 5: {
                afficherLecons(ch);
                printf("Numéro de la leçon : ");
                int n = lireEntier();
                Lecon *l = ch->lecons;
                for (int i = 1; l && i < n; i++) l = l->suivant;
                if (l) {
                    int sq;
                    do {
                        printf("QCM : 1. Ajouter  2. Supprimer  3. Lister  0. Retour\n");
                        sq = lireEntier();
                        if (sq == 1) {
                            char texte[200];
                            char p0[100], p1[100], p2[100], p3[100];
                            int bonne;
                            printf("Énoncé : "); lireChaine(texte, 200);
                            printf("Proposition 1 : "); lireChaine(p0, 100);
                            printf("Proposition 2 : "); lireChaine(p1, 100);
                            printf("Proposition 3 : "); lireChaine(p2, 100);
                            printf("Proposition 4 : "); lireChaine(p3, 100);
                            printf("Indice bonne réponse (0-3) : "); bonne = lireEntier();
                            ajouterQuestion(l, texte, p0, p1, p2, p3, bonne);
                        } else if (sq == 2) {
                            int nb = getNombreQuestions(l);
                            printf("Numéro question à supprimer (1..%d) : ", nb);
                            int idx = lireEntier() - 1;
                            if (idx >= 0 && idx < nb) supprimerQuestion(l, idx);
                        } else if (sq == 3) {
                            for (int i = 0; i < getNombreQuestions(l); i++) {
                                Question *q = getQuestion(l, i);
                                printf("%d. %s\n", i+1, q->texte);
                                for (int j = 0; j < 4; j++)
                                    printf("   %d. %s\n", j+1, q->propositions[j]);
                                printf("   (bonne : %d)\n", q->bonne_reponse+1);
                            }
                        }
                    } while (sq != 0);
                }
                break;
            }
        }
    } while (choix != 0);
}

void menuProfesseur(Chapitre **cours) {
    int choix;
    do {
        printf("\n=== MENU PROFESSEUR ===\n");
        printf("1. Afficher tout le cours (récursif)\n");
        printf("2. Ajouter un chapitre\n");
        printf("3. Supprimer un chapitre\n");
        printf("4. Renommer un chapitre\n");
        printf("5. Gérer les leçons d'un chapitre\n");
        printf("6. Exporter JSON (database.json)\n");
        printf("7. Importer JSON\n");
        printf("0. Quitter (retour menu principal)\n");
        choix = lireEntier();
        switch (choix) {
            case 1: afficherCoursRecursif(*cours, 0); break;
            case 2: {
                printf("Titre du chapitre : ");
                char titre[100];
                lireChaine(titre, 100);
                ajouterChapitre(cours, titre);
                break;
            }
            case 3: {
                afficherChapitres(*cours);
                printf("Titre du chapitre à supprimer : ");
                char titre[100];
                lireChaine(titre, 100);
                supprimerChapitre(cours, titre);
                break;
            }
            case 4: {
                afficherChapitres(*cours);
                printf("Titre du chapitre à renommer : ");
                char titre[100];
                lireChaine(titre, 100);
                Chapitre *c = trouverChapitre(*cours, titre);
                if (c) {
                    printf("Nouveau titre : ");
                    lireChaine(titre, 100);
                    renommerChapitre(c, titre);
                }
                break;
            }
            case 5: {
                afficherChapitres(*cours);
                printf("Titre du chapitre : ");
                char titre[100];
                lireChaine(titre, 100);
                Chapitre *c = trouverChapitre(*cours, titre);
                if (c) menuGererLecons(c);
                break;
            }
            case 6: exporterJSON("database.json", *cours); break;
            case 7: {
                Chapitre *nouveau = importerJSON("database.json");
                if (nouveau) {
                    libererCours(*cours);
                    *cours = nouveau;
                    printf("Import réussi.\n");
                } else printf("Erreur d'import.\n");
                break;
            }
        }
    } while (choix != 0);
}

static void prendreQCM(Lecon *l) {
    if (!l || !l->questions) {
        printf("Aucune question dans cette leçon.\n");
        return;
    }
    int nb = getNombreQuestions(l);
    int score = 0;
    for (int i = 0; i < nb; i++) {
        Question *q = getQuestion(l, i);
        printf("\nQ%d : %s\n", i+1, q->texte);
        for (int j = 0; j < 4; j++)
            printf("  %d. %s\n", j+1, q->propositions[j]);
        printf("Votre réponse (1-4) : ");
        int rep = lireEntier() - 1;
        if (evaluerReponse(q, rep)) {
            printf("✓ Correct !\n");
            score++;
        } else {
            printf("✗ Faux. La bonne réponse était : %s\n", q->propositions[q->bonne_reponse]);
        }
    }
    printf("Score : %d / %d\n", score, nb);
}

void menuEtudiant(Chapitre *cours) {
    if (!cours) { printf("Aucun cours chargé. Veuillez d'abord importer depuis le menu professeur.\n"); return; }
    int choix;
    do {
        printf("\n=== MENU ÉTUDIANT ===\n");
        printf("1. Afficher tout le cours\n");
        printf("2. Consulter une leçon (contenu, mots-clés, QCM)\n");
        printf("3. Rechercher un mot-clé\n");
        printf("4. Passer le QCM d'une leçon\n");
        printf("5. Marquer une leçon comme terminee\n");
        printf("6. Voir ma progression\n");
        printf("0. Retour\n");
        choix = lireEntier();
        switch (choix) {
            case 1: afficherCoursRecursif(cours, 0); break;
            case 2: {
                afficherChapitres(cours);
                printf("Chapitre : ");
                char ct[100]; lireChaine(ct, 100);
                Chapitre *ch = trouverChapitre(cours, ct);
                if (ch && ch->lecons) {
                    afficherLecons(ch);
                    printf("Leçon : ");
                    char lt[100]; lireChaine(lt, 100);
                    Lecon *l = ch->lecons;
                    while (l && strcmp(l->titre, lt) != 0) l = l->suivant;
                    if (l) {
                        printf("\n=== %s ===\n", l->titre);
                        printf("Mots-cles : ");
                        MotCle *m = l->mots_cles;
                        if (!m) printf("(aucun)");
                        while (m) { printf(" %s", m->mot); m = m->suivant; }
                        printf("\nQuestions QCM : %d\n", getNombreQuestions(l));
                    }
                }
                break;
            }
            case 3: {
                printf("Mot-clé à rechercher : ");
                char mot[100]; lireChaine(mot, 100);
                if (rechercherMotCle(cours, mot))
                    printf("Mot-cle '%s' trouve dans au moins une lecon.\n", mot);
                else printf("Mot-clé non trouvé.\n");
                break;
            }
            case 4: {
                afficherChapitres(cours);
                printf("Chapitre : ");
                char ct[100]; lireChaine(ct, 100);
                Chapitre *ch = trouverChapitre(cours, ct);
                if (ch && ch->lecons) {
                    afficherLecons(ch);
                    printf("Leçon : ");
                    char lt[100]; lireChaine(lt, 100);
                    Lecon *l = ch->lecons;
                    while (l && strcmp(l->titre, lt) != 0) l = l->suivant;
                    if (l) prendreQCM(l);
                }
                break;
            }
            case 5: {
                afficherChapitres(cours);
                printf("Chapitre : ");
                char ct[100]; lireChaine(ct, 100);
                Chapitre *ch = trouverChapitre(cours, ct);
                if (ch && ch->lecons) {
                    afficherLecons(ch);
                    printf("Leçon : ");
                    char lt[100]; lireChaine(lt, 100);
                    marquerLeconTerminee(cours, ct, lt);
                    printf("Lecon marquee terminee.\n");
                }
                break;
            }
            case 6: afficherProgression(cours); break;
        }
    } while (choix != 0);
}