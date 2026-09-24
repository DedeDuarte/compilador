#include <stdlib.h>

#include "sintatico.h"
#include "lexico.h"
#include "utils.h"

void identificar_lista_expressao(TInfoAtomo* lookahead, FILE* file);
void identificar_expressao(TInfoAtomo* lookahead, FILE* file);
void identificar_comando(TInfoAtomo* lookahead, FILE* file);
void identificar_comando_composto(TInfoAtomo* lookahead, FILE* file);
void identificar_lista_variaveis(TInfoAtomo* lookahead, FILE* file);
void identificar_declaracao_variaveis(TInfoAtomo* lookahead, FILE* file);

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

void identificar_fator(TInfoAtomo* lookahead, FILE* file) {
    switch (lookahead->atomo) {
        case IDENTIFICADOR:
            consome(lookahead, IDENTIFICADOR, file);
            if (lookahead->atomo == ABRE_PARENTESE) {
                consome(lookahead, ABRE_PARENTESE, file);
                identificar_lista_expressao(lookahead, file);
                consome(lookahead, FECHA_PARENTESE, file);
            }
            break;
        case CONSTINT:
        case CONSTCHAR:
        case VERDADEIRO:
        case FALSO:
            consome(lookahead, lookahead->atomo, file);
            break;
        case ABRE_PARENTESE:
            consome(lookahead, ABRE_PARENTESE, file);
            identificar_expressao(lookahead, file);
            consome(lookahead, FECHA_PARENTESE, file);
            break;
        case SOMA:
        case SUBTRACAO:
        case NAO:
            consome(lookahead, lookahead->atomo, file);
            identificar_fator(lookahead, file);
            break;
        default:
            _erro_sintatico(lookahead, "fator");
    }
}

void identificar_operador_multiplicacao(TInfoAtomo* lookahead, FILE* file) {
    switch (lookahead->atomo) {
        case MULTIPLICACAO:
        case DIV:
        case E:
            consome(lookahead, lookahead->atomo, file);
            break;
        default:
            _erro_sintatico(lookahead, "operador de multiplicacao");
    }
}

void identificar_termo(TInfoAtomo* lookahead, FILE* file) {
    identificar_fator(lookahead, file);

    while (lookahead->atomo == MULTIPLICACAO ||
           lookahead->atomo == DIV || lookahead->atomo == E) {
        identificar_operador_multiplicacao(lookahead, file);
        identificar_fator(lookahead, file);
    }
}

void identificar_operador_adicao(TInfoAtomo* lookahead, FILE* file) {
    switch (lookahead->atomo) {
        case SOMA:
        case SUBTRACAO:
        case MOD:
        case OU:
            consome(lookahead, lookahead->atomo, file);
            break;
        default:
            _erro_sintatico(lookahead, "operador de adicao");
    }
}

void identificar_expressao_simples(TInfoAtomo* lookahead, FILE* file) {
    identificar_termo(lookahead, file);

    while (lookahead->atomo == SOMA || lookahead->atomo == SUBTRACAO ||
           lookahead->atomo == MOD || lookahead->atomo == OU) {
        identificar_operador_adicao(lookahead, file);
        identificar_termo(lookahead, file);
    }
}

void identificar_operador_relacional(TInfoAtomo* lookahead, FILE* file) {
    switch (lookahead->atomo) {
        case DIFERENTE:
        case MENOR:
        case MENOR_IGUAL:
        case MAIOR_IGUAL:
        case MAIOR:
        case IGUAL:
            consome(lookahead, lookahead->atomo, file);
            break;
        default:
            _erro_sintatico(lookahead, "operador relacional");
    }
}

void identificar_expressao(TInfoAtomo* lookahead, FILE* file) {
    identificar_expressao_simples(lookahead, file);

    if (lookahead->atomo == DIFERENTE || lookahead->atomo == MENOR ||
        lookahead->atomo == MENOR_IGUAL || lookahead->atomo == MAIOR_IGUAL ||
        lookahead->atomo == MAIOR || lookahead->atomo == IGUAL) {
        identificar_operador_relacional(lookahead, file);
        identificar_expressao_simples(lookahead, file);
    }
}

void identificar_lista_expressao(TInfoAtomo* lookahead, FILE* file) {
    identificar_expressao(lookahead, file);

    while (lookahead->atomo == VIRGULA) {
        consome(lookahead, VIRGULA, file);
        identificar_expressao(lookahead, file);
    }
}

void identificar_chamada_procedimento(TInfoAtomo* lookahead, FILE* file) {
    if (lookahead->atomo == ABRE_PARENTESE) {
        consome(lookahead, ABRE_PARENTESE, file);
        identificar_lista_expressao(lookahead, file);
        consome(lookahead, FECHA_PARENTESE, file);
    }
}

void identificar_comando_repeticao(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, ENQUANTO, file);
    identificar_expressao(lookahead, file);
    consome(lookahead, FACA, file);
    identificar_comando(lookahead, file);
}

void identificar_comando_condicional(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, SE, file);
    identificar_expressao(lookahead, file);
    consome(lookahead, ENTAO, file);
    identificar_comando(lookahead, file);

    if (lookahead->atomo == SENAO) {
        consome(lookahead, SENAO, file);
        identificar_comando(lookahead, file);
    }
}

void identificar_comando_saida(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, ESCREVA, file);
    consome(lookahead, ABRE_PARENTESE, file);
    identificar_lista_expressao(lookahead, file);
    consome(lookahead, FECHA_PARENTESE, file);
}

