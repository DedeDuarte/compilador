/*
 * |==== Grupo (solo) ====|==========|
 * |         Nome         |    RA    |
 * |----------------------|----------|
 * | Andre Doerner Duarte | 10427938 |
 * |======================|==========|
 */

#include "mepa.h"

// Ultimo rotulo entregue
static int contador_rotulos = 0;

int novo_rotulo(void) {
    contador_rotulos++;
    return contador_rotulos;
}

static int base_temp = 0;

void definir_base_temporaria(int endereco) {
    base_temp = endereco;
}

int base_temporaria(void) {
    return base_temp;
}
