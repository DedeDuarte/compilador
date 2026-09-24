#ifndef SINTATICO_H
#define SINTATICO_H

#include <stdio.h>

#include "atomos.h"

void consome(TInfoAtomo* atual, TAtomo esperado, FILE* file);
void analizar(FILE* file);

#endif