void identificar_comando_entrada(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, LEIA, file);
    consome(lookahead, ABRE_PARENTESE, file);
    consome(lookahead, IDENTIFICADOR, file);

    while (lookahead->atomo == VIRGULA) {
        consome(lookahead, VIRGULA, file);
        consome(lookahead, IDENTIFICADOR, file);
    }

    consome(lookahead, FECHA_PARENTESE, file);
}

void identificar_comando_atribuicao(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, ATRIBUICAO, file);
    identificar_expressao(lookahead, file);
}

void identificar_comando(TInfoAtomo* lookahead, FILE* file) {
    switch (lookahead->atomo) {
        case IDENTIFICADOR:
            // Atribuicao e chamada de procedimento comecam com identificador.
            consome(lookahead, IDENTIFICADOR, file);
            if (lookahead->atomo == ATRIBUICAO)
                identificar_comando_atribuicao(lookahead, file);
            else
                identificar_chamada_procedimento(lookahead, file);
            break;
        case LEIA:
            identificar_comando_entrada(lookahead, file);
            break;
        case ESCREVA:
            identificar_comando_saida(lookahead, file);
            break;
        case SE:
            identificar_comando_condicional(lookahead, file);
            break;
        case ENQUANTO:
            identificar_comando_repeticao(lookahead, file);
            break;
        case INICIO:
            identificar_comando_composto(lookahead, file);
            break;
        default:
            _erro_sintatico(lookahead, "comando");
    }
}

void identificar_comando_composto(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, INICIO, file);
    identificar_comando(lookahead, file);

    while (lookahead->atomo == PONTO_VIRGULA) {
        consome(lookahead, PONTO_VIRGULA, file);
        identificar_comando(lookahead, file);
    }

    consome(lookahead, FIM, file);
}

void identificar_parametro_formal(TInfoAtomo* lookahead, FILE* file) {
    if (lookahead->atomo == VAR)
        consome(lookahead, VAR, file);

    identificar_lista_variaveis(lookahead, file);
}

void identificar_parametros_formais(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, ABRE_PARENTESE, file);

    if (lookahead->atomo != FECHA_PARENTESE) {
        identificar_parametro_formal(lookahead, file);

        while (lookahead->atomo == PONTO_VIRGULA) {
            consome(lookahead, PONTO_VIRGULA, file);
            identificar_parametro_formal(lookahead, file);
        }
    }

    consome(lookahead, FECHA_PARENTESE, file);
}

void identificar_tipo(TInfoAtomo* lookahead, FILE* file) {
    switch (lookahead->atomo) {
        case CARACTERE:
            consome(lookahead, CARACTERE, file);
            break;
        case INTEIRO:
            consome(lookahead, INTEIRO, file);
            break;
        case LOGICO:
            consome(lookahead, LOGICO, file);
            break;
        default:
            _erro_sintatico(lookahead, "caractere | inteiro | logico");
    }
}

void identificar_declaracao_procedimento(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, PROCEDIMENTO, file);
    consome(lookahead, IDENTIFICADOR, file);
    identificar_parametros_formais(lookahead, file);
    identificar_declaracao_variaveis(lookahead, file);
    identificar_comando_composto(lookahead, file);
}

void identificar_declaracao_funcao(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, FUNCAO, file);
    identificar_tipo(lookahead, file);
    consome(lookahead, IDENTIFICADOR, file);
    identificar_parametros_formais(lookahead, file);
    identificar_declaracao_variaveis(lookahead, file);
    identificar_comando_composto(lookahead, file);
}

void identificar_declaracao_rotinas(TInfoAtomo* lookahead, FILE* file) {
    while (lookahead->atomo == FUNCAO || lookahead->atomo == PROCEDIMENTO) {
        if (lookahead->atomo == FUNCAO)
            identificar_declaracao_funcao(lookahead, file);
        else
            identificar_declaracao_procedimento(lookahead, file);
    }
}

void identificar_lista_variaveis(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, IDENTIFICADOR, file);
    while (lookahead->atomo == VIRGULA) {
        consome(lookahead, VIRGULA, file);
        consome(lookahead, IDENTIFICADOR, file);
    }
    consome(lookahead, DOIS_PONTOS, file);
    identificar_tipo(lookahead, file);
}

void identificar_declaracao_variaveis(TInfoAtomo* lookahead, FILE* file) {
    if (lookahead->atomo == VAR) {
        consome(lookahead, VAR, file);
        identificar_lista_variaveis(lookahead, file);
        consome(lookahead, PONTO_VIRGULA, file);

        while (lookahead->atomo == IDENTIFICADOR) {
            identificar_lista_variaveis(lookahead, file);
            consome(lookahead, PONTO_VIRGULA, file);
        }
    }
}

void identificar_bloco(TInfoAtomo* lookahead, FILE* file) {
    identificar_declaracao_variaveis(lookahead, file);
    identificar_declaracao_rotinas(lookahead, file);
    identificar_comando_composto(lookahead, file);
}

void identificar_programa(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, ALGORITMO, file);
    consome(lookahead, IDENTIFICADOR, file);
    consome(lookahead, PONTO_VIRGULA, file);
    identificar_bloco(lookahead, file);
    consome(lookahead, PONTO, file);

    if (lookahead->atomo != EOS)
        _erro_sintatico(lookahead, "fim do arquivo");
}

void analizar(FILE* file) {
    TInfoAtomo atomo;
    avancar(&atomo, file);
    identificar_programa(&atomo, file);

    printf("%d linhas analisadas, programa sintaticamente correto\n", atomo.linha);
}
