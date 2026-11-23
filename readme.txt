Github do projeto: https://github.com/DedeDuarte/compilador

Grupo (solo):
    Andre Doerner Duarte - 10427938

---

Compilador Portugol

Como compilar e executar:
    make
    ./compilador <nome do arquivo de entrada>

Parte concluida:
    O trabalho foi concluido por inteiro: analise lexica, sintatica e semantica, tabela de simbolos e geracao de codigo MEPA. A saida mostra o codigo MEPA e, depois dele, a Tabela de Simbolos.

Decisoes de design:
    1. A tabela de simbolos e uma só, com o escopo guardado em cada no. Ao sair de uma rotina, os identificadores locais continuam na tabela, desativados, para aparecerem na saida.
    2. A MEPA nao tem instrucao de resto. "a mod b" vira a - (a div b) * b, com duas variaveis temporarias. Por isso o AMEM do programa principal reserva duas posicoes a mais.

Bugs conhecidos:
    1. A contagem pode indicar uma linha a mais se o arquivo terminar com uma quebra de linha (não consegui resolver kkk)
    2. O parametro "var" e aceito, mas tratado como parametro por valor.
    3. Se houver erro, o MEPA gerado ate ali aparece antes da mensagem de erro.
