CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

EXIT = a.out

main:
	$(CC) $(CFLAGS) main.c src/*.c -o $(EXIT)

clean:
	rm -f $(EXIT)