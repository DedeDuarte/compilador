CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = main.c src/*.c
TARGET = a.out

main:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: main
	./$(TARGET) data/file.man

clean:
	rm -f $(TARGET)
