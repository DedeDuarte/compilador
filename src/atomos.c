#include "string.h"

#include "atomos.h"

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