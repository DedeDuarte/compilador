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

void declarar_identificador(TInfoAtomo* lookahead, TCategoria categoria);

void calcular_enderecos_parametros(void);

void entrar_rotina(void);
void sair_rotina(void);

#endif
