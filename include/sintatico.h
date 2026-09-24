#ifndef SINTATICO_H
#define SINTATICO_H

#include <stdio.h>

#include "atomos.h"

void _erro_sintatico(TInfoAtomo* lookahead, const char* esperado_str);
void _erro_lexico(TInfoAtomo* lookahead);
void avancar(TInfoAtomo* lookahead, FILE* file);
void consome(TInfoAtomo* atual, TAtomo esperado, FILE* file);
void analizar(FILE* file);

#endif
