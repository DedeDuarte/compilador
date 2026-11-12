#!/bin/bash
# Roda os testes automáticos. Uso: make test (ou ./tests/rodar_testes.sh na raiz).
cd "$(dirname "$0")/.." || exit 1
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
falhas=0

# Imprime OK ou FALHA conforme o código de saída do último comando
resultado() {
    if [ $? -eq 0 ]; then echo "OK    $1"; else echo "FALHA $1"; falhas=$((falhas + 1)); fi
}

# 1. hashMack: compara a saída do programa do professor com a esperada
gcc -Wall -Wno-unused-result -g -Og tests/testa_hash_mack.c lib/hash_mack.o -o "$tmp/hash" &&
    "$tmp/hash" a b c d v1 v2 aa bb cc dd ee | diff -q - tests/hash_esperado.txt > /dev/null
resultado "hashMack"

# 2. Compilação sem nenhum warning
avisos=$(gcc -Wall -Wextra -Wno-unused-result -Iinclude -g -Og src/*.c lib/hash_mack.o -o "$tmp/compilador" 2>&1)
[ -z "$avisos" ]
resultado "compilação sem warnings"
[ -n "$avisos" ] && echo "$avisos"

for arq in data/*.man; do
    nome=$(basename "$arq" .man)

    # 3. Saída do compilador igual à esperada
    "$tmp/compilador" "$arq" 2>&1 | diff -q - "tests/esperado/$nome.txt" > /dev/null
    resultado "saída de $arq"

    # 4. Valgrind sem erros de memória (99 = código de erro do valgrind)
    valgrind -q --leak-check=yes --error-exitcode=99 "$tmp/compilador" "$arq" > /dev/null 2>&1
    [ $? -ne 99 ]
    resultado "valgrind em $arq"
done

[ $falhas -eq 0 ]
