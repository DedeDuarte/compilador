#include <stdlib.h>

#include "lexico.h"
#include "atomos.h"
#include "verificadores.h"
#include "utils.h"

TInfoAtomo obter_atomo(FILE* file) {
    static int linha = 1;

    TInfoAtomo atomo = {0};
    atomo.atomo = ERRO;
    atomo.linha = linha;

    int c = fgetc(file);
    while (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
        if (c == '\n')
            linha++;

        c = fgetc(file);
    }

    atomo.atomo = ERRO;

    atomo.linha = linha;

    if (c == EOF) {
        atomo.atomo = EOS;
        return atomo;
    }

    switch (c) {
        case '+': atomo.atomo = SOMA;              break;
        case '-': atomo.atomo = SUBTRACAO;         break;
        case '*': atomo.atomo = MULTIPLICACAO;     break;
        case '=': atomo.atomo = IGUAL;             break;
        case '(': atomo.atomo = ABRE_PARENTESE;    break;
        case ')': atomo.atomo = FECHA_PARENTESE;   break;
        case ';': atomo.atomo = PONTO_VIRGULA;     break;
        case ',': atomo.atomo = VIRGULA;           break;
        case '.': atomo.atomo = PONTO;             break;

        case '0' ... '9':
            eh_constint(c, file, &atomo);
            break;
        
        case 'a' ... 'z':
        case 'A' ... 'Z':
        case '_':
            eh_alpha(c, file, &atomo);
            break;

        case ':':
            eh_atribuicao(file, &atomo);
            break;

        case '<':
        case '>':
            eh_comparacao(c, file, &atomo);
            break;

        case '{':
            linha += eh_comentario(file, &atomo);
            break;

        case '\'':
            linha += eh_constchar(file, &atomo);
            break;

        default:
            break;
    }

    return atomo;
}