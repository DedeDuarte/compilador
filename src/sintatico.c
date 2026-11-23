/*
 * |==== Grupo (solo) ====|==========|
 * |         Nome         |    RA    |
 * |----------------------|----------|
 * | Andre Doerner Duarte | 10427938 |
 * |======================|==========|
 */

#include <stdlib.h>

#include "sintatico.h"
#include "identificadores.h"
#include "lexico.h"
#include "semantico.h"
#include "utils.h"

static FILE* fonte = NULL;

void encerrar_analise(int codigo) {
    liberar_tabela(&tabela);
    fclose(fonte);
    exit(codigo);
}

void _erro_sintatico(TInfoAtomo* lookahead, const char* esperado_str) {
    printf(COR_VERMELHO);
    printf(
        "#%d: Erro sintatico:\n"
        "    Esperado [%s]\n"
        "    Recebido [%s]\n"
        "    ",
        lookahead->linha,
        esperado_str,
        atomo_para_str(lookahead->atomo)
    );

    print_atomo(lookahead);
    printf(COR_NORMAL);
    encerrar_analise(2);
}

void _erro_lexico(TInfoAtomo* lookahead) {
    printf(COR_VERMELHO);
    printf(
        "#%d: Erro lexico:\n"
        "    Atomo invalido iniciado por '%c'\n",
        lookahead->linha,
        lookahead->atributo.ch
    );
    printf(COR_NORMAL);
    encerrar_analise(1);
}

void avancar(TInfoAtomo* lookahead, FILE* file) {
    do {
        *lookahead = obter_atomo(file);

        if (lookahead->atomo == ERRO)
            _erro_lexico(lookahead);
    } while (lookahead->atomo == COMENTARIO);
}

void consome(TInfoAtomo* lookahead, TAtomo esperado, FILE* file) {
    if (lookahead->atomo == esperado) {
        avancar(lookahead, file);
    }

    else if (lookahead->atomo == ERRO) {
        _erro_lexico(lookahead);
    }

    else {
        _erro_sintatico(lookahead, atomo_para_str(esperado));
    }
}

void analizar(FILE* file) {
    TInfoAtomo lookahead;

    fonte = file;

    avancar(&lookahead, file);
    identificar_programa(&lookahead, file);

    printf("\n");
    imprimir_tabela(&tabela);
    liberar_tabela(&tabela);
}
