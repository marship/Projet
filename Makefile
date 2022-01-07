CC=gcc
CFLAGS=-g -Wall -Werror -Wextra

SRC=$(wildcard *.c)
OBJ=$(patsubst %.c, %.o, $(SRC))
BIN=main

all: $(BIN)

main: $(OBJ)
	$(CC) $^ -o $@

clean:
	rm -f *.o

distclean: clean
	rm -f $(BIN)

re: distclean all
