/*
 * |==== Grupo (solo) ====|==========|
 * |         Nome         |    RA    |
 * |----------------------|----------|
 * | Andre Doerner Duarte | 10427938 |
 * |======================|==========|
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tabela_simbolos.h"

int hashMack(char* s);

void iniciar_tabela(TTabelaSimbolos* tabela) {
    for (int i = 0; i < PRIME_NUMER; i++)
        tabela->entradas[i] = NULL;
}

TNo* buscar_simbolo(TTabelaSimbolos* tabela, char* id, int escopo) {
    TNo* no = tabela->entradas[hashMack(id)];

    while (no != NULL) {
        if (strcmp(no->ID, id) == 0 && no->escopo == escopo)
            return no;
        no = no->prox;
    }

    return NULL;
}

int inserir_simbolo(TTabelaSimbolos* tabela, char* id, TCategoria categoria, int escopo, int endereco) {
    if (buscar_simbolo(tabela, id, escopo) != NULL)
        return 1;

    TNo* novo = malloc(sizeof(TNo));
    if (novo == NULL) {
        fprintf(stderr, "Erro: sem memoria para a tabela de simbolos\n");
        exit(1);
    }

    strncpy(novo->ID, id, 15);
    novo->ID[15] = '\0';
    novo->categoria = categoria;
    novo->escopo = escopo;
    novo->endereco = endereco;

    // Insere no inicio da lista da entrada
    int entrada = hashMack(id);
    novo->prox = tabela->entradas[entrada];
    tabela->entradas[entrada] = novo;

    return 0;
}

void liberar_tabela(TTabelaSimbolos* tabela) {
    for (int i = 0; i < PRIME_NUMER; i++) {
        TNo* no = tabela->entradas[i];

        while (no != NULL) {
            TNo* proximo = no->prox;
            free(no);
            no = proximo;
        }

        tabela->entradas[i] = NULL;
    }
}

void remover_locais(TTabelaSimbolos* tabela) {
    for (int i = 0; i < PRIME_NUMER; i++) {
        TNo* anterior = NULL;
        TNo* atual = tabela->entradas[i];

        while (atual != NULL) {
            TNo* proximo = atual->prox;

            if (atual->escopo == ESC_LOCAL) {
                if (anterior == NULL)
                    tabela->entradas[i] = proximo;
                else
                    anterior->prox = proximo;
                free(atual);
            }
            else {
                anterior = atual;
            }

            atual = proximo;
        }
    }
}
