#include <stdlib.h>

#include "sintatico.h"
#include "identificadores.h"
#include "lexico.h"
#include "utils.h"

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
    exit(2);
}

void _erro_lexico(TInfoAtomo* lookahead) {
    printf(COR_VERMELHO);
    printf("Erro lexico em:\n    ");
    print_atomo(lookahead);
    printf(COR_NORMAL);
    exit(1);
}

void avancar(TInfoAtomo* lookahead, FILE* file) {
    do {
        *lookahead = obter_atomo(file);

        if (lookahead->atomo == ERRO)
            _erro_lexico(lookahead);

        if (lookahead->atomo == COMENTARIO)
            print_atomo(lookahead);
    } while (lookahead->atomo == COMENTARIO);
}

void consome(TInfoAtomo* lookahead, TAtomo esperado, FILE* file) {
    if (lookahead->atomo == esperado) {
        print_atomo(lookahead);
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

    avancar(&lookahead, file);
    identificar_programa(&lookahead, file);

    printf("%d linhas analisadas, programa sintaticamente correto\n", lookahead.linha);
}
