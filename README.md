# Compilador Portugol

## Como compilar e rodar

Na pasta do projeto, execute:

```sh
make
./a.out data/file.man
```

O `make` executa o comando GCC abaixo (com todos os arquivos `.c` que estão em `src/`):

```sh
gcc -Wall -Wextra -Iinclude main.c src/*.c -o a.out
```

`-Wall -Wextra` ativam avisos do compilador, `-Iinclude` indica onde estão os arquivos `.h`, `src/*.c` inclui os arquivos `.c` dessa pasta e `-o a.out` define o nome do executável. Para usar outro arquivo de entrada, substitua `data/file.man` pelo caminho desejado. Para apagar o executável, use `make clean`.

## Estrutura

```text
.
├── main.c                 entrada do programa e abertura do arquivo
├── makefile               comando de compilação
├── README.md              instruções e estrutura do projeto
├── include/
│   ├── atomos.h           tipos dos átomos
│   ├── lexico.h           declaração do analisador léxico
│   ├── utils.h            declaração da impressão dos átomos
│   └── verificadores.h    declarações dos verificadores
├── src/
│   ├── atomos.c           arquivo dos átomos
│   ├── lexico.c           leitura dos átomos
│   ├── utils.c            impressão dos átomos
│   └── verificadores.c    reconhecimento de lexemas
├── data/
│   ├── file.man           exemplo de entrada
│   └── file2.man          outro exemplo de entrada
└── docs/
    └── proj.pdf          enunciado do projeto
```
