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

// Rotina cujo corpo esta sendo analisado (NULL no programa principal)
static TNo* rotina_atual = NULL;

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

TNo* declarar_identificador(TInfoAtomo* lookahead, TCategoria categoria) {
    int endereco = 0;

    if (categoria == CAT_VARIAVEL)
        endereco = proximo_endereco;

    if (inserir_simbolo(&tabela, lookahead->atributo.id, categoria, escopo_atual, endereco))
        _erro_semantico(lookahead->linha, lookahead->atributo.id, "ja declarado neste escopo");

    if (categoria == CAT_VARIAVEL)
        proximo_endereco++;

    TNo* no = buscar_simbolo(&tabela, lookahead->atributo.id, escopo_atual);

    if (categoria == CAT_PARAMETRO) {
        if (num_parametros == MAX_PARAMETROS)
            _erro_semantico(lookahead->linha, lookahead->atributo.id, "excede o maximo de parametros");

        parametros[num_parametros] = no;
        num_parametros++;
    }

    return no;
}

void calcular_enderecos_parametros(void) {
    for (int i = 1; i <= num_parametros; i++)
        parametros[i - 1]->endereco = -(num_parametros + 3 - i);

    rotina_atual->qtd_parametros = num_parametros;
    num_parametros = 0;
}

void entrar_rotina(TNo* rotina) {
    rotina_atual = rotina;
    escopo_atual = ESC_LOCAL;
    proximo_endereco = 0;
}

void sair_rotina(void) {
    rotina_atual = NULL;
    escopo_atual = ESC_GLOBAL;
    desativar_locais(&tabela);
}

TNo* buscar_identificador(TInfoAtomo* lookahead) {
    TNo* no = NULL;

    if (escopo_atual == ESC_LOCAL)
        no = buscar_simbolo(&tabela, lookahead->atributo.id, ESC_LOCAL);

    if (no == NULL)
        no = buscar_simbolo(&tabela, lookahead->atributo.id, ESC_GLOBAL);

    if (no == NULL)
        _erro_semantico(lookahead->linha, lookahead->atributo.id, "nao foi declarado");

    return no;
}

void verificar_valor(TNo* no, int linha) {
    if (no->categoria == CAT_PROCEDIMENTO)
        _erro_semantico(linha, no->ID, "e um procedimento e nao tem valor");
}

void verificar_chamada_funcao(TNo* no, int linha) {
    if (no->categoria != CAT_FUNCAO)
        _erro_semantico(linha, no->ID, "nao e uma funcao");
}

void verificar_chamada_procedimento(TNo* no, int linha) {
    if (no->categoria != CAT_PROCEDIMENTO)
        _erro_semantico(linha, no->ID, "nao e um procedimento");
}

void verificar_atribuicao(TNo* no, int linha) {
    if (no->categoria == CAT_VARIAVEL || no->categoria == CAT_PARAMETRO)
        return;

    // O nome da funcao recebe o valor de retorno, so dentro dela
    if (no->categoria == CAT_FUNCAO && no == rotina_atual)
        return;

    _erro_semantico(linha, no->ID, "nao pode receber atribuicao");
}

void verificar_leitura(TNo* no, int linha) {
    if (no->categoria != CAT_VARIAVEL && no->categoria != CAT_PARAMETRO)
        _erro_semantico(linha, no->ID, "nao e uma variavel");
}

void verificar_argumentos(TNo* no, int qtd, int linha) {
    if (qtd != no->qtd_parametros)
        _erro_semantico(linha, no->ID, "recebeu numero errado de argumentos");
}

int total_variaveis(void) {
    return proximo_endereco;
}
