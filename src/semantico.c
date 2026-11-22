/*
 * |==== Grupo (solo) ====|==========|
 * |         Nome         |    RA    |
 * |----------------------|----------|
 * | Andre Doerner Duarte | 10427938 |
 * |======================|==========|
 */

#include <stdio.h>
#include <stdlib.h>

#include "semantico.h"
#include "utils.h"

TTabelaSimbolos tabela;
int escopo_atual = ESC_GLOBAL;

static int proximo_endereco = 0;

#define MAX_PARAMETROS 32
static TNo* parametros[MAX_PARAMETROS];
static int num_parametros = 0;

void _erro_semantico(int linha, const char* id, const char* mensagem) {
    printf(COR_VERMELHO);
    printf(
        "#%d: Erro semantico:\n"
        "    identificador '%s' %s\n",
        linha,
        id,
        mensagem
    );
    printf(COR_NORMAL);
    liberar_tabela(&tabela);
    exit(3);
}

void declarar_identificador(TInfoAtomo* lookahead, TCategoria categoria) {
    int endereco = 0;

    if (categoria == CAT_VARIAVEL)
        endereco = proximo_endereco;

    if (inserir_simbolo(&tabela, lookahead->atributo.id, categoria, escopo_atual, endereco))
        _erro_semantico(lookahead->linha, lookahead->atributo.id, "ja declarado neste escopo");

    if (categoria == CAT_VARIAVEL)
        proximo_endereco++;

    if (categoria == CAT_PARAMETRO) {
        if (num_parametros == MAX_PARAMETROS)
            _erro_semantico(lookahead->linha, lookahead->atributo.id, "excede o maximo de parametros");

        parametros[num_parametros] = buscar_simbolo(&tabela, lookahead->atributo.id, escopo_atual);
        num_parametros++;
    }
}

void calcular_enderecos_parametros(void) {
    for (int i = 1; i <= num_parametros; i++)
        parametros[i - 1]->endereco = -(num_parametros + 3 - i);

    num_parametros = 0;
}

void entrar_rotina(void) {
    escopo_atual = ESC_LOCAL;
    proximo_endereco = 0;
}

void sair_rotina(void) {
    escopo_atual = ESC_GLOBAL;
    remover_locais(&tabela);
}
