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

| Flag | Função |
| ---- | ------ |
| `-Wall -Wextra` | Ativam avisos do compilador |
| `-Iinclude` | Indica onde estão os arquivos `.h`
| `src/*.c` | Inclui os arquivos `.c` dessa pasta
| `-o a.out` | Define o nome do executável

Para usar outro arquivo de entrada, substitua `data/file.man` pelo caminho desejado.   
Para apagar o executável, use `make clean`.

## Estrutura

```text
.
├── main.c                 entrada do programa, abertura do arquivo e início da análise
├── makefile               comando de compilação
├── README.md              instruções e estrutura do projeto
├── include/
│   ├── atomos.h           tipos e informações dos átomos
│   ├── identificadores.h  declarações das regras da gramática
│   ├── lexico.h           declaração do analisador léxico
│   ├── sintatico.h        declarações do analisador sintático
│   ├── utils.h            declarações das funções de exibição dos átomos
│   └── verificadores.h    declarações dos verificadores léxicos
├── src/
│   ├── atomos.c           identificação de palavras reservadas
│   ├── identificadores.c  reconhecimento das regras da gramática
│   ├── lexico.c           leitura dos átomos
│   ├── sintatico.c        controle da análise sintática e tratamento de erros
│   ├── utils.c            exibição dos átomos
│   └── verificadores.c    reconhecimento de lexemas
├── data/
│   ├── file.man           exemplo de entrada
│   └── file2.man          outro exemplo de entrada
└── docs/
    └── proj.pdf           enunciado do projeto
```
