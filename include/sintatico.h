#ifndef SINTATICO_H
#define SINTATICO_H

#include <stdio.h>

#include "atomos.h"

void avancar(TInfoAtomo* lookahead, FILE* file);
void consome(TInfoAtomo* atual, TAtomo esperado, FILE* file);
void analizar(FILE* file);

#endif