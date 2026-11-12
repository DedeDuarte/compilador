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

.PHONY: all run test clean
