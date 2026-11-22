/*
 * |==== Grupo (solo) ====|==========|
 * |         Nome         |    RA    |
 * |----------------------|----------|
 * | Andre Doerner Duarte | 10427938 |
 * |======================|==========|
 */

#include "identificadores.h"
#include "semantico.h"
#include "sintatico.h"

void identificar_fator(TInfoAtomo* lookahead, FILE* file) {
    switch (lookahead->atomo) {
        case IDENTIFICADOR: {
            TNo* no = buscar_identificador(lookahead);
            int linha = lookahead->linha;
            int qtd = 0;

            consome(lookahead, IDENTIFICADOR, file);
            if (lookahead->atomo == ABRE_PARENTESE) {
                verificar_chamada_funcao(no, linha);
                consome(lookahead, ABRE_PARENTESE, file);
                qtd = identificar_lista_expressao(lookahead, file);
                consome(lookahead, FECHA_PARENTESE, file);
            }
            else {
                verificar_valor(no, linha);
            }

            if (no->categoria == CAT_FUNCAO)
                verificar_argumentos(no, qtd, linha);
            break;
        }
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

// Devolve a quantidade de expressoes da lista
int identificar_lista_expressao(TInfoAtomo* lookahead, FILE* file) {
    int qtd = 1;

    identificar_expressao(lookahead, file);

    while (lookahead->atomo == VIRGULA) {
        consome(lookahead, VIRGULA, file);
        identificar_expressao(lookahead, file);
        qtd++;
    }

    return qtd;
}

// Devolve a quantidade de argumentos da chamada
int identificar_chamada_procedimento(TInfoAtomo* lookahead, FILE* file) {
    int qtd = 0;

    if (lookahead->atomo == ABRE_PARENTESE) {
        consome(lookahead, ABRE_PARENTESE, file);
        qtd = identificar_lista_expressao(lookahead, file);
        consome(lookahead, FECHA_PARENTESE, file);
    }

    return qtd;
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
    TNo* no;
    int linha;

    consome(lookahead, LEIA, file);
    consome(lookahead, ABRE_PARENTESE, file);

    no = buscar_identificador(lookahead);
    linha = lookahead->linha;
    consome(lookahead, IDENTIFICADOR, file);
    verificar_leitura(no, linha);

    while (lookahead->atomo == VIRGULA) {
        consome(lookahead, VIRGULA, file);

        no = buscar_identificador(lookahead);
        linha = lookahead->linha;
        consome(lookahead, IDENTIFICADOR, file);
        verificar_leitura(no, linha);
    }

    consome(lookahead, FECHA_PARENTESE, file);
}

void identificar_comando_atribuicao(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, ATRIBUICAO, file);
    identificar_expressao(lookahead, file);
}

void identificar_comando(TInfoAtomo* lookahead, FILE* file) {
    switch (lookahead->atomo) {
        case IDENTIFICADOR: {
            TNo* no = buscar_identificador(lookahead);
            int linha = lookahead->linha;

            consome(lookahead, IDENTIFICADOR, file);
            if (lookahead->atomo == ATRIBUICAO) {
                verificar_atribuicao(no, linha);
                identificar_comando_atribuicao(lookahead, file);
            }
            else {
                verificar_chamada_procedimento(no, linha);
                verificar_argumentos(no, identificar_chamada_procedimento(lookahead, file), linha);
            }
            break;
        }
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

    identificar_lista_variaveis(lookahead, file, CAT_PARAMETRO);
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

    calcular_enderecos_parametros();
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
    TNo* rotina = declarar_identificador(lookahead, CAT_PROCEDIMENTO);
    consome(lookahead, IDENTIFICADOR, file);
    entrar_rotina(rotina);
    identificar_parametros_formais(lookahead, file);
    identificar_declaracao_variaveis(lookahead, file);
    identificar_comando_composto(lookahead, file);
    sair_rotina();
}

void identificar_declaracao_funcao(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, FUNCAO, file);
    identificar_tipo(lookahead, file);
    TNo* rotina = declarar_identificador(lookahead, CAT_FUNCAO);
    consome(lookahead, IDENTIFICADOR, file);
    entrar_rotina(rotina);
    identificar_parametros_formais(lookahead, file);
    identificar_declaracao_variaveis(lookahead, file);
    identificar_comando_composto(lookahead, file);
    sair_rotina();
}

void identificar_declaracao_rotinas(TInfoAtomo* lookahead, FILE* file) {
    while (lookahead->atomo == FUNCAO || lookahead->atomo == PROCEDIMENTO) {
        if (lookahead->atomo == FUNCAO)
            identificar_declaracao_funcao(lookahead, file);
        else
            identificar_declaracao_procedimento(lookahead, file);
    }
}

void identificar_lista_variaveis(TInfoAtomo* lookahead, FILE* file, TCategoria categoria) {
    declarar_identificador(lookahead, categoria);
    consome(lookahead, IDENTIFICADOR, file);
    while (lookahead->atomo == VIRGULA) {
        consome(lookahead, VIRGULA, file);
        declarar_identificador(lookahead, categoria);
        consome(lookahead, IDENTIFICADOR, file);
    }
    consome(lookahead, DOIS_PONTOS, file);
    identificar_tipo(lookahead, file);
}

void identificar_declaracao_variaveis(TInfoAtomo* lookahead, FILE* file) {
    if (lookahead->atomo == VAR) {
        consome(lookahead, VAR, file);
        identificar_lista_variaveis(lookahead, file, CAT_VARIAVEL);
        consome(lookahead, PONTO_VIRGULA, file);

        while (lookahead->atomo == IDENTIFICADOR) {
            identificar_lista_variaveis(lookahead, file, CAT_VARIAVEL);
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
