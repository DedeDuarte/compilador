#include <stdlib.h>

#include "lexico.h"
#include "atomos.h"
#include "verificadores.h"
#include "utils.h"

void obter_atomo(FILE* file) {
    TInfoAtomo* atomo = calloc(1, sizeof(TInfoAtomo));
    atomo->linha = 1;

    int c;
    while ((c = fgetc(file)) != EOF) {
        while (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
            if (c == '\n')
                atomo->linha++;

            c = fgetc(file);
        }

        atomo->atomo = ERRO;

        int linhas_puladas = 0;
        switch (c) {
            case '+': atomo->atomo = SOMA;              break;
            case '-': atomo->atomo = SUBTRACAO;         break;
            case '*': atomo->atomo = MULTIPLICACAO;     break;
            case '=': atomo->atomo = IGUAL;             break;
            case '(': atomo->atomo = ABRE_PARENTESE;    break;
            case ')': atomo->atomo = FECHA_PARENTESE;   break;
            case ';': atomo->atomo = PONTO_VIRGULA;     break;
            case ',': atomo->atomo = VIRGULA;           break;
            case '.': atomo->atomo = PONTO;             break;

            case '0' ... '9':
                eh_constint(c, file, atomo);
                break;
            
            case 'a' ... 'z':
            case 'A' ... 'Z':
            case '_':
                eh_alpha(c, file, atomo);
                break;

            case ':':
                eh_atribuicao(file, atomo);
                break;

            case '<':
            case '>':
                eh_comparacao(c, file, atomo);
                break;

            case '{':
                linhas_puladas = eh_comentario(file, atomo);
                break;

            case '\'':
                linhas_puladas = eh_constchar(file, atomo);
                break;

            default:
                break;
        }

        print_atomo(atomo);
        atomo->linha += linhas_puladas;
    }
}