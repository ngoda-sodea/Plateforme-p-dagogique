#include "persistence.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ---------- EXPORT (inchangé) ---------- */
static void echapperJSON(FILE *f, const char *str) {
    fputc('"', f);
    while (*str) {
        if (*str == '"' || *str == '\\') fputc('\\', f);
        fputc(*str, f);
        str++;
    }
    fputc('"', f);
}

static void exporterMotsCles(FILE *f, MotCle *liste) {
    fputs("    \"mots_cles\": [", f);
    int first = 1;
    while (liste) {
        if (!first) fputs(", ", f);
        echapperJSON(f, liste->mot);
        first = 0;
        liste = liste->suivant;
    }
    fputs("],\n", f);
}

static void exporterQuestions(FILE *f, Question *q) {
    fputs("    \"questions\": [\n", f);
    int firstQ = 1;
    while (q) {
        if (!firstQ) fputs(",\n", f);
        fputs("      {\n", f);
        fprintf(f, "        \"texte\": ");
        echapperJSON(f, q->texte);
        fputs(",\n", f);
        fputs("        \"propositions\": [", f);
        for (int i = 0; i < 4; i++) {
            if (i > 0) fputs(", ", f);
            echapperJSON(f, q->propositions[i]);
        }
        fprintf(f, "],\n        \"bonne_reponse\": %d\n", q->bonne_reponse);
        fputs("      }", f);
        firstQ = 0;
        q = q->suivant;
    }
    fputs("\n    ]\n", f);
}

static void exporterLecons(FILE *f, Lecon *l) {
    fputs("    \"lecons\": [\n", f);
    int firstL = 1;
    while (l) {
        if (!firstL) fputs(",\n", f);
        fputs("      {\n", f);
        fprintf(f, "        \"titre\": ");
        echapperJSON(f, l->titre);
        fputs(",\n", f);
        exporterMotsCles(f, l->mots_cles);
        exporterQuestions(f, l->questions);
        fputs("      }", f);
        firstL = 0;
        l = l->suivant;
    }
    fputs("\n    ]\n", f);
}

int exporterJSON(const char *chemin, Chapitre *cours) {
    FILE *f = fopen(chemin, "w");
    if (!f) return 0;
    fputs("{\n  \"chapitres\": [\n", f);
    int firstC = 1;
    Chapitre *c = cours;
    while (c) {
        if (!firstC) fputs(",\n", f);
        fputs("    {\n", f);
        fprintf(f, "      \"titre\": ");
        echapperJSON(f, c->titre);
        fputs(",\n", f);
        exporterLecons(f, c->lecons);
        fputs("    }", f);
        firstC = 0;
        c = c->suivant;
    }
    fputs("\n  ]\n}\n", f);
    fclose(f);
    return 1;
}

/* ---------- IMPORT avec lecture ligne par ligne portable ---------- */
static char* lireLigneFichier(FILE *f) {
    char *str = NULL;
    size_t size = 0;
    char buffer[256];

    do {
        if (!fgets(buffer, sizeof(buffer), f)) {
            if (str) free(str);
            return NULL;
        }
        size_t buffer_len = strlen(buffer);
        char *new_str = realloc(str, size + buffer_len + 1);
        if (!new_str) {
            free(str);
            return NULL;
        }
        str = new_str;
        memcpy(str + size, buffer, buffer_len + 1);
        size += buffer_len;
        if (buffer_len > 0 && buffer[buffer_len-1] == '\n') {
            str[size-1] = '\0';
            break;
        }
    } while (1);
    return str;
}

static char* extraireChaineEntreGuillemets(const char *ligne, const char *debut) {
    const char *p = strstr(ligne, debut);
    if (!p) return NULL;
    p = strchr(p + strlen(debut), '"');
    if (!p) return NULL;
    p++;
    const char *q = strchr(p, '"');
    if (!q) return NULL;
    size_t len = q - p;
    char *res = malloc(len + 1);
    strncpy(res, p, len);
    res[len] = '\0';
    return res;
}

static int extraireEntier(const char *ligne, const char *debut) {
    const char *p = strstr(ligne, debut);
    if (!p) return -1;
    p = strchr(p + strlen(debut), ':');
    if (!p) return -1;
    p++;
    while (*p && isspace((unsigned char)*p)) p++;
    return atoi(p);
}

static void lireTableauMotsCles(FILE *f, MotCle **liste) {
    char *line;
    while ((line = lireLigneFichier(f))) {
        if (strchr(line, ']')) {
            free(line);
            break;
        }
        char *mot = extraireChaineEntreGuillemets(line, "");
        if (mot) {
            MotCle *m = malloc(sizeof(MotCle));
            m->mot = mot;
            m->suivant = *liste;
            *liste = m;
        }
        free(line);
    }
}

static void lireQuestions(FILE *f, Question **questions) {
    char *line;
    int dansQuestion = 0;
    Question *q = NULL;
    while ((line = lireLigneFichier(f))) {
        if (strstr(line, "\"texte\":")) {
            q = malloc(sizeof(Question));
            memset(q, 0, sizeof(Question));
            q->texte = extraireChaineEntreGuillemets(line, "\"texte\":");
            dansQuestion = 1;
        } else if (dansQuestion && strstr(line, "\"propositions\":")) {
            for (int i = 0; i < 4; i++) {
                char *propLine = lireLigneFichier(f);
                if (propLine) {
                    q->propositions[i] = extraireChaineEntreGuillemets(propLine, "");
                    free(propLine);
                }
            }
        } else if (dansQuestion && strstr(line, "\"bonne_reponse\":")) {
            q->bonne_reponse = extraireEntier(line, "\"bonne_reponse\"");
            q->suivant = *questions;
            *questions = q;
            dansQuestion = 0;
            q = NULL;
        } else if (strstr(line, "]")) {
            free(line);
            break;
        }
        free(line);
    }
}

static void lireLecons(FILE *f, Lecon **lecons) {
    char *line;
    Lecon *lecon = NULL;
    while ((line = lireLigneFichier(f))) {
        if (strstr(line, "\"titre\":")) {
            lecon = malloc(sizeof(Lecon));
            memset(lecon, 0, sizeof(Lecon));
            lecon->titre = extraireChaineEntreGuillemets(line, "\"titre\":");
        } else if (lecon && strstr(line, "\"mots_cles\":")) {
            lireTableauMotsCles(f, &lecon->mots_cles);
        } else if (lecon && strstr(line, "\"questions\":")) {
            lireQuestions(f, &lecon->questions);
        } else if (strstr(line, "}")) {
            if (lecon) {
                lecon->suivant = *lecons;
                *lecons = lecon;
                lecon = NULL;
            }
        } else if (strstr(line, "]")) {
            free(line);
            break;
        }
        free(line);
    }
}

Chapitre* importerJSON(const char *chemin) {
    FILE *f = fopen(chemin, "r");
    if (!f) return NULL;
    Chapitre *cours = NULL;
    char *line;
    Chapitre *chapitre = NULL;
    while ((line = lireLigneFichier(f))) {
        if (strstr(line, "\"titre\":")) {
            chapitre = malloc(sizeof(Chapitre));
            memset(chapitre, 0, sizeof(Chapitre));
            chapitre->titre = extraireChaineEntreGuillemets(line, "\"titre\":");
        } else if (chapitre && strstr(line, "\"lecons\":")) {
            lireLecons(f, &chapitre->lecons);
        } else if (strstr(line, "}")) {
            if (chapitre) {
                chapitre->suivant = cours;
                cours = chapitre;
                chapitre = NULL;
            }
        }
        free(line);
    }
    fclose(f);
    return cours;
}