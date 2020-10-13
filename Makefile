# Makefile for configurator

CC=gcc
CCFLAFS=#-Wall -Werror -Wextra -pedantic
NAME=configurator
CLEAN=rm -f
BUILD=build
SRC=src
INCLUDE=include

$(BUILD)/$(NAME): $(BUILD)/main.o $(BUILD)/configurator.o
	$(CC) $(CCFLAFS) -o $@ $^

$(BUILD)/main.o: $(SRC)/main.c
	$(CC) $(CCFLAFS) -o $@ -c $(SRC)/main.c -I$(INCLUDE)

$(BUILD)/configurator.o: $(SRC)/configurator.c
	$(CC) $(CCFLAFS) -o $@ -c $(SRC)/configurator.c -I$(INCLUDE)

clean:
	$(CLEAN) *.o $(NAME)