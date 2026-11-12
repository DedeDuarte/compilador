# Compilador Portugol
## Como compilar e rodar

Rode os seguintes comandos na pasta principal do projeto:

```sh
make
./compilador data/teste.man
```

O [Makefile](Makefile) executa o comando GCC abaixo (com todos os arquivos `.c` de [src/](src/) e o objeto de [lib/](lib/)):

```sh
gcc -Wall -Wextra -Wno-unused-result -Iinclude -g -Og src/*.c lib/*.o -o compilador
```

| Flag | Função |
| ---- | ------ |
| `-Wall -Wextra` | Ativam avisos do compilador |
| `-Wno-unused-result` | Silencia o aviso de resultado não usado |
| `-Iinclude` | Indica onde estão os arquivos `.h` |
| `src/*.c` | Inclui os arquivos `.c` dessa pasta |
| `lib/*.o` | Inclui o objeto [hash_mack.o](lib/hash_mack.o) (função `hashMack`, fornecida pelo professor) |
| `-g -Og` | Informações de depuração e otimização leve |
| `-o compilador` | Define o nome do executável |

Para usar outro arquivo de entrada, substitua `data/teste.man` pelo caminho desejado.  
Para compilar e rodar de uma vez, use `make run` (ou `make run ARQ=outro.man` para outro arquivo).  
Para apagar o executável, use `make clean`.

## Estrutura

- [src/](src/): código-fonte, um módulo por arquivo
  - [main.c](src/main.c): entrada do programa, abertura do arquivo e início da análise
  - [atomos.c](src/atomos.c): identificação de palavras reservadas
  - [lexico.c](src/lexico.c): leitura dos átomos
  - [verificadores.c](src/verificadores.c): reconhecimento de lexemas
  - [sintatico.c](src/sintatico.c): controle da análise sintática e tratamento de erros
  - [identificadores.c](src/identificadores.c): reconhecimento das regras da gramática
  - [utils.c](src/utils.c): exibição dos átomos
- [include/](include/): um `.h` para cada `.c` acima, exceto [main.c](src/main.c)
- [lib/](lib/): objeto pré-compilado [hash_mack.o](lib/hash_mack.o)
- [tests/](tests/): programa de teste da `hashMack`, [testa_hash_mack.c](tests/testa_hash_mack.c)
- [data/](data/): programas de entrada `.man` ([teste.man](data/teste.man), [teste1.man](data/teste1.man), [teste2.man](data/teste2.man))
- [docs/](docs/): enunciados em PDF ([proj_1b.pdf](docs/proj_1b.pdf), [proj_2b.pdf](docs/proj_2b.pdf)) e [CLAUDE.md](docs/CLAUDE.md)
- [Makefile](Makefile): comando de compilação
- [README.txt](README.txt): arquivo de entrega
