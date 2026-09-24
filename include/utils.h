/*
 * |==== Grupo (solo) ====|==========|
 * |         Nome         |    RA    |
 * |----------------------|----------|
 * | Andre Doerner Duarte | 10427938 |
 * |======================|==========|
 */

#ifndef UTILS_H
#define UTILS_H

#include "atomos.h"

#define COR_NORMAL   "\033[0m"
#define COR_VERMELHO "\033[38;2;255;0;0m"

const char* atomo_para_str(TAtomo atomo);
void print_atomo(TInfoAtomo* atomo);

#endif
