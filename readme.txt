Github do projeto: https://github.com/DedeDuarte/compilador

Grupo (solo):
    Andre Doerner Duarte - 10427938

---

Compilador Portugol

Como compilar e executar:
    make
    ./compilador <nome do arquivo de entrada>

    Sem make:
    gcc -Wall -Wno-unused-result -g -Og -Iinclude src/*.c lib/hash_mack.o -o compilador

Parte concluida:
    O trabalho foi concluido por inteiro: analise lexica, sintatica e semantica, tabela de simbolos e geracao de codigo MEPA (com parametro var por referencia). A saida mostra o codigo MEPA e, depois dele, a Tabela de Simbolos.

Decisoes de design:
    1. A tabela de simbolos e uma só, com o escopo guardado em cada no. Ao sair de uma rotina, os identificadores locais continuam na tabela, desativados, para aparecerem na saida.
    2. A MEPA nao tem instrucao de resto. "a mod b" vira a - (a div b) * b, com duas variaveis temporarias. Por isso o AMEM do programa principal reserva duas posicoes a mais.
    3. A VM da materia nao tira os parametros da pilha no RTPR (o procedimento3.mepa da aula trava nela). Entao o compilador gera RTPR 0, e quem chama tira os parametros com DMEM n logo depois do CHPR. Sem isso, uma funcao f(1, 2) que retorna 100 imprimia 2.

Bugs conhecidos:
    1. Se houver erro, o MEPA gerado ate ali aparece antes da mensagem de erro.
