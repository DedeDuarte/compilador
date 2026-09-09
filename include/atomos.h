#ifndef ATOMOS_H
#define ATOMOS_H

typedef enum {
    ERRO,

    // Palavras reservadas
    ALGORITMO,      CARACTERE,      DIV,
    E,              ENQUANTO,       ENTAO,
    ESCREVA,        FACA,           FALSO,
    FIM,            FUNCAO,         INICIO,
    INTEIRO,        LEIA,           LOGICO,
    MOD,            OU,             PROCEDIMENTO,
    SE,             SENAO,          VAR,
    VERDADEIRO,

    // Identificadores e constantes
    IDENTIFICADOR,  CONSTINT,       CONSTCHAR,

    // Operadores
    ATRIBUICAO,     SOMA,           SUBTRACAO,
    MULTIPLICACAO,  MENOR,          MENOR_IGUAL,
    MAIOR,          MAIOR_IGUAL,    DIFERENTE,
    IGUAL,

    // Delimitadores
    PONTO_VIRGULA,  VIRGULA,        PONTO,
    DOIS_PONTOS,    ABRE_PARENTESE, FECHA_PARENTESE,

    // Controle
    COMENTARIO, EOS

} TAtomo;

typedef struct {
    TAtomo atomo;
    int linha;
    union {
        int numero;
        char id[16];
        char ch;
    } atributo;
} TInfoAtomo;

#endif
