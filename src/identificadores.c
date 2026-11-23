/*
 * |==== Grupo (solo) ====|==========|
 * |         Nome         |    RA    |
 * |----------------------|----------|
 * | Andre Doerner Duarte | 10427938 |
 * |======================|==========|
 */

#include "identificadores.h"
#include "mepa.h"
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
                printf("   AMEM 1\n");
                consome(lookahead, ABRE_PARENTESE, file);
                qtd = identificar_lista_expressao(lookahead, file);
                consome(lookahead, FECHA_PARENTESE, file);
            }
            else {
                verificar_valor(no, linha);
                if (no->categoria == CAT_FUNCAO)
                    printf("   AMEM 1\n");
            }

            if (no->categoria == CAT_FUNCAO) {
                verificar_argumentos(no, qtd, linha);
                printf("   CHPR L%d\n", no->rotulo);
            }
            else {
                printf("   CRVL %d,%d\n", no->escopo, no->endereco);
            }
            break;
        }
        case CONSTINT:
            printf("   CRCT %d\n", lookahead->atributo.numero);
            consome(lookahead, CONSTINT, file);
            break;
        case CONSTCHAR:
            printf("   CRCT %d\n", lookahead->atributo.ch);
            consome(lookahead, CONSTCHAR, file);
            break;
        case VERDADEIRO:
            printf("   CRCT 1\n");
            consome(lookahead, VERDADEIRO, file);
            break;
        case FALSO:
            printf("   CRCT 0\n");
            consome(lookahead, FALSO, file);
            break;
        case ABRE_PARENTESE:
            consome(lookahead, ABRE_PARENTESE, file);
            identificar_expressao(lookahead, file);
            consome(lookahead, FECHA_PARENTESE, file);
            break;
        case SOMA:
            consome(lookahead, SOMA, file);
            identificar_fator(lookahead, file);
            break;
        case SUBTRACAO:
            consome(lookahead, SUBTRACAO, file);
            identificar_fator(lookahead, file);
            printf("   INVR\n");
            break;
        case NAO:
            consome(lookahead, NAO, file);
            identificar_fator(lookahead, file);
            printf("   NEGA\n");
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
        TAtomo operador = lookahead->atomo;

        identificar_operador_multiplicacao(lookahead, file);
        identificar_fator(lookahead, file);

        if (operador == MULTIPLICACAO)
            printf("   MULT\n");
        else if (operador == DIV)
            printf("   DIVI\n");
        else
            printf("   CONJ\n");
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
        TAtomo operador = lookahead->atomo;

        identificar_operador_adicao(lookahead, file);
        identificar_termo(lookahead, file);

        if (operador == SOMA) {
            printf("   SOMA\n");
        }
        else if (operador == SUBTRACAO) {
            printf("   SUBT\n");
        }
        else if (operador == OU) {
            printf("   DISJ\n");
        }
        else {
            // A VM nao tem resto: a mod b = a - (a div b) * b, com a e b em 2 temporarias
            int t = base_temporaria();

            printf("   ARMZ 0,%d\n", t + 1);
            printf("   ARMZ 0,%d\n", t);
            printf("   CRVL 0,%d\n", t);
            printf("   CRVL 0,%d\n", t);
            printf("   CRVL 0,%d\n", t + 1);
            printf("   DIVI\n");
            printf("   CRVL 0,%d\n", t + 1);
            printf("   MULT\n");
            printf("   SUBT\n");
        }
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
        TAtomo operador = lookahead->atomo;

        identificar_operador_relacional(lookahead, file);
        identificar_expressao_simples(lookahead, file);

        switch (operador) {
            case DIFERENTE:    printf("   CMDG\n"); break;
            case MENOR:        printf("   CMME\n"); break;
            case MENOR_IGUAL:  printf("   CMEG\n"); break;
            case MAIOR_IGUAL:  printf("   CMAG\n"); break;
            case MAIOR:        printf("   CMMA\n"); break;
            default:           printf("   CMIG\n"); break;
        }
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
    int inicio = novo_rotulo();
    int fim = novo_rotulo();

    consome(lookahead, ENQUANTO, file);
    printf("L%d:NADA\n", inicio);
    identificar_expressao(lookahead, file);
    printf("   DSVF L%d\n", fim);
    consome(lookahead, FACA, file);
    identificar_comando(lookahead, file);
    printf("   DSVS L%d\n", inicio);
    printf("L%d:NADA\n", fim);
}

