#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/atomos.h"
#include "include/utils.h"

void is_constint(char c, FILE* file, TInfoAtomo* atomo) {
    int value = 0;

    while (c >= '0' && c <= '9') {
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

void is_letter(char c, FILE* file, TInfoAtomo* atomo) {
    char STR_SIZE = 16;
    char str[STR_SIZE];

    do
}

void obter_atomo(FILE* file) {
    TInfoAtomo* atomo = calloc(1, sizeof(TInfoAtomo));
    atomo->linha = 1;

    char c;
    while ((c = fgetc(file)) != EOF) {
        while (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
            if (c == '\n')
                atomo->linha++;

            c = fgetc(file);
        }

        atomo->atomo = ERRO;

        switch (c) {
        case '0' ... '9':
            is_constint(c, file, atomo);
            break;
        
        case 'a' ... 'z':
        case 'A' ... 'Z':
            is_letter(c, file, atomo);
            break;

        default:
            break;
        }

        print_atomo(atomo);
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
