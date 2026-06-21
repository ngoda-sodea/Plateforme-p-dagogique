// persistence.h
#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "syllabus.h"

int exporterJSON(const char *chemin, Chapitre *cours);
Chapitre* importerJSON(const char *chemin);

#endif