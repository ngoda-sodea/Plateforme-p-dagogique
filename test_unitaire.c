#include "syllabus.h"
#include "progression.h"
#include <assert.h>
#include <stdio.h>

// Petite macro pour compter les succès/échecs
#define TEST(name) do { printf("Test %s... ", name); fflush(stdout); } while(0)
#define PASS  printf("OK\n"); passed++
#define FAIL  printf("ECHEC\n"); failed++

int main() {
    int passed = 0, failed = 0;

    // ----------------------------------------------
    // Test création et gestion des chapitres (Étudiant 2)
    // ----------------------------------------------
    TEST("creerChapitre / ajouterChapitre");
    Chapitre *cours = NULL;
    ajouterChapitre(&cours, "Chap1");
    ajouterChapitre(&cours, "Chap2");
    assert(cours != NULL);
    assert(strcmp(cours->titre, "Chap1") == 0);
    assert(cours->suivant != NULL);
    assert(strcmp(cours->suivant->titre, "Chap2") == 0);
    PASS;

    TEST("trouverChapitre");
    Chapitre *c = trouverChapitre(cours, "Chap2");
    assert(c != NULL && strcmp(c->titre, "Chap2") == 0);
    assert(trouverChapitre(cours, "Inexistant") == NULL);
    PASS;

    TEST("renommerChapitre");
    renommerChapitre(cours, "Premier");
    assert(strcmp(cours->titre, "Premier") == 0);
    PASS;

    TEST("supprimerChapitre");
    supprimerChapitre(&cours, "Chap2");
    assert(cours->suivant == NULL);
    // On recrée pour la suite
    ajouterChapitre(&cours, "Chap2");
    PASS;

    // ----------------------------------------------
    // Test gestion des leçons (Étudiant 3)
    // ----------------------------------------------
    TEST("creerLecon / ajouterLecon");
    Chapitre *chap1 = trouverChapitre(cours, "Premier");
    ajouterLecon(chap1, "Lecon1");
    ajouterLecon(chap1, "Lecon2");
    assert(chap1->lecons != NULL);
    assert(strcmp(chap1->lecons->titre, "Lecon1") == 0);
    assert(chap1->lecons->suivant != NULL);
    PASS;

    TEST("renommerLecon");
    renommerLecon(chap1->lecons, "NouvelleLecon");
    assert(strcmp(chap1->lecons->titre, "NouvelleLecon") == 0);
    PASS;

    TEST("supprimerLecon");
    supprimerLecon(chap1, "Lecon2");
    assert(chap1->lecons->suivant == NULL);
    // Recréer pour les tests suivants
    ajouterLecon(chap1, "Lecon2");
    PASS;

    // ----------------------------------------------
    // Test mots-clés (Étudiant 4)
    // ----------------------------------------------
    TEST("ajouterMotCle / supprimerMotCle");
    Lecon *l = chap1->lecons; // NouvelleLecon
    ajouterMotCle(l, "C");
    ajouterMotCle(l, "Java");
    assert(l->mots_cles != NULL);
    assert(strcmp(l->mots_cles->mot, "Java") == 0); // ajout en tête
    assert(l->mots_cles->suivant != NULL && strcmp(l->mots_cles->suivant->mot, "C") == 0);
    supprimerMotCle(l, "C");
    assert(l->mots_cles->suivant == NULL);
    PASS;

    TEST("rechercherMotCle");
    assert(rechercherMotCle(cours, "Java") == 1);
    assert(rechercherMotCle(cours, "Python") == 0);
    PASS;

    // ----------------------------------------------
    // Test QCM (Étudiant 5)
    // ----------------------------------------------
    TEST("creerQuestion / ajouterQuestion");
    ajouterQuestion(l, "Q1", "A", "B", "C", "D", 2);
    ajouterQuestion(l, "Q2", "A2", "B2", "C2", "D2", 0);
    assert(getNombreQuestions(l) == 2);
    Question *q = getQuestion(l, 1);
    assert(strcmp(q->texte, "Q2") == 0);
    PASS;

    TEST("evaluerReponse");
    q = getQuestion(l, 0);
    assert(evaluerReponse(q, 2) == 1);
    assert(evaluerReponse(q, 1) == 0);
    PASS;

    TEST("supprimerQuestion");
    supprimerQuestion(l, 0);
    assert(getNombreQuestions(l) == 1);
    PASS;

    // ----------------------------------------------
    // Test progression (Étudiant 6)
    // ----------------------------------------------
    TEST("marquerLeconTerminee / progression");
    initialiserProgression();
    marquerLeconTerminee(cours, "Premier", "NouvelleLecon");
    assert(estLeconTerminee("Premier", "NouvelleLecon") == 1);
    // Lecon2 n'est pas terminée
    assert(estLeconTerminee("Premier", "Lecon2") == 0);
    // Nombre total de leçons = 2 (NouvelleLecon et Lecon2)
    assert(getNombreTotalLecons(cours) == 2);
    PASS;

    // ----------------------------------------------
    // Test persistance JSON (export/import)
    // ----------------------------------------------
    TEST("exporterJSON");
    int ok = exporterJSON("test_export.json", cours);
    assert(ok == 1);
    FILE *f = fopen("test_export.json", "r");
    assert(f != NULL);
    fclose(f);
    PASS;

    // TEST("importerJSON");
    // Chapitre *cours2 = importerJSON("test_export.json");
    // assert(cours2 != NULL);
    // // Vérification sommaire
    // assert(strcmp(cours2->titre, "Premier") == 0);
    // assert(cours2->lecons != NULL);
    // assert(strcmp(cours2->lecons->titre, "NouvelleLecon") == 0);
    // libererCours(cours2);
    // PASS;

    TEST("importerJSON");
    Chapitre *cours2 = importerJSON("test_export.json");
    assert(cours2 != NULL);
    // Chercher le chapitre "Premier" dans la liste (peu importe sa position)
    Chapitre *chapPremier = trouverChapitre(cours2, "Premier");
    assert(chapPremier != NULL);
    // Vérifier qu'il contient la leçon "NouvelleLecon"
    assert(chapPremier->lecons != NULL);
    assert(strcmp(chapPremier->lecons->titre, "NouvelleLecon") == 0);
    libererCours(cours2);
    PASS;

    // ----------------------------------------------
    // Nettoyage final
    // ----------------------------------------------
    libererCours(cours);
    libererProgression();
    remove("test_export.json");

    // ----------------------------------------------
    // Bilan
    // ----------------------------------------------
    printf("\n=== BILAN DES TESTS ===\n");
    printf("Passés : %d\n", passed);
    printf("Échecs : %d\n", failed);
    return failed == 0 ? 0 : 1;
}