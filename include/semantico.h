/*
 * |==== Grupo (solo) ====|==========|
 * |         Nome         |    RA    |
 * |----------------------|----------|
 * | Andre Doerner Duarte | 10427938 |
 * |======================|==========|
 */

#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "atomos.h"
#include "tabela_simbolos.h"

extern TTabelaSimbolos tabela;
extern int escopo_atual;

void _erro_semantico(int linha, const char* id, const char* mensagem);

TNo* declarar_identificador(TInfoAtomo* lookahead, TCategoria categoria);

void calcular_enderecos_parametros(void);
void definir_referencia(int referencia);

void entrar_rotina(TNo* rotina);
void sair_rotina(void);

int total_variaveis(void);

TNo* buscar_identificador(TInfoAtomo* lookahead);

void verificar_valor(TNo* no, int linha);
void verificar_chamada_funcao(TNo* no, int linha);
void verificar_chamada_procedimento(TNo* no, int linha);
void verificar_atribuicao(TNo* no, int linha);
void verificar_leitura(TNo* no, int linha);
void verificar_argumentos(TNo* no, int qtd, int linha);

#endif
