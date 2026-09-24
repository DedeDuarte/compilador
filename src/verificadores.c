#include <ctype.h>
#include <limits.h>
#include <string.h>

#include "../include/verificadores.h"

#define STR_MAX_SIZE 16
#define TRUE 1
#define FALSE 0

void eh_constint(int c, FILE* file, TInfoAtomo* atomo) {
    int value = 0;
    int excedeu = 0;

    while (isdigit(c)) {
        // Verifica se o número ainda cabe em int
        if (value > (INT_MAX - (c - '0')) / 10)
            excedeu = 1;
        else if (!excedeu)
            value = value * 10 + (c - '0'); // pega o valor real do número
        c = fgetc(file);
    }

    if (excedeu)
        return;

    // Caso o número esteja em notação exponencial, como 12E+2
    if (c == 'E') {
        int expoente = 0;
        c = fgetc(file);

        if (c == '+')
            c = fgetc(file);

        // Depois do E ou E+, precisa ter pelo menos um dígito
        if (!isdigit(c))
            return;

        while (isdigit(c)) {
            // Evita estouro ao ler um expoente muito grande
            if (expoente < 10) {
                expoente = expoente * 10 + (c - '0');
                if (expoente > 10)
                    expoente = 10;
            }
            c = fgetc(file);
        }

        // Multiplica por 10 para cada unidade do expoente
        while (expoente-- > 0) {
            if (value > INT_MAX / 10)
                return;

            value *= 10;
        }
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

    if (c == '=')
        atomo->atomo = ATRIBUICAO;

    else {
        atomo->atomo = DOIS_PONTOS;

        if (c != EOF)
            ungetc(c, file);
    }
}

void eh_comparacao(int c, FILE* file, TInfoAtomo* atomo) {
    if (c == '<') {
        c = fgetc(file);

        if (c == '=') // Caso "<="
            atomo->atomo = MENOR_IGUAL;

        else if (c == '>') // Caso "<>"
            atomo->atomo = DIFERENTE;

        else { // Caso "<"
            atomo->atomo = MENOR;

            if (c != EOF)
                ungetc(c, file);
        }
    }

    else {
        c = fgetc(file);

        if (c == '=')   // Caso ">="
            atomo->atomo = MAIOR_IGUAL;

        else {          // Caso ">"
            atomo->atomo = MAIOR;

            if (c != EOF)
                ungetc(c, file);
        }
    }
}

int eh_comentario(FILE* file, TInfoAtomo* atomo) {
    int c = fgetc(file);

    int linhas_puladas = 0;

    if (c != '-') {             // Caso não "{-"
        if (c != EOF)
            ungetc(c, file);

        return linhas_puladas;
    }

    while (c != EOF) {
        c = fgetc(file);

        if (c == '\n')          // Caso "\n"
            linhas_puladas++;

        if (c == '-') {         // Caso "-..."
            c = fgetc(file);

            if (c == '}') {     // Caso "-}"
                atomo->atomo = COMENTARIO;
                break;
            }

            else if (c != EOF) // Caso não "-}"
                ungetc(c, file);
        }
    }

    return linhas_puladas;
}

int eh_constchar(FILE* file, TInfoAtomo* atomo) {
    int c = fgetc(file);
    if (c == EOF)
        return 0;

    atomo->atributo.ch = c;

    int linhas_puladas = (c == '\n');

    c = fgetc(file);
    if (c != '\'') {
        if (c != EOF)
            ungetc(c, file);

        return linhas_puladas;
    }

    atomo->atomo = CONSTCHAR;
    return linhas_puladas;
}
