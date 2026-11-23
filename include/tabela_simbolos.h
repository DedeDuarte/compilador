/*
 * |==== Grupo (solo) ====|==========|
 * |         Nome         |    RA    |
 * |----------------------|----------|
 * | Andre Doerner Duarte | 10427938 |
 * |======================|==========|
 */

#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#define PRIME_NUMER 211 // numero primo para reduzir colisoes
#define MAX_PARAMETROS 32

typedef enum {
    CAT_VARIAVEL,
    CAT_PARAMETRO,
    CAT_FUNCAO,
    CAT_PROCEDIMENTO
} TCategoria;

#define ESC_GLOBAL 0
#define ESC_LOCAL  1

typedef struct _TNo {
    char ID[16];
    int endereco;
    TCategoria categoria;
    int escopo;
    int qtd_parametros;     // so para funcao e procedimento
    int ref_parametros[MAX_PARAMETROS];
    int referencia;
    int rotulo;
    int ativo;
    struct _TNo *prox;
} TNo;

typedef struct {
    TNo *entradas[PRIME_NUMER];
} TTabelaSimbolos;

void iniciar_tabela(TTabelaSimbolos* tabela);

int inserir_simbolo(TTabelaSimbolos* tabela, char* id, TCategoria categoria, int escopo, int endereco);

TNo* buscar_simbolo(TTabelaSimbolos* tabela, char* id, int escopo);

void liberar_tabela(TTabelaSimbolos* tabela);

void desativar_locais(TTabelaSimbolos* tabela);

void imprimir_tabela(TTabelaSimbolos* tabela);

#endif
