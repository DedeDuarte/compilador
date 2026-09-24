#ifndef VERIFICADORES_H
#define VERIFICADORES_H

#include <stdio.h>

#include "atomos.h"

#define STR_MAX_SIZE 16

void eh_constint(int c, FILE* file, TInfoAtomo* atomo);
void eh_alpha(int c, FILE* file, TInfoAtomo* atomo);
void eh_atribuicao(FILE* file, TInfoAtomo* atomo);
void eh_comparacao(int c, FILE* file, TInfoAtomo* atomo);

#endif