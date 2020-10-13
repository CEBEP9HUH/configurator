# Makefile for configurator

CC=gcc
CCFLAFS=-Wall -Werror -Wextra -pedantic
NAME=configurator
CLEAN=rm -f
SRC=./src

$(NAME): main.o
	$(CC) $(CCFLAFS) -o $@ $^

main.o: $(SRC)/main.c
	$(CC) $(CCFLAFS) -c $^

clean:
	$(CLEAN) *.o $(NAME)