#include <ctype.h>
// #include <stdlib.h>
#include <string.h>

#include "../include/verificadores.h"

#define STR_MAX_SIZE 16

void eh_constint(int c, FILE* file, TInfoAtomo* atomo) {
    int value = 0;

    while (isdigit(c)) {
        value = value * 10 + (c - '0'); // pega o valor real do número
        c = fgetc(file);
    }

    if (isalpha(c) || c == '_')
        return;

    if (c != EOF)
        ungetc(c, file);
    

    atomo->atomo = CONSTINT;
    atomo->atributo.numero = value;
}

void eh_alpha(int c, FILE* file, TInfoAtomo* atomo) {
    char str[STR_MAX_SIZE];

    char excedeu = 0;

    int i = 0;
    while (isalpha(c) || isdigit(c) || c == '_') {
        if (i < STR_MAX_SIZE-1)
            str[i++] = tolower(c);
        else
            excedeu = 1;

        c = fgetc(file);
    }
    
    str[i] = '\0';
    
    if (c != EOF)
        ungetc(c, file);

    if (excedeu)
        return;
    
    identifica_atomo_str(str, atomo);
}

void eh_atribuicao(FILE* file, TInfoAtomo* atomo) {
    int c = fgetc(file);

    if (c == '=') {
        atomo->atomo = ATRIBUICAO;
    }
    else {
        atomo->atomo = DOIS_PONTOS;
        if (c != EOF)
            ungetc(c, file);
    }
}