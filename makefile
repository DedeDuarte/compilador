CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = main.c src/atomos.c src/lexico.c src/sintatico.c src/utils.c src/verificadores.c
TARGET = a.out

main:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: main
	./$(TARGET) data/file2.man

clean:
	rm -f $(TARGET)
