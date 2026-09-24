/*
 * |==== Grupo (solo) ====|==========|
 * |         Nome         |    RA    |
 * |----------------------|----------|
 * | Andre Doerner Duarte | 10427938 |
 * |======================|==========|
 */

#include <stdio.h>
#include "utils.h"

const char* atomo_para_str(TAtomo atomo) {
    switch (atomo) {
        case ERRO:              return "erro";
        case ALGORITMO:         return "algoritmo";
        case CARACTERE:         return "caractere";
        case DIV:               return "div";
        case E:                 return "e";
        case ENQUANTO:          return "enquanto";
        case ENTAO:             return "entao";
        case ESCREVA:           return "escreva";
        case FACA:              return "faca";
        case FALSO:             return "falso";
        case FIM:               return "fim";
        case FUNCAO:            return "funcao";
        case INICIO:            return "inicio";
        case INTEIRO:           return "inteiro";
        case LEIA:              return "leia";
        case LOGICO:            return "logico";
        case MOD:               return "mod";
        case NAO:               return "nao";
        case OU:                return "ou";
        case PROCEDIMENTO:      return "procedimento";
        case SE:                return "se";
        case SENAO:             return "senao";
        case VAR:               return "var";
        case VERDADEIRO:        return "verdadeiro";
        case IDENTIFICADOR:     return "identificador";
        case CONSTINT:          return "constint";
        case CONSTCHAR:         return "constchar";
        case ATRIBUICAO:        return "atribuicao";
        case SOMA:              return "soma";
        case SUBTRACAO:         return "subtracao";
        case MULTIPLICACAO:     return "multiplicacao";
        case MENOR:             return "menor";
        case MENOR_IGUAL:       return "menor_igual";
        case MAIOR:             return "maior";
        case MAIOR_IGUAL:       return "maior_igual";
        case DIFERENTE:         return "diferente";
        case IGUAL:             return "igual";
        case PONTO_VIRGULA:     return "ponto_virgula";
        case VIRGULA:           return "virgula";
        case PONTO:             return "ponto";
        case DOIS_PONTOS:       return "dois_pontos";
        case ABRE_PARENTESE:    return "abre_par";
        case FECHA_PARENTESE:   return "fecha_par";
        case COMENTARIO:        return "comentario";
        case EOS:               return "eos";
    }

    return "erro";
}

void print_atomo(TInfoAtomo* atomo) {
    const char* nome = atomo_para_str(atomo->atomo);

    if (atomo->atomo == ERRO)
        printf("#%d: " COR_VERMELHO "%s" COR_NORMAL, atomo->linha, nome);
    else
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