void identificar_comando_condicional(TInfoAtomo* lookahead, FILE* file) {
    int senao = novo_rotulo();

    consome(lookahead, SE, file);
    identificar_expressao(lookahead, file);
    printf("   DSVF L%d\n", senao);
    consome(lookahead, ENTAO, file);
    identificar_comando(lookahead, file);

    if (lookahead->atomo == SENAO) {
        int fim = novo_rotulo();

        printf("   DSVS L%d\n", fim);
        printf("L%d:NADA\n", senao);
        consome(lookahead, SENAO, file);
        identificar_comando(lookahead, file);
        printf("L%d:NADA\n", fim);
    }
    else {
        printf("L%d:NADA\n", senao);
    }
}

void identificar_comando_saida(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, ESCREVA, file);
    consome(lookahead, ABRE_PARENTESE, file);

    identificar_expressao(lookahead, file);
    printf("   IMPR\n");

    while (lookahead->atomo == VIRGULA) {
        consome(lookahead, VIRGULA, file);
        identificar_expressao(lookahead, file);
        printf("   IMPR\n");
    }

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
    printf("   LEIT\n");
    printf("   ARMZ %d,%d\n", no->escopo, no->endereco);

    while (lookahead->atomo == VIRGULA) {
        consome(lookahead, VIRGULA, file);

        no = buscar_identificador(lookahead);
        linha = lookahead->linha;
        consome(lookahead, IDENTIFICADOR, file);
        verificar_leitura(no, linha);
        printf("   LEIT\n");
        printf("   ARMZ %d,%d\n", no->escopo, no->endereco);
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

                if (no->categoria == CAT_FUNCAO)
                    printf("   ARMZ 1,%d\n", -(no->qtd_parametros + 3));
                else
                    printf("   ARMZ %d,%d\n", no->escopo, no->endereco);
            }
            else {
                verificar_chamada_procedimento(no, linha);
                verificar_argumentos(no, identificar_chamada_procedimento(lookahead, file), linha);
                printf("   CHPR L%d\n", no->rotulo);
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
    rotina->rotulo = novo_rotulo();
    printf("L%d:NADA\n", rotina->rotulo);
    printf("   ENPR\n");
    entrar_rotina(rotina);
    identificar_parametros_formais(lookahead, file);
    identificar_declaracao_variaveis(lookahead, file);
    int m = total_variaveis();
    printf("   AMEM %d\n", m);
    identificar_comando_composto(lookahead, file);
    printf("   DMEM %d\n", m);
    printf("   RTPR %d\n", rotina->qtd_parametros);
    sair_rotina();
}

void identificar_declaracao_funcao(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, FUNCAO, file);
    identificar_tipo(lookahead, file);
    TNo* rotina = declarar_identificador(lookahead, CAT_FUNCAO);
    consome(lookahead, IDENTIFICADOR, file);
    rotina->rotulo = novo_rotulo();
    printf("L%d:NADA\n", rotina->rotulo);
    printf("   ENPR\n");
    entrar_rotina(rotina);
    identificar_parametros_formais(lookahead, file);
    identificar_declaracao_variaveis(lookahead, file);
    int m = total_variaveis();
    printf("   AMEM %d\n", m);
    identificar_comando_composto(lookahead, file);
    printf("   DMEM %d\n", m);
    printf("   RTPR %d\n", rotina->qtd_parametros);
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
    int n;
    int principal;

    identificar_declaracao_variaveis(lookahead, file);

    // As rotinas zeram o contador, entao o total e guardado agora
    // Depois das variaveis, 2 posicoes temporarias para o mod
    n = total_variaveis();
    definir_base_temporaria(n);
    printf("   AMEM %d\n", n + 2);

    if (lookahead->atomo == FUNCAO || lookahead->atomo == PROCEDIMENTO) {
        principal = novo_rotulo();
        printf("   DSVS L%d\n", principal);
        identificar_declaracao_rotinas(lookahead, file);
        printf("L%d:NADA\n", principal);
    }

    identificar_comando_composto(lookahead, file);

    printf("   DMEM %d\n", n + 2);
}

void identificar_programa(TInfoAtomo* lookahead, FILE* file) {
    consome(lookahead, ALGORITMO, file);
    consome(lookahead, IDENTIFICADOR, file);
    consome(lookahead, PONTO_VIRGULA, file);

    printf("   INPP\n");
    identificar_bloco(lookahead, file);
    consome(lookahead, PONTO, file);
    printf("   PARA\n");

    if (lookahead->atomo != EOS)
        _erro_sintatico(lookahead, "fim do arquivo");
}
