#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atomos.h"
#include "utils.h"
#include "verificadores.h"
#include "lexico.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr,
            "ERRO\n"
            "Uso correto: ./%s <arquivo.fonte>\n",
            argv[1]
        );
        exit(1);
    }
    
    // Abre arquivo
    FILE* file = fopen(argv[1], "r");

    // Verifica se `file` foi aberto corretamente
    if (file == NULL) {
        fprintf(stderr, "Arquivo \"%s\" não foi aberto corretamente\nVerifique o caminho\n", argv[1]);
        exit(2);
    }

    obter_atomo(file);

    return 0;
}
