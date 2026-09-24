CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = main.c src/*.c
TARGET = a.out

main:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)