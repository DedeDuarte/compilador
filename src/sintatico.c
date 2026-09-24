#include "sintatico.h"
#include "atomos.h"
#include "lexico.h"
#include "utils.h"

void analizar(FILE* file) {
    TInfoAtomo atomo;

    do {
        atomo = obter_atomo(file);
        print_atomo(&atomo);
    } while (atomo.atomo != EOS);
}