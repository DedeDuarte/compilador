#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/atomos.h"
#include "include/utils.h"
#include "include/verificadores.h"

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

            default:
                break;
        }

        print_atomo(atomo);
        atomo->linha += linhas_puladas;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr,
            "ERRO\n"
            "Uso correto: ./%s <arquivo.fonte>\n",
            argv[1]
        );
        exit(1);
    }
    
    // Abre arquivo
    FILE* file = fopen(argv[1], "r");

    // Verifica se `file` foi aberto corretamente
    if (file == NULL) {
        fprintf(stderr, "Arquivo \"%s\" não foi aberto corretamente\nVerifique o caminho\n", argv[1]);
        exit(2);
    }

    obter_atomo(file);

    return 0;
}
