#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/atomos.h"
#include "include/utils.h"

#define STR_MAX_SIZE 16

void eh_constint(char c, FILE* file, TInfoAtomo* atomo) {
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

void identifica_atomo_str(char* str, TInfoAtomo* atomo) {
    const char* palavras[] = {
        "e", "ou", "se", "div", "fim", "mod", "var", "faca",
        "leia", "entao", "falso", "senao", "funcao", "inicio",
        "logico", "escreva", "inteiro", "enquanto", "algoritmo",
        "caractere", "verdadeiro", "procedimento"
    };

    TAtomo atomos[] = {
        E, OU, SE, DIV, FIM, MOD, VAR, FACA,
        LEIA, ENTAO, FALSO, SENAO, FUNCAO, INICIO,
        LOGICO, ESCREVA, INTEIRO, ENQUANTO, ALGORITMO,
        CARACTERE, VERDADEIRO, PROCEDIMENTO
    };

    int quantidade = sizeof(palavras) / sizeof(palavras[0]);

    for (int i = 0; i < quantidade; i++) {
        if (strcmp(str, palavras[i]) == 0) {
            atomo->atomo = atomos[i];
            return;
        }
    }

    atomo->atomo = IDENTIFICADOR;
    strcpy(atomo->atributo.id, str);
}

void eh_alpha(char c, FILE* file, TInfoAtomo* atomo) {
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
            case '+':
                atomo->atomo = SOMA; break;
            case '-':
                atomo->atomo = SUBTRACAO; break;
            case '*':
                atomo->atomo = MULTIPLICACAO; break;
            case '=':
                atomo->atomo = IGUAL; break;
            case '(':
                atomo->atomo = ABRE_PARENTESE; break;
            case ')':
                atomo->atomo = FECHA_PARENTESE; break;
            case ';':
                atomo->atomo = PONTO_VIRGULA; break;
            case ',':
                atomo->atomo = VIRGULA; break;
            case '.':
                atomo->atomo = PONTO; break;

            case '0' ... '9':
                eh_constint(c, file, atomo);
                break;
            
            case 'a' ... 'z':
            case 'A' ... 'Z':
            case '_':
                eh_alpha(c, file, atomo);
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
