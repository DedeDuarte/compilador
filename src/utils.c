#include <stdio.h>
#include "utils.h"

void print_atomo(TInfoAtomo* atomo) {
    const char* nome = "erro";

    switch (atomo->atomo) {
        case ERRO:              nome = "erro"; break;
        case ALGORITMO:         nome = "algoritmo"; break;
        case CARACTERE:         nome = "caractere"; break;
        case DIV:               nome = "div"; break;
        case E:                 nome = "e"; break;
        case ENQUANTO:          nome = "enquanto"; break;
        case ENTAO:             nome = "entao"; break;
        case ESCREVA:           nome = "escreva"; break;
        case FACA:              nome = "faca"; break;
        case FALSO:             nome = "falso"; break;
        case FIM:               nome = "fim"; break;
        case FUNCAO:            nome = "funcao"; break;
        case INICIO:            nome = "inicio"; break;
        case INTEIRO:           nome = "inteiro"; break;
        case LEIA:              nome = "leia"; break;
        case LOGICO:            nome = "logico"; break;
        case MOD:               nome = "mod"; break;
        case OU:                nome = "ou"; break;
        case PROCEDIMENTO:      nome = "procedimento"; break;
        case SE:                nome = "se"; break;
        case SENAO:             nome = "senao"; break;
        case VAR:               nome = "var"; break;
        case VERDADEIRO:        nome = "verdadeiro"; break;
        case IDENTIFICADOR:     nome = "identificador"; break;
        case CONSTINT:          nome = "constint"; break;
        case CONSTCHAR:         nome = "constchar"; break;
        case ATRIBUICAO:        nome = "atribuicao"; break;
        case SOMA:              nome = "soma"; break;
        case SUBTRACAO:         nome = "subtracao"; break;
        case MULTIPLICACAO:     nome = "multiplicacao"; break;
        case MENOR:             nome = "menor"; break;
        case MENOR_IGUAL:       nome = "menor_igual"; break;
        case MAIOR:             nome = "maior"; break;
        case MAIOR_IGUAL:       nome = "maior_igual"; break;
        case DIFERENTE:         nome = "diferente"; break;
        case IGUAL:             nome = "igual"; break;
        case PONTO_VIRGULA:     nome = "ponto_virgula"; break;
        case VIRGULA:           nome = "virgula"; break;
        case PONTO:             nome = "ponto"; break;
        case DOIS_PONTOS:       nome = "dois_pontos"; break;
        case ABRE_PARENTESE:    nome = "abre_par"; break;
        case FECHA_PARENTESE:   nome = "fecha_par"; break;
        case COMENTARIO:        nome = "comentario"; break;
        case EOS:               nome = "eos"; break;
    }

    printf("#%d: %s", atomo->linha, nome);

    switch (atomo->atomo) {
        case IDENTIFICADOR:
            printf(" -> %s", atomo->atributo.id);
            break;
        case CONSTINT:
            printf(" -> %d", atomo->atributo.numero);
            break;
        case CONSTCHAR:
            printf(" -> '%c'", atomo->atributo.ch);
            break;
        default:
            break;
    }

    putchar('\n');
}
