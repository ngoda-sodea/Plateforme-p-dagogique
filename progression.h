// progression.h
#ifndef PROGRESSION_H
#define PROGRESSION_H

#include "syllabus.h"

void initialiserProgression();
void libererProgression();
void marquerLeconTerminee(Chapitre *cours, const char *chapTitre, const char *leconTitre);
int estLeconTerminee(const char *chapTitre, const char *leconTitre);
void afficherProgression(Chapitre *cours);
int getNombreTotalLecons(Chapitre *cours);

#endif