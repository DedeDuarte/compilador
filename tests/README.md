# Testes automáticos

Rode na raiz do projeto:

```sh
make test
```

O [Makefile](../Makefile) chama o script [rodar_testes.sh](rodar_testes.sh), que roda 4 verificações:

1. **hashMack:** compila [testa_hash_mack.c](testa_hash_mack.c) com o [hash_mack.o](../lib/hash_mack.o) e compara a saída com [hash_esperado.txt](hash_esperado.txt).
2. **Warnings:** compila o projeto e falha se o gcc imprimir qualquer aviso.
3. **Saída do compilador:** roda o compilador em cada `.man` de [data/](../data/) e compara com o arquivo de mesmo nome em [esperado/](esperado/).
4. **Valgrind:** roda o compilador em cada `.man` e falha se o valgrind achar erro de memória.

## Os arquivos de [esperado/](esperado/)

Na fase 2 eles guardam o código MEPA, a Tabela de Símbolos e as mensagens de erro (léxico, sintático e semântico). Depois de mudar o formato da saída, regrave-os conferindo a saída antes:

```sh
make
for f in data/*.man; do ./compilador $f > tests/esperado/$(basename $f .man).txt 2>&1; done
make clean
```
