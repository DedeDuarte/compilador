/*
 * |==== Grupo (solo) ====|==========|
 * |         Nome         |    RA    |
 * |----------------------|----------|
 * | Andre Doerner Duarte | 10427938 |
 * |======================|==========|
 */

#ifndef IDENTIFICADORES_H
#define IDENTIFICADORES_H

#include <stdio.h>

#include "atomos.h"

// <fator> ::= identificador [ '(' <lista_expressao> ')' ] | constint | constchar |
//             '(' <expressao> ')' | ( '+' | '-' | nao ) <fator> | verdadeiro | falso
void identificar_fator(TInfoAtomo* lookahead, FILE* file);

// <operador_multiplicacao> ::= '*' | div | e
void identificar_operador_multiplicacao(TInfoAtomo* lookahead, FILE* file);

// <termo> ::= <fator> { <operador_multiplicacao> <fator> }
void identificar_termo(TInfoAtomo* lookahead, FILE* file);

// <operador_adicao> ::= '+' | '-' | mod | ou
void identificar_operador_adicao(TInfoAtomo* lookahead, FILE* file);

// <expressao_simples> ::= <termo> { <operador_adicao> <termo> }
void identificar_expressao_simples(TInfoAtomo* lookahead, FILE* file);

// <operador_relacional> ::= '<>' | '<' | '<=' | '>=' | '>' | '='
void identificar_operador_relacional(TInfoAtomo* lookahead, FILE* file);

// <expressao> ::= <expressao_simples> [ <operador_relacional> <expressao_simples> ]
void identificar_expressao(TInfoAtomo* lookahead, FILE* file);

// <lista_expressao> ::= <expressao> { ',' <expressao> }
void identificar_lista_expressao(TInfoAtomo* lookahead, FILE* file);

// <chamada_procedimento> ::= identificador [ '(' <lista_expressao> ')' ]
// O identificador ja foi consumido em identificar_comando().
void identificar_chamada_procedimento(TInfoAtomo* lookahead, FILE* file);

// <comando_repeticao> ::= enquanto <expressao> faca <comando>
void identificar_comando_repeticao(TInfoAtomo* lookahead, FILE* file);

// <comando_condicional> ::= se <expressao> entao <comando> [ senao <comando> ]
void identificar_comando_condicional(TInfoAtomo* lookahead, FILE* file);

// <comando_saida> ::= escreva '(' <lista_expressao> ')'
void identificar_comando_saida(TInfoAtomo* lookahead, FILE* file);

// <comando_entrada> ::= leia '(' identificador { ',' identificador } ')'
void identificar_comando_entrada(TInfoAtomo* lookahead, FILE* file);

// <comando_atribuicao> ::= identificador ':=' <expressao>
// O identificador ja foi consumido em identificar_comando().
void identificar_comando_atribuicao(TInfoAtomo* lookahead, FILE* file);

// <comando> ::= <comando_atribuicao> | <comando_entrada> | <comando_saida> |
//               <comando_condicional> | <comando_repeticao> |
//               <chamada_procedimento> | <comando_composto>
void identificar_comando(TInfoAtomo* lookahead, FILE* file);

// <comando_composto> ::= inicio <comando> { ';' <comando> } fim
void identificar_comando_composto(TInfoAtomo* lookahead, FILE* file);

// <parametro_formal> ::= [ var ] <lista_variaveis>
void identificar_parametro_formal(TInfoAtomo* lookahead, FILE* file);

// <parametros_formais> ::= '(' <parametro_formal> { ';' <parametro_formal> } ')' |
//                          '(' ')'
void identificar_parametros_formais(TInfoAtomo* lookahead, FILE* file);

// <tipo> ::= caractere | inteiro | logico
void identificar_tipo(TInfoAtomo* lookahead, FILE* file);

// <declaracao_de_procedimento> ::= procedimento identificador <parametros_formais>
//                                 <declaracao_variaveis> <comando_composto>
void identificar_declaracao_procedimento(TInfoAtomo* lookahead, FILE* file);

// <declaracao_de_funcao> ::= funcao <tipo> identificador <parametros_formais>
//                           <declaracao_variaveis> <comando_composto>
void identificar_declaracao_funcao(TInfoAtomo* lookahead, FILE* file);

// <declaracao_de_rotinas> ::= { <declaracao_de_funcao> | <declaracao_de_procedimento> }
void identificar_declaracao_rotinas(TInfoAtomo* lookahead, FILE* file);

// <lista_variaveis> ::= identificador { ',' identificador } ':' <tipo>
void identificar_lista_variaveis(TInfoAtomo* lookahead, FILE* file);

// <declaracao_variaveis> ::= [ var <lista_variaveis> ';' { <lista_variaveis> ';' } ]
void identificar_declaracao_variaveis(TInfoAtomo* lookahead, FILE* file);

// <bloco> ::= <declaracao_variaveis> <declaracao_de_rotinas> <comando_composto>
void identificar_bloco(TInfoAtomo* lookahead, FILE* file);

// <programa> ::= algoritmo identificador ';' <bloco> '.'
void identificar_programa(TInfoAtomo* lookahead, FILE* file);

#endif