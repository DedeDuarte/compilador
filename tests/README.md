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

## Os arquivos de [esperado/](esperado/) vão mudar

Hoje eles guardam a saída da **fase 1** (os átomos reconhecidos). Na fase 2 o compilador deixa de imprimir átomos e passa a imprimir o código MEPA e a Tabela de Símbolos, então o objetivo desse teste muda: ele passará a conferir o MEPA gerado e as mensagens de erro (léxico, sintático e semântico).

Quando o formato da saída estabilizar, regrave os arquivos esperados (conferindo a saída antes):

```sh
make
for f in data/*.man; do ./compilador $f > tests/esperado/$(basename $f .man).txt 2>&1; done
make clean
```

Os testes 1, 2 e 4 não dependem do formato da saída e continuam valendo.
