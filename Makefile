CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-result -Iinclude -g -Og

SRC = src/*.c
LIB = lib/*.o
TARGET = compilador

ARQ = data/teste.man

all:
	$(CC) $(CFLAGS) $(SRC) $(LIB) -o $(TARGET)

run: all
	./$(TARGET) $(ARQ)
	rm -f $(TARGET)

clean:
	rm -f $(TARGET)

test:
	./tests/rodar_testes.sh

help:
	@echo "Comandos disponiveis:"
	@echo "  make        Compila e gera ./$(TARGET)"
	@echo "  make run    Compila, roda com ARQ e remove o executavel"
	@echo "  make test   Roda os testes"
	@echo "  make clean  Remove o executavel"
	@echo "  make help   Mostra esta ajuda"

.PHONY: all run test clean help
