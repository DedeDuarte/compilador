CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-result -Iinclude -g -Og

SRC = src/*.c
TARGET = compilador

main:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